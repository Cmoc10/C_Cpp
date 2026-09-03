#include "../fio/fio.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    char filename[] ="unix_sentence.text"; 
    char buffer[1024]; // buffer to store data

    // open the file using the local fio library (Hint: see line 1)
    file = openFile(filename);
    // read and print the contents of the file
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // put file contents (buffer) into stdout (Hint: look at linebreaker)
        fputs(buffer, stdout);
    }
    // Close the file
    fclose(file);

    return 0;
}