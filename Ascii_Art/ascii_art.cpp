#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <string>
#include <map>
#include <chrono>
#include <thread>
#include <functional>
#include <termios.h>
#include <signal.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ascii_art.h"

std::mutex mtx; // mutex for thread-safe control over recursiveDepth
std::atomic<bool> running{true};
termios orig_termios;

//---------------------------------------------------------------------------
//  Ink access functions
//---------------------------------------------------------------------------

bool acquireRedInk(int theRed);
bool acquireGreenInk(int theGreen);
bool acquireBlueInk(int theBlue);
bool refillRedInk(int theRed);
bool refillGreenInk(int theGreen);
bool refillBlueInk(int theBlue);

//---------------------------------------------------------------------------
//  Private function prototypes
//---------------------------------------------------------------------------

void fillTank(int y, int LEVEL_WIDTH);
void myKeyboard(unsigned char c);
void myEventLoop(int val);
void cleanup();

//---------------------------------------------------------------------------
//  Interface constants
//---------------------------------------------------------------------------

extern bool DRAW_COLORED_TRAVELER_HEADS;
extern int inklingSleepTime;

//---------------------------------------------------------------------------
//  File-level global variables
//---------------------------------------------------------------------------

void (*gridDisplayFunc)(void);
void (*stateDisplayFunc)(void);

extern int MAX_LEVEL;
extern int MAX_ADD_INK;
extern int MAX_NUM_TRAVELER_THREADS;

extern int producerSleepTime;

// path to the pipe
std::string pipePath = "/tmp/my_pipe";
static unsigned int numPipesOpen = 0;
static int pipe_fd = -1;

//---------------------------------------------------------------------------
//  Util Terminal Print ASCII functions
//---------------------------------------------------------------------------

void setTextColor(TextColor color) {
    std::cout << "\033[" << static_cast<int>(color) << "m";
}

void resetTextColor() {
    setTextColor(TextColor::DEFAULT);
}

void print() {
    resetTextColor();
    std::cout << std::endl;
}

template <typename T, typename... Args>
void print(const T& first, const Args&... rest) {
    if constexpr (std::is_same_v<T, TextColor>) {
        setTextColor(first);
    } else {
        std::cout << first;
    }
    print(rest...);
}

void printCell() {
    resetTextColor();
}

template <typename T, typename... Args>
void printCell(const T& first, const Args&... rest) {
    if constexpr (std::is_same_v<T, TextColor>) {
        setTextColor(first);
    } else {
        std::cout << first;
    }
    printCell(rest...);
}

void clearTerminal() {
    std::cout << "\033[H\033[J";
}

//---------------------------------------------------------------------------
//  Drawing functions
//---------------------------------------------------------------------------

void drawGridAndInklingsASCII(int** grid, int numRows, int numCols, std::vector<InklingInfo>& inklingList) {
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            bool inklingFound = false;
            for (auto inkling : inklingList) {
                if (inkling.isLive && row == inkling.row && col == inkling.col) {
                    switch ((int)(inkling.type)) {
                        case RED_TRAV:
                            printCell(TextColor::RED, "[", iconDirections[inkling.dir], "]");
                            inklingFound = true;
                            break;
                        case GREEN_TRAV:
                            printCell(TextColor::GREEN, "[", iconDirections[inkling.dir], "]");
                            inklingFound = true;
                            break;
                        case BLUE_TRAV:
                            printCell(TextColor::BLUE, "[", iconDirections[inkling.dir], "]");
                            inklingFound = true;
                            break;
                    }
                }
            }
            if(!inklingFound) {
                printCell(TextColor::BLACK, "[ ]");
            }
        }
        std::cout << std::endl;
    }
}

void fillTank(int y, int LEVEL_WIDTH) {
    print("fillTank", y, LEVEL_WIDTH);
}

void drawState(int numLiveThreads, int redLevel, int greenLevel, int blueLevel) {
    print(TextColor::BLACK, "Ink Tank Levels, the MAX is: ", MAX_LEVEL);
    print(TextColor::RED, "Red: ", redLevel, TextColor::GREEN, " Green: ", greenLevel, TextColor::BLUE, " Blue: ", blueLevel);
    print("Live Threads: ", numLiveThreads);
    print();

    if(numLiveThreads == 0) {
        cleanupAndQuit("drawState: no threads left.");
    }
}

void updateTerminal(void) {
    try {
        clearTerminal();
        gridDisplayFunc();
        stateDisplayFunc();
    } catch (const std::exception& e) {
        std::cerr << "ERROR :/ updateTerminal :: exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "ERROR :/ updateTerminal :: unknown exception" << std::endl;
    }
}

//---------------------------------------------------------------------------
//  Keyboard functions
//---------------------------------------------------------------------------

void cleanup() {
    running = false;
    
    // Restore original terminal settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    
    // Close pipe if open
    if (pipe_fd != -1) {
        close(pipe_fd);
        pipe_fd = -1;
        numPipesOpen = 0;
    }
}

void signalHandler(int signum) {
    cleanup();
    exit(signum);
}

void enableRawMode() {
    // Save original terminal settings
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
        throw std::runtime_error("Failed to get terminal attributes");
    }
    
    // Register cleanup on program exit
    std::atexit(cleanup);
    
    // Register signal handlers
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);
    
    // Configure raw mode
    termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;  // No minimum input
    raw.c_cc[VTIME] = 1; // 100ms timeout
    
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        throw std::runtime_error("Failed to set terminal attributes");
    }
}

void keyListener() {
    try {
        enableRawMode();
        
        while (running) {
            char c;
            if (read(STDIN_FILENO, &c, 1) == 1) {
                std::lock_guard<std::mutex> lock(mtx);
                myKeyboard(c);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR :/ keyListener :: exception: " << e.what() << std::endl;
        cleanup();
    }
}

void myKeyboard(unsigned char c) {
    bool ok = false;
    switch (c) {
        case 27:
            cleanupAndQuit("ESC pressed, quitting...");
            break;
        case 'r':
            ok = refillRedInk(MAX_ADD_INK);
            break;
        case 'g':
            ok = refillGreenInk(MAX_ADD_INK);
            break;
        case 'b':
            ok = refillBlueInk(MAX_ADD_INK);
            break;
        default:
            ok = true;
            break;
    }

    if (!ok) {
        std::cerr << "Refill action failed." << std::endl;
    }
}

//---------------------------------------------------------------------------
//  Timer functions
//---------------------------------------------------------------------------

void customTimerFunc(int milliseconds, std::function<void(int)> func, int val) {
    std::thread([=]() {
        try {
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
            if (running) {
                func(val);
            }
        } catch (const std::exception& e) {
            std::cerr << "ERROR :/ customTimerFunc :: exception: " << e.what() << std::endl;
        }
    }).detach();
}

void myEventLoop(int val) {
    updateTerminal();
    static std::atomic<int> recursiveDepth = 0;
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (recursiveDepth >= 5) return;
        ++recursiveDepth;
    }
    
    if (running) {
        customTimerFunc(1000, myEventLoop, val);
    }
    
    {
        std::lock_guard<std::mutex> lock(mtx);
        --recursiveDepth;
    }

    if (access(pipePath.c_str(), F_OK) == 0 && numPipesOpen == 0 && running) {
        pipe_fd = open(pipePath.c_str(), O_RDONLY | O_NONBLOCK);
        if (pipe_fd != -1) {
            numPipesOpen++;
        }
    }
    
    if (pipe_fd > 0) {
        char command;
        int bytes_read = read(pipe_fd, &command, sizeof(command));
        if (bytes_read > 0) {
            std::lock_guard<std::mutex> lock(mtx);
            myKeyboard(command);
        }
    }
}

void initializeFrontEnd(int argc, char** argv, void (*gridDisplayCB)(void), void (*stateDisplayCB)(void)) {
    gridDisplayFunc = gridDisplayCB;
    stateDisplayFunc = stateDisplayCB;
    updateTerminal();
    
    // Start keyboard listener in a separate thread
    std::thread(keyListener).detach();
}