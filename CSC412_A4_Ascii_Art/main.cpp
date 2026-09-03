//
//  main.cpp
//  inklings
//
//  Authors: Jean-Yves Hervé, Shaun Wallace, and Luis Hernandez
//

 /*-------------------------------------------------------------------------+
 |	A graphic front end for a grid+state simulation.						|
 |																			|
 |	This application simply creates a a colored grid and displays           |
 |  some state information in the terminal using ASCII art.			        |
 |	Only mess with this after everything else works and making a backup		|
 |	copy of your project.                                                   |
 |																			|
 |	Current Keyboard Events                                     			|
 |		- 'ESC' --> exit the application									|
 |		- 'r' --> add red ink												|
 |		- 'g' --> add green ink												|
 |		- 'b' --> add blue ink												|
 +-------------------------------------------------------------------------*/

#include <random>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>

#include "ascii_art.h"

//==================================================================================
//	Function prototypes
//==================================================================================
void displayGridPane(void);
void displayStatePane(void);
void initializeApplication(void);
void threadFunction(InklingInfo* inkling);
void getNewDirection(InklingInfo* inkling);
bool checkIfInCorner(InklingInfo* inkling);
void redColorThreadFunc();
void greenColorThreadFunc();
void blueColorThreadFunc();
bool checkEnoughInk(InklingInfo* inkling, int moveAmount);

//==================================================================================
//	Application-level global variables
//==================================================================================


//	The state grid and its dimensions
int** grid;
int NUM_ROWS, NUM_COLS;

//	the number of live threads (that haven't terminated yet)
int MAX_NUM_TRAVELER_THREADS;
int numLiveThreads = 0;

//vector to store each struct
std::vector<InklingInfo> info;
bool DRAW_COLORED_TRAVELER_HEADS = true;

//	the ink levels
int MAX_LEVEL = 50;
int MAX_ADD_INK = 10;
int REFILL_INK = 10;
int redLevel = 20, greenLevel = 10, blueLevel = 40;

// create locks for color levels
std::mutex redLock;
std::mutex blueLock;
std::mutex greenLock;
std::mutex blueCellLock;
std::mutex redCellLock;
std::mutex greenCellLock;

// ink producer sleep time (in microseconds)
// [min sleep time is arbitrary]
const int MIN_SLEEP_TIME = 30000; // 30000
int producerSleepTime = 100000; // 100000

// inkling sleep time (in microseconds)
int inklingSleepTime = 1000000; // 1000000


//==================================================================================
//	These are the functions that tie the simulation with the rendering.
//	Some parts are "don't touch."  Other parts need your help to ensure
//	that access to critical data and the ASCII art are properly synchronized
//==================================================================================

void displayGridPane(void) {
	//---------------------------------------------------------
	//	This is the call that writes ASCII art to render the grid.
	//
	//	Should we synchronize this call?
	//---------------------------------------------------------
    drawGridAndInklingsASCII(grid, NUM_ROWS, NUM_COLS, info);
}

void displayStatePane(void) {
	//---------------------------------------------------------
	//	This is the call that updates state information
	//
	//	Should we synchronize this call?
	//---------------------------------------------------------
	drawState(numLiveThreads, redLevel, greenLevel, blueLevel);
}

//------------------------------------------------------------------------
//	These are the functions that would be called by a inkling thread in
//	order to acquire red/green/blue ink to trace its trail.
//	You *must* synchronize access to the ink levels (C++ lock and unlock)
//------------------------------------------------------------------------
// You probably want to edit these...
bool acquireRedInk(int theRed) {
	bool ok = false;
	if (redLevel >= theRed)
	{
		redLevel -= theRed;
		ok = true;
	}
	return ok;
}

bool acquireGreenInk(int theGreen) {
	bool ok = false;
	if (greenLevel >= theGreen)
	{
		greenLevel -= theGreen;
		ok = true;
	}
	return ok;
}

bool acquireBlueInk(int theBlue) {
	bool ok = false;
	if (blueLevel >= theBlue)
	{
		blueLevel -= theBlue;
		ok = true;
	}
	return ok;
}


//------------------------------------------------------------------------
//	These are the functions that would be called by a producer thread in
//	order to refill the red/green/blue ink tanks.
//	You *must* synchronize access to the ink levels (C++ lock and unlock)
//------------------------------------------------------------------------
// You probably want to edit these...
bool refillRedInk(int theRed) {
	bool ok = false;
	if (redLevel + theRed <= MAX_LEVEL)
	{
		redLevel += theRed;
		ok = true;
	}
	return ok;
}

bool refillGreenInk(int theGreen) {
	bool ok = false;
	if (greenLevel + theGreen <= MAX_LEVEL)
	{
		greenLevel += theGreen;
		ok = true;
	}
	return ok;
}

bool refillBlueInk(int theBlue) {
	bool ok = false;
	if (blueLevel + theBlue <= MAX_LEVEL)
	{
		blueLevel += theBlue;
		ok = true;
	}
	return ok;
}

//------------------------------------------------------------------------
//	You shouldn't have to touch this one.  Definitely if you do not
//	add the "producer" threads, and probably not even if you do.
//------------------------------------------------------------------------
void speedupProducers(void) {
	// decrease sleep time by 20%, but don't get too small
	int newSleepTime = (8 * producerSleepTime) / 10;
	
	if (newSleepTime > MIN_SLEEP_TIME) {
		producerSleepTime = newSleepTime;
	}
}

void slowdownProducers(void) {
	// increase sleep time by 20%
	producerSleepTime = (12 * producerSleepTime) / 10;
}

//-------------------------------------------------------------------------------------
//	You need to change the TODOS in the main function to pass the the autograder tests
//-------------------------------------------------------------------------------------
int main(int argc, char** argv) {
    // a try/catch block for debugging to catch weird errors in your code
    try {
        // check that arguments are valid, must be a 20x20 or greater and at least 8 threads/inklings
        if (argc == 4) {
            if (std::stoi(argv[1]) >= 20 && std::stoi(argv[2]) >= 20 && std::stoi(argv[3]) >= 8) {
                NUM_ROWS = std::stoi(argv[1]);
                NUM_COLS = std::stoi(argv[2]);
                MAX_NUM_TRAVELER_THREADS = std::stoi(argv[3]);
                numLiveThreads = std::stoi(argv[3]);
            }
        } else {
            std::cout << "No arguments provided, running with 8x8 grid and 4 threads.\n\tThis message will disappear in 2 seconds... \n";
            sleep(2); // so the user can read the message in std::cout one line up
            clearTerminal();
            // some small defaults, will these run?
            NUM_ROWS = 8;
            NUM_COLS = 8;
            MAX_NUM_TRAVELER_THREADS = 4;
            numLiveThreads = 4;
        }
        
        // Initialize the front end
        initializeFrontEnd(argc, argv, displayGridPane, displayStatePane);
        
        // Initialize the application's grid and inklings
        initializeApplication();

        //------------------------------------------------------------------------
        //	TODO: create producer threads that check the levels of each ink
        //------------------------------------------------------------------------

        // Create producer threads for each color to monitor and refill ink levels
        std::thread redProducer(redColorThreadFunc);
        std::thread greenProducer(greenColorThreadFunc);
        std::thread blueProducer(blueColorThreadFunc);

        //------------------------------------------------------------------------
        //	TODO: create threads for the inklings
        //------------------------------------------------------------------------
        
        // Create a vector to hold each inkling's thread
        std::vector<std::thread> inklingThreads;
        for (auto& inkling : info) {
            inklingThreads.emplace_back(threadFunction, &inkling);
        }
        
        // Now we enter the main event loop of the program
        myEventLoop(0);

        // Ensure main does not return immediately, killing detached threads
        std::this_thread::sleep_for(std::chrono::seconds(30));

        // Join all threads before program exit
        redProducer.join();
        greenProducer.join();
        blueProducer.join();
        
        for (auto& inklingThread : inklingThreads) {
            inklingThread.join();
        }

    } catch (const std::exception& e) {
        std::cerr << "ERROR :: Oh snap! Unhandled exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "ERROR :: Red handed! Unknown exception caught" << std::endl;
    }

    return 0;
}



//==================================================================================
//
//	TODO this is a part that you have to edit and add to.
//
//==================================================================================

//-------------------------------------------------------------------------------------
//	Main implementation
//-------------------------------------------------------------------------------------

// Function to get current timestamp in required format
std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    auto now_t = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_t), "%H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << now_ms.count();
    
    return ss.str();
}

// Function to create a new log file for an inkling
void createInklingLog(int inklingNum, const std::string& color, int startRow, int startCol) {
    std::filesystem::create_directories("logFolder");
    chmod("logFolder", 0755);
    
    std::string filename = "logFolder/inkling" + std::to_string(inklingNum) + ".txt";
    std::ofstream logFile(filename);
    chmod(filename.c_str(), 0755);
    
    if (logFile.is_open()) {
        std::string timestamp = getCurrentTimestamp();
        logFile << timestamp << ",inkling" << inklingNum << "," << color 
                << ",row" << startRow << ",col" << startCol << std::endl;
        logFile.close();
    }
}

// Function to log inkling movement
void logInklingMove(int inklingNum, const std::string& direction, int row, int col) {
    std::string filename = "logFolder/inkling" + std::to_string(inklingNum) + ".txt";
    std::ofstream logFile(filename, std::ios::app);
    
    if (logFile.is_open()) {
        std::string timestamp = getCurrentTimestamp();
        logFile << timestamp << ",inkling" << inklingNum << "," << direction 
                << ",row" << row << ",col" << col << std::endl;
        logFile.close();
    }
}

// Function to log inkling termination
void logInklingTermination(int inklingNum) {
    std::string filename = "logFolder/inkling" + std::to_string(inklingNum) + ".txt";
    std::ofstream logFile(filename, std::ios::app);
    
    if (logFile.is_open()) {
        std::string timestamp = getCurrentTimestamp();
        logFile << timestamp << ",inkling" << inklingNum << ",terminated" << std::endl;
        logFile.close();
    }
}

void cleanupAndQuit(const std::string& msg) {
    // Join all threads before cleanup
    for (auto& inkling : info) {
        inkling.isLive = false;
    }
    
    std::cout << "Somebody called quits, goodbye sweet digital world, this was their message: \n" << msg;
    
    for (int i=0; i< NUM_ROWS; i++)
        delete []grid[i];
    delete []grid;

    exit(0);
}

void initializeApplication(void) {
    grid = new int*[NUM_ROWS];
    for (int i=0; i<NUM_ROWS; i++)
        grid[i] = new int[NUM_COLS];
    
    std::random_device myRandDev;
    std::default_random_engine myEngine(myRandDev());
    std::uniform_int_distribution<int> rowDist(1, NUM_ROWS-2); // Avoid corners
    std::uniform_int_distribution<int> colDist(1, NUM_COLS-2); // Avoid corners
    std::uniform_int_distribution<int> typeDist(0, NUM_TRAV_TYPES-1);
    std::uniform_int_distribution<int> dirDist(0, NUM_TRAVEL_DIRECTIONS-1);
    
    // Initialize grid
    for (int i=0; i<NUM_ROWS; i++) {
        for (int j=0; j<NUM_COLS; j++) {
            grid[i][j] = 0;
        }
    }

    // Create inklings at random positions
    for (int i = 0; i < MAX_NUM_TRAVELER_THREADS; i++) {
        int row, col;
        bool validPosition;
        
        // Keep trying until we find an unoccupied position
        do {
            validPosition = true;
            row = rowDist(myEngine);
            col = colDist(myEngine);
            
            // Check if position is already occupied
            for (const auto& existing : info) {
                if (existing.row == row && existing.col == col) {
                    validPosition = false;
                    break;
                }
            }
        } while (!validPosition);

        InklingInfo inkling = {
            static_cast<InklingType>(typeDist(myEngine)),
            row,
            col,
            static_cast<TravelDirection>(dirDist(myEngine)),
            true
        };
        info.push_back(inkling);
        std::string color = (inkling.type == RED_TRAV) ? "red" : (inkling.type == GREEN_TRAV) ? "green" : "blue";
        createInklingLog(i + 1, color, inkling.row, inkling.col);
    }
}

void threadFunction(InklingInfo* inkling) {
    while (inkling->isLive) {
        // Check if in corner
        if (checkIfInCorner(inkling)) {
            inkling->isLive = false;
            numLiveThreads--;
            
            // Log termination
            logInklingTermination(inkling - &info[0] + 1);
            return;
        }

        // Get new direction and move
        getNewDirection(inkling);

        // Log movement
        std::string direction = (inkling->dir == NORTH) ? "north" :
                                (inkling->dir == SOUTH) ? "south" :
                                (inkling->dir == EAST)  ? "east"  : "west";
        logInklingMove(inkling - &info[0] + 1, direction, inkling->row, inkling->col);
        
        std::this_thread::sleep_for(std::chrono::microseconds(inklingSleepTime));
    }
}

void getNewDirection(InklingInfo* inkling) {
    std::random_device rd;
    std::default_random_engine eng(rd());
    
    // Get perpendicular directions
    std::vector<TravelDirection> validDirs;
    if (inkling->dir == NORTH || inkling->dir == SOUTH) {
        if (inkling->col > 0) validDirs.push_back(WEST);
        if (inkling->col < NUM_COLS-1) validDirs.push_back(EAST);
    } else {
        if (inkling->row > 0) validDirs.push_back(NORTH);
        if (inkling->row < NUM_ROWS-1) validDirs.push_back(SOUTH);
    }
    
    if (!validDirs.empty()) {
        std::uniform_int_distribution<> dist(0, validDirs.size()-1);
        TravelDirection newDir = validDirs[dist(eng)];
        
        // Calculate move amount
        int maxMove = 1;
        if (newDir == NORTH) maxMove = inkling->row;
        else if (newDir == SOUTH) maxMove = NUM_ROWS - 1 - inkling->row;
        else if (newDir == WEST) maxMove = inkling->col;
        else maxMove = NUM_COLS - 1 - inkling->col;
        
        if (maxMove > 0) {
            std::uniform_int_distribution<> moveDist(1, maxMove);
            int moveAmount = moveDist(eng);
            
            if (checkEnoughInk(inkling, moveAmount)) {
                // Update position
                switch (newDir) {
                    case NORTH: inkling->row -= moveAmount; break;
                    case SOUTH: inkling->row += moveAmount; break;
                    case WEST: inkling->col -= moveAmount; break;
                    case EAST: inkling->col += moveAmount; break;
                    default: break;
                }
                inkling->dir = newDir;
            }
        }
    }
}

bool checkIfInCorner(InklingInfo* inkling) {
    return (inkling->row == 0 && inkling->col == 0) ||
           (inkling->row == 0 && inkling->col == NUM_COLS-1) ||
           (inkling->row == NUM_ROWS-1 && inkling->col == 0) ||
           (inkling->row == NUM_ROWS-1 && inkling->col == NUM_COLS-1);
}

bool checkEnoughInk(InklingInfo* inkling, int moveAmount) {
    bool hasInk = false;
    
    switch (inkling->type) {
        case RED_TRAV:
            redLock.lock();
            hasInk = acquireRedInk(moveAmount);
            redLock.unlock();
            break;
            
        case GREEN_TRAV:
            greenLock.lock();
            hasInk = acquireGreenInk(moveAmount);
            greenLock.unlock();
            break;
            
        case BLUE_TRAV:
            blueLock.lock();
            hasInk = acquireBlueInk(moveAmount);
            blueLock.unlock();
            break;
            
        default:
            break;
    }
    
    if (hasInk) {
        // Update grid with inkling's color
        switch (inkling->type) {
            case RED_TRAV:
                redCellLock.lock();
                grid[inkling->row][inkling->col] = RED_TRAV + 1;
                redCellLock.unlock();
                break;
                
            case GREEN_TRAV:
                greenCellLock.lock();
                grid[inkling->row][inkling->col] = GREEN_TRAV + 1;
                greenCellLock.unlock();
                break;
                
            case BLUE_TRAV:
                blueCellLock.lock();
                grid[inkling->row][inkling->col] = BLUE_TRAV + 1;
                blueCellLock.unlock();
                break;
                
            default:
                break;
        }
    }
    
    return hasInk;
}

void redColorThreadFunc() {
    while (numLiveThreads > 0) {
        redLock.lock();
        refillRedInk(REFILL_INK);
        redLock.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(producerSleepTime));
    }
}

void greenColorThreadFunc() {
    while (numLiveThreads > 0) {
        greenLock.lock();
        refillGreenInk(REFILL_INK);
        greenLock.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(producerSleepTime));
    }
}

void blueColorThreadFunc() {
    while (numLiveThreads > 0) {
        blueLock.lock();
        refillBlueInk(REFILL_INK);
        blueLock.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(producerSleepTime));
    }
}