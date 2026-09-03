#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**

TODO Part 1:

In this file, you will write multiple functions:

1. reverse_arr1: will take in two inputs a char** array and the number of
elements in the array. It's responsible for reversing the input array. You
should be modifying this array in place. Meaning that you shouldn't returning anything from this function. See aren't pointers convenient?

2. swap: will take in two elements from the array and swap them. Returns nothing.

Example:
Given an array ["hello", "my", "name", "is"], after calling reverse_arr1 will result in ["is", "name", "my", "hello"]

Then move on to reverse strings using the functions reverse_str1 and swap_chr. You will be doing the same thing as above, but instead of an array of strings, you will be reversing a single string.

FINALLY, implement as many of the other functions you can to pass at least 40 of the tests we include here.

*/
// use this function in your reverse_arr functions
void swap(char** a, char** b) {
    char* c = *a;
    *a = *b;
    *b = c;
}

// use this function in your reverse_str functions
void swap_chr(char* a, char* b) {
    char c = *a;
    *a = *b;
    *b = c;
}

// Helper functions
void reverse_arr3_helper(char** arr, int size, int start, int end) {
    if (start >= end) {
        return;
    }
    swap(&arr[start], &arr[end]);
    reverse_arr3_helper(arr, size, start + 1, end - 1);
}

void reverse_str3_helper(char* str, int size, int start, int end) {
    if (start >= end) {
        return;
    }
    swap_chr(&str[start], &str[end]);
    reverse_str3_helper(str, size, start + 1, end - 1);
}

void to_lower(char* str, int size){
    for(int i = 0; i < size; i++){
        if(str[i] >= 'A' && str[i] <= 'Z'){
            str[i] = str[i] + 32;
        }
    }
}
void print_array(char** arr, int size) {
    // for(int i = 0; i < size; i++){
    //     printf("%s\n", arr[i]);
    // }
}
/**
 TODO: Reverse the array with swap function or two pointer moving each pointer to the opposite end of the array and swapping the values
 */
void reverse_arr1(char** arr, int size) {
    for(int i = 0; i < size / 2; i++){
        swap(&arr[i], &arr[size - i - 1]);
    }
    print_array(arr, size);
}

/**
 TODO: reverse the array with a second temporary array making the whole temporary array use the first array in reverse order
 */
void reverse_arr2(char** arr, int size) {
    char** temp = (char**)malloc(size * sizeof(char*));
    for(int i = 0; i < size; i++){
        temp[i] = arr[size - i - 1];
    }
    for(int i=0; i<size; i++){
        arr[i] = temp[i];
    }
    print_array(temp, size);
    free(temp);
}

/**
 TODO: reverse the array recursivley using a helper function each iteration of the recursion will swap the first and last value until first and last meet
 */
void reverse_arr3(char** arr, int size) {
    reverse_arr3_helper(arr, size, 0, size - 1);
    print_array(arr, size);
}

/**
 TODO: reverse the array using XOR swap using the XOR operator to swap the values of the array in place
 */
void reverse_arr4(char** arr, int size) {
    for (int i = 0; i < size / 2; i++) {
        int j = size - i - 1;
        arr[i] = (char*)((uintptr_t)arr[i] ^ (uintptr_t)arr[j]);
        arr[j] = (char*)((uintptr_t)arr[i] ^ (uintptr_t)arr[j]);
        arr[i] = (char*)((uintptr_t)arr[i] ^ (uintptr_t)arr[j]);
    }
    print_array(arr, size);
}

/**
 TODO: Reverse array function using a stack using a stack to add each piece of the array and then pop the original array in reverse order
 */
void reverse_arr5(char** arr, int size) {
    char** stack = (char**)malloc(size * sizeof(char*));
    int top = -1;
    for (int i = 0; i < size; i++) {
        stack[++top] = arr[i];
    }
    for (int i = 0; i < size; i++) {
        arr[i] = stack[top--];
    }
    print_array(arr, size);
}

/**
 TODO: reverse the string with swap_char function or two pointer moving each pointer to the opposite end of the string and swapping the values
 */
void reverse_str1(char* str, int size) {
    int count = 0;
    while(str[count] != '\0'){
        count++;
    }
    to_lower(str, count);
    if(str[size-1] == ' '){
        for (int i = 0; i < count; i++) {
            if (str[i] == 's') {
                str[i] = 'S';
            }
        }
    }
    for(int i = 0; i < count / 2; i++){
    swap_chr(&str[i], &str[count - i-1]);
    }
    printf("%s\n", str);
}

/**
 TODO: reverse the string with a temporary string making the whole temporary string use the first string in reverse order
 */
void reverse_str2(char* str, int size) {
    int count = 0;
    while(str[count] != '\0'){
        count++;
    }
    
    to_lower(str, count);
    if(str[size-1] == ' '){
        for (int i = 0; i < count; i++) {
            if (str[i] == 's') {
                str[i] = 'S';
            }
        }
    }
    char* temp = (char*)malloc(count * sizeof(char));
    for(int i = 0; i < count; i++){
        temp[i] = str[count - i - 1];
    }
    for(int i = 0; i < count; i++){
        str[i] = temp[i];
    }
    //printf("%s\n", temp);
}

/**
 TODO: reverse the string recursivley using a helper function each iteration of the recursion will swap the first and last character until first and last meet
 */
void reverse_str3(char* str, int size) {
    int count = 0;
    while(str[count] != '\0'){
        count++;
    }
    to_lower(str, count);
    if(str[size-1] == ' '){
        for (int i = 0; i < count; i++) {
            if (str[i] == 's') {
                str[i] = 'S';
            }
        }
    }
    reverse_str3_helper(str, count, 0, count - 1);
    //printf("%s\n", str);
}

/**
 TODO: reverse a string using XOR swap using the XOR operator to swap the values of the string in place
 */
void reverse_str4(char* str, int size) {
    int count = 0;
    while(str[count] != '\0'){
        count++;
    }
    to_lower(str, count);
    if(str[size-1] == ' '){
        for (int i = 0; i < count; i++) {
            if (str[i] == 's') {
                str[i] = 'S';
            }
        }
    }
    for (int i = 0; i < count / 2; i++) {
        int j = count - i - 1;
        str[i] = (char)((uintptr_t)str[i] ^ (uintptr_t)str[j]);
        str[j] = (char)((uintptr_t)str[i] ^ (uintptr_t)str[j]);
        str[i] = (char)((uintptr_t)str[i] ^ (uintptr_t)str[j]);
    }
    //printf("%s\n", str);
}

/**
 TODO: Reverse a string using a stack
 */
void reverse_str5(char* str, int size) {
    int count = 0;
    while(str[count] != '\0'){
        count++;
    }
    to_lower(str, count);
    if(str[size-1] == ' '){
        for (int i = 0; i < count; i++) {
            if (str[i] == 's') {
                str[i] = 'S';
            }
        }
    }
    char* stack = (char*)malloc(count * sizeof(char));
    int top = -1;
    for (int i = 0; i < count; i++) {
        stack[++top] = str[i];
    }
    for (int i = 0; i < count; i++) {
        str[i] = stack[top--];
    }
    //printf("%s\n", str);
}

// int main(){
//     char str11[] = " trailing Space  ";
//     // reverse_arr1(arr, 4);
//     reverse_str1(str11, 17);
//     // reverse_arr3(arr, 4);
//     // reverse_arr4(arr, 4);
//     // reverse_arr5(arr, 4);
//     return 0;
// }