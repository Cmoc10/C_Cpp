#include <stdio.h>
#include "fio.h"

// open an existing file for reading
FILE* openFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
    }
    return file;
}

// create a new file for writing
FILE* createFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error creating file");
    }
    return file;
}

// write a string to a file
int writeToFile(FILE* file, const char* content) {
    if (file == NULL) {
        fprintf(stderr, "Error: File pointer is NULL\n");
        return -1;
    }
    
    int result = fputs(content, file);
    if (result == EOF) {
        perror("Error writing to file");
        return -1;
    }
    return 0;
}
