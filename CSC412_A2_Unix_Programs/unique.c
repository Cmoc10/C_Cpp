#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//makes the strings only have the lowercase characters and not any weird ones so strcmp can work
void fix_strings(char* str){
    char* temp = (char*)malloc(strlen(str) * sizeof(char));
    for(int i=0; i<strlen(str); i++){
        //If the string is within alphabetically characters
        if(str[i] > 96 && str[i] < 123){
            temp[i] = str[i];
        }
    }
    memcpy(str, temp, strlen(str));
    return;
}

int main() {
    // creates a 1024 byte buffer to store data, will this be enough memory to store data?
    char buffer[1024];
    char last[1024];
    int count = 0;
    // Read data from stdin and write it to stdout (standard output)
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        //pass data from the buffer to a function to process the data
        fix_strings(buffer);
        if(count == 0 || strcmp(buffer, last)){
            memcpy(last, buffer, 1024);
            fix_strings(buffer);
            printf("%s\n", buffer);
            count++;
        }
    }
    return 0;
}