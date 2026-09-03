#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//merge sort implementation
void merge(char** arr, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
   //dynamically allocate memore to left and right arrays
    char** L = (char**)malloc(n1 * sizeof(char*));
    char** R = (char**)malloc(n2 * sizeof(char*));
   
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
   
    i = 0;
    j = 0;
    k = left;
   
    while (i < n1 && j < n2) {
        if (strcmp(L[i], R[j]) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
   
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
   
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
   
    free(L);
    free(R);
}
//mergesort main function
void mergeSort(char** arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
       
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
       
        merge(arr, left, mid, right);
    }
}

void sort(char** arr, int n) {
    mergeSort(arr, 0, n - 1);
}
int main() {
    // creates a 1024 byte buffer to store data, will this be enough memory to store data?
    char buffer[2048];
    char* strings[2048];
    int count = 0;
    // Read data from stdin and write it to stdout (standard output)
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // pass data from the buffer to a function to process the data
        //remove newline character
        buffer[strcspn(buffer, "\n")] = 0;
        strings[count] = strdup(buffer);
        count++;
    }
    sort(strings, count);
    for(int i=0; i<count; i++){
        printf("%s\n", strings[i]);
        free(strings[i]);
    }
    return 0;
}
