#include "../fio/fio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100
#define MAX_WORDS 3005

void fix_strings(char* str){
    char* temp = (char*)malloc(strlen(str) * sizeof(char));
    for(int i=0; i<strlen(str); i++){
        if(str[i] > 96 && str[i] < 123){
            temp[i] = str[i];
        }
    }
    memcpy(str, temp, strlen(str));
    return;
}

//This took a long time to figure out but basically it stores all of the words from the file in an array
//and then checks them against the input if they are not in there it prints it
int main() {
    char buffer[1024];
    const char *filename = "unix_dict.text";
    
    FILE *file = openFile(filename);
    if (file == NULL) {
        return 1;
    }
    
    // Read all lines from the file into memory
    char words[MAX_WORDS][1024];
    int word_count = 0;
    
    //fixing the strings as I said in my review
    while (fgets(words[word_count], sizeof(words[word_count]), file) != NULL && word_count < MAX_WORDS) {
        fix_strings(words[word_count]);
        word_count++;
    }
    fclose(file);
    // Process input from stdin
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        fix_strings(buffer);
        int found = 0;
        for (int i = 0; i < word_count; i++) {
            //if a match is found break and go to next word
            if (strcmp(buffer, words[i]) == 0) {
                found = 1;
                break;
            }
        }
        // if no match is found print the word
        if (!found) {
            printf("%s\n", buffer);
        }
    }

    return 0;
}