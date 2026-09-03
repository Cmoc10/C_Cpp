#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Function to print the hexadecimal representation of the buffer
void printHex(char* buffer, long fsize, uint8_t rowSize) {
    int i;
    for (i = 0; i < fsize; i+= rowSize) {
        if (i % rowSize == 0 && i != 0) {
            printf("\n");
        }
        printf("%08x ", i); // Display index as an 8-digit hexadecimal number padded with zeros
        int j;
        for (j = 0; j < rowSize && i + j < fsize; j++) {
            printf("%02x ", (unsigned char) buffer[i + j]);
        }
    }
    printf("\n");
}

// Function to print the decimal representation of the buffer
void printDec(char* buffer, long fsize, uint8_t rowSize){
    int i;
    for(i = 0; i < fsize; i+= rowSize){
        if(i % rowSize == 0 && i != 0){
            printf("\n");
        }
        printf("%08x ", i); // Display index as an 8-digit hexadecimal number padded with zeros
        int j;
        for(j = 0; j < rowSize && i + j < fsize; j++){
            printf("%u ", (unsigned char) buffer[i + j]);
        }
    }
    printf("\n");
}

int main(int argc, char** argv){
    FILE* file;
    char* buffer;
    long fsize;
    size_t result;
    
    // Open the file in binary mode
    file = fopen(argv[1], "rb");
    if(file == NULL){
        printf("Error: File not found\n");
        fclose(file);
        return 1;
    }

    // Get the size of the file
    fseek(file, 0, SEEK_END);
    fsize = ftell(file);
    rewind(file);

    // Allocate memory for the buffer
    buffer = (char*) malloc(fsize * sizeof(char));
    if(buffer == NULL){
        printf("Error: Invalid argument\n");
        fclose(file);
        free(buffer);
        return 1;
    }

    // Read the file into the buffer
    result = fread(buffer, 1, fsize, file);
    if(result != fsize){
        printf("Error: Invalid argument\n");
        fclose(file);
        free(buffer);
        return 1;
    }

    // Check the command line arguments for row size and format
    if(argv[2] != NULL){
        int num = atoi(argv[2]);
        if(argv[3] != NULL){
            if(strcmp(argv[3], "d") == 0){
                // Print the decimal representation of the buffer
                printDec(buffer, fsize, num);
            }
            else if(strcmp(argv[3], "x") == 0){
                // Print the hexadecimal representation of the buffer
                printHex(buffer, fsize, num);
            }
            else{
                printf("Error: Invalid argument\n");
                fclose(file);
                free(buffer);
                return 1;
            }
        }
        else{
            printf("Error: Invalid argument\n");
            fclose(file);
            free(buffer);
            return 1;
        }
    }

    // Close the file and free the buffer
    fclose(file);
    free(buffer);
    
    return 0;
    //Github copilot helped me with the printHex and printDec functions
}