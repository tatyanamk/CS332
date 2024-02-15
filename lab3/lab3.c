
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to read strings into the array.
void stringReader(char ***arr, int N) {
    int i;

    // Allocated memory for an array of pointers.
    *arr = (char **)malloc(N * sizeof(char *));

    // Prompts user to enter the number of strings and allocates memory for each string.
    for (i = 0; i < N; i++) {
        printf("Enter string %d: ", (i + 1));
        char size[50];  
        scanf("%s", size);

        // Allocated memory for the strings.
        (*arr)[i] = (char *)malloc((strlen(size) + 1) * sizeof(char));
        strcpy((*arr)[i], size);
    }
}

// Function to perform insertion sort on the strings.
void insertionSortStr(char **arr, int N) {
    int i, currentLoc;
    char *size;

    for (i = 1; i < N; i++) {
        currentLoc = i;
        while (currentLoc > 0 && strcmp(arr[currentLoc - 1], arr[currentLoc]) > 0) {
            // Switches the strings if they're arranged wrong.
            size = arr[currentLoc];
            arr[currentLoc] = arr[currentLoc - 1];
            arr[currentLoc - 1] = size;
            currentLoc--;
        }
    }
}

// Function to display the array of strings
void displayStrs(char **arr, int N) {
    printf("Sorted array: [");
    for (int i = 0; i < N - 1; i++) {
        printf("%s, ", arr[i]);
    }
    printf("%s]\n", arr[N - 1]);
}

// Deallocates the memory previously allocated for the strings.
void freeStrs(char **arr, int N) {
    for (int i = 0; i < N; i++) {
        free(arr[i]);
    }
    free(arr);
}

int main() {
    int N;

    printf("Number of strings: ");
    scanf("%d", &N);

    char **arr;

    stringReader(&arr, N);

    insertionSortStr(arr, N);

    displayStrs(arr, N);

    freeStrs(arr, N);

    return 0;
}
