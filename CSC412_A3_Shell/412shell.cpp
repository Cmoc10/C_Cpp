#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <fcntl.h> // for file descriptor manipulation functions

/*
 the data structure to hold the parsed command
 including any arguments and an optional output file for redirection
*/ 
struct ParsedCommand {
    std::vector<std::string> args; // stores command arguments
    std::string outputFile; // store the output file name for redirection, if any
};

// parses the user input into a ParsedCommand data structure above
ParsedCommand customParseInput(const std::string& input) {
    ParsedCommand command;
    std::string token;
    bool redirectOutput = false;
    bool inQuotes = false;
    for (size_t i = 0; i < input.length(); ++i) {
        char currentChar = input[i];
        // toggle inQuotes flag if a quote is encountered
        if (currentChar == '\"') {
            inQuotes = !inQuotes;
            continue; // skip adding the quote to the token
        }
        // handle space characters
        if (currentChar == ' ' && !inQuotes) {
            if (!token.empty()) {
                if (redirectOutput) {
                    command.outputFile = token;
                    break; // assuming only one output redirection per command
                } else {
                    command.args.push_back(token);
                    token.clear();
                }
            }
        } else if (currentChar == '>' && !inQuotes && token.empty()) {
            redirectOutput = true;
        } else {
            token += currentChar;
        }
    }
    // add the last token if it's not empty
    if (!token.empty()) {
        if (redirectOutput) {
            command.outputFile = token;
        } else {
            command.args.push_back(token);
        }
    }
    return command;
}

// converts a vector of std::string arguments to a vector of char* pointers required by execvp
std::vector<char*> convertArgs(const std::vector<std::string>& args) {
    std::vector<char*> cargs;
    for (const auto& arg : args) {
        cargs.push_back(const_cast<char*>(arg.c_str())); // safe cast since execvp doesn't modify arguments.
    }
    cargs.push_back(nullptr); // execvp expects a NULL-terminated array of arguments.
    return cargs;
}

/*
    This is the function you will use to fork to create a child process
    The child process will execute a user specified command (see the given ParsedCommand struct)
    The three major steps are listed as TODOs for you below
    Make sure you handle any errors and/or fork failure appropriately 
*/
void do_fork(const ParsedCommand& command) {
    if (command.args.empty()) {
        //just return if nothing there
        return; 
    }

    // Convert vector<string> to vector<char*> for execvp
    std::vector<char*> cargs = convertArgs(command.args);
    
    // Start forking it
    pid_t pid = fork();
    
    if (pid < 0) {
        // If pid is negative, I forking failed
        perror("fork");
        return;
    } 
    else if (pid == 0) {
        // Child process
        
        // Handle output redirection if specified with dup2
        if (!command.outputFile.empty()) {
            // Open the output file
            int fd = open(command.outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                //check for an open error
                perror("open");
                exit(1);
            }
            
            // Redirect stdout to the file
            if (dup2(fd, STDOUT_FILENO) < 0) {
                //check for a output error
                perror("dup2");
                close(fd);
                exit(1);
            }
            close(fd);
        }
        
        // Execute the command
        if (execvp(cargs[0], cargs.data()) < 0) {
            // If execvp fails, print error
            perror("execvp");
            exit(1);
        }
    } 
    else {
        // Parent process
        int status;
        
        // Wait for child process to complete
        if (waitpid(pid, &status, 0) < 0) {
            perror("waitpid");
            return;
        }
        
    }
}


// our old friend main(), this is entry for our shell program ;)
int main() {
    std::string input; // stores user input

    // our main loop: prompt for input, parse, and execute commands until "exit" is entered
    while (std::cout << "412shell> " && std::getline(std::cin, input) && input != "exit") {
        ParsedCommand command = customParseInput(input); // parse the user input into a command
        do_fork(command);
    }

    return 0;
}