#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variable to count steps
int step = 0;

// Function to print the array
void printArray(int arr[], int start, int end, char* message) {
    int i;
    printf("%s [", message);
    for (i = start; i <= end; i++) {
        printf("%d", arr[i]);
        if (i < end) printf(", ");
    }
    printf("]\n");
}

// Function to merge two subarrays
void merge(int arr[], int left, int middle, int right) {
    int n1 = middle - left + 1;
    int n2 = right - middle;
    int i, j, k;
    
    // Create temporary arrays
    int *L = (int*)malloc(n1 * sizeof(int));
    int *R = (int*)malloc(n2 * sizeof(int));
    
    // Copy data to temporary arrays
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[middle + 1 + j];
    
    printf("\n--- STEP %d: MERGING ---\n", ++step);
    printf("Left subarray: ");
    for (i = 0; i < n1; i++) {
        printf("%d", L[i]);
        if (i < n1-1) printf(", ");
    }
    printf("\nRight subarray: ");
    for (i = 0; i < n2; i++) {
        printf("%d", R[i]);
        if (i < n2-1) printf(", ");
    }
    printf("\n");
    
    // Merge the temporary arrays
    i = 0;
    j = 0;
    k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    // Copy remaining elements
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
    
    printf("Merge result: ");
    printArray(arr, left, right, "");
    
    free(L);
    free(R);
}

// Main Merge Sort function
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        
        printf("\n=== DIVIDING: ");
        printArray(arr, left, right, "");
        printf("    Left half: indices %d to %d\n", left, middle);
        printf("    Right half: indices %d to %d\n", middle + 1, right);
        
        // Sort first and second half
        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);
        
        // Merge the sorted halves
        merge(arr, left, middle, right);
    }
}

int main(int argc, char *argv[]) {
    int n, i, capacity;
    int *arr;
    char line[1000];
    char *token;
    char *num;
    
    printf("===============================================\n");
    printf("  MERGE SORT ALGORITHM - STEP BY STEP\n");
    printf("===============================================\n\n");
    
    // Code to handle arguments (as requested)
    if (argc > 1) {
        num = argv[1];
        fprintf(stdout, "argv[1] = %s \n", argv[1]);
        for (i = 0; i < strlen(argv[1]); i++)
            fprintf(stdout, "num[%d] = %c \n", i, num[i]);
        printf("\n");
        
        // Use the argument as input
        strncpy(line, argv[1], sizeof(line) - 1);
        line[sizeof(line) - 1] = '\0';
    } else {
        printf("Enter numbers separated by spaces or commas:\n");
        fgets(line, sizeof(line), stdin);
    }
    
    capacity = 10;
    arr = (int*)malloc(capacity * sizeof(int));
    n = 0;
    
    token = strtok(line, " ,\n");
    
    while (token != NULL) {
        if (n >= capacity) {
            capacity *= 2;
            arr = (int*)realloc(arr, capacity * sizeof(int));
        }
        
        arr[n] = atoi(token);
        n++;
        
        token = strtok(NULL, " ,\n");
    }
    
    if (n == 0) {
        printf("Error: You must enter at least one number.\n");
        free(arr);
        return 1;
    }
    
    printf("\n===============================================\n");
    printf("ORIGINAL ARRAY: ");
    printArray(arr, 0, n-1, "");
    printf("===============================================\n");
    
    /* Apply Merge Sort */
    mergeSort(arr, 0, n - 1);
    
    printf("\n===============================================\n");
    printf("FINAL SORTED ARRAY: ");
    printArray(arr, 0, n-1, "");
    printf("===============================================\n");
    printf("\nTotal merge steps: %d\n", step);
    
    free(arr);
    return 0;
}
