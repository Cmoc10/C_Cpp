#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <algorithm>
#include <sys/stat.h>
// Structure to hold a log entry
struct LogEntry {
    std::string timestamp;
    std::string content;
    
    bool operator<(const LogEntry& other) const {
        return timestamp < other.timestamp;
    }
};

// Function to combine and sort all log files
void combineLogFiles() {
    std::vector<LogEntry> allEntries;
    
    // Read all log files from logFolder
    for (const auto& entry : std::filesystem::directory_iterator("logFolder")) {
        if (entry.path().filename() != "actions.txt") {
            std::ifstream logFile(entry.path());
            std::string line;
            
            while (std::getline(logFile, line)) {
                size_t commaPos = line.find(',');
                if (commaPos != std::string::npos) {
                    LogEntry logEntry;
                    logEntry.timestamp = line.substr(0, commaPos);
                    logEntry.content = line;
                    allEntries.push_back(logEntry);
                }
            }
        }
    }
    
    // Sort entries by timestamp
    std::sort(allEntries.begin(), allEntries.end());
    
    // Write sorted entries to actions.txt
    std::string actionsFile = "logFolder/actions.txt";
    std::ofstream outFile(actionsFile);
    chmod(actionsFile.c_str(), 0755);
    
    if (outFile.is_open()) {
        for (const auto& entry : allEntries) {
            outFile << entry.content << std::endl;
        }
        outFile.close();
    }
}

// Main function to handle log combination
int main() {
    if (std::filesystem::exists("logFolder")) {
        combineLogFiles();
        std::cout << "Log files combined and sorted in actions.txt" << std::endl;
    } else {
        std::cerr << "logFolder directory not found" << std::endl;
        return 1;
    }
    return 0;
}