#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printArray(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("\n");
}

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && arr[left] > arr[largest])
        largest = left;
    
    if (right < n && arr[right] > arr[largest])
        largest = right;
    
    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        
        printf("   Swap: %d <-> %d: ", arr[largest], arr[i]);
        printArray(arr, n);
        
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    int i, j, k;
    
    printf("\n=== BUILDING MAX HEAP ===\n");
    for (i = n / 2 - 1; i >= 0; i--) {
        printf("\nHeapify from index %d:\n", i);
        heapify(arr, n, i);
    }
    
    printf("\n=== MAX HEAP BUILT ===\n");
    printArray(arr, n);
    
    printf("\n=== SORTING ===\n");
    for (i = n - 1; i > 0; i--) {
        printf("\nStep %d:\n", n - i);
        printf("   Move root %d to end (position %d)\n", arr[0], i);
        
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        
        printf("   Current array: ");
        printArray(arr, n);
        
        printf("   Restoring heap (size %d):\n", i);
        heapify(arr, i, 0);
    }
}

int main(int argc, char *argv[]) {
    int *arr = NULL;
    int n = 0;
    int i, j, k;
    
    printf("=== HEAP SORT ===\n");
    
    // Check if arguments were provided
    if (argc < 2) {
        printf("Usage: %s <number1,number2,number3,...>\n", argv[0]);
        printf("Example: %s 12,11,13,5,6,7\n", argv[0]);
        printf("Or: %s 12 11 13 5 6 7\n\n", argv[0]);
        return 1;
    }
    
    // Check if numbers are comma-separated (single argument) or space-separated (multiple arguments)
    if (argc == 2 && strchr(argv[1], ',') != NULL) {
        // Comma-separated format: program "12,11,13,5,6,7"
        char *input = argv[1];
        int capacity = 10;
        
        arr = (int*)malloc(capacity * sizeof(int));
        if (arr == NULL) {
            printf("Memory allocation error.\n");
            return 1;
        }
        
        char *token = strtok(input, ",");
        while (token != NULL) {
            if (n >= capacity) {
                capacity *= 2;
                int *temp = (int*)realloc(arr, capacity * sizeof(int));
                if (temp == NULL) {
                    printf("Memory allocation error.\n");
                    free(arr);
                    return 1;
                }
                arr = temp;
            }
            
            arr[n] = atoi(token);
            n++;
            token = strtok(NULL, ",");
        }
    } else {
        // Space-separated format: program 12 11 13 5 6 7
        n = argc - 1;
        
        arr = (int*)malloc(n * sizeof(int));
        if (arr == NULL) {
            printf("Memory allocation error.\n");
            return 1;
        }
        
        for (i = 0; i < n; i++) {
            arr[i] = atoi(argv[i + 1]);
        }
    }
    
    if (n == 0) {
        printf("No valid numbers entered.\n");
        free(arr);
        return 1;
    }
    
    printf("\nOriginal array (%d elements): ", n);
    printArray(arr, n);
    
    heapSort(arr, n);
    
    printf("\n=== FINAL RESULT ===\n");
    printf("Sorted array: ");
    for (i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("\n");
    
    free(arr);
    
    return 0;
}
