#ifndef FIO_H
#define FIO_H

#include <stdio.h>

// =open an existing file for reading
FILE* openFile(const char* filename);

// create a new file for writing
FILE* createFile(const char* filename);

// write a string to a file
int writeToFile(FILE* file, const char* content);

#endif /* FIO_H */