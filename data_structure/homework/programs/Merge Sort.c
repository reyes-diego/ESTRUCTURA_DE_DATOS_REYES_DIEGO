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

void printSubArray(int arr[], int left, int right) {
    int i;
    printf("[");
    for (i = left; i <= right; i++) {
        printf("%d", arr[i]);
        if (i < right) printf(", ");
    }
    printf("]");
}

void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // Create temporary arrays
    int *L = (int*)malloc(n1 * sizeof(int));
    int *R = (int*)malloc(n2 * sizeof(int));
    
    if (L == NULL || R == NULL) {
        printf("Memory allocation error.\n");
        return;
    }
    
    // Copy data to temporary arrays
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    printf("   Merging: ");
    printSubArray(arr, left, mid);
    printf(" and ");
    printSubArray(arr, mid + 1, right);
    printf("\n");
    
    // Merge the temporary arrays back
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
    
    // Copy remaining elements of L[]
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    // Copy remaining elements of R[]
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    printf("   Result: ");
    printSubArray(arr, left, right);
    printf("\n\n");
    
    free(L);
    free(R);
}

void mergeSortRecursive(int arr[], int left, int right, int *step) {
    int i, j, k;
    
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        printf("Step %d: Dividing ", *step);
        printSubArray(arr, left, right);
        printf(" into ");
        printSubArray(arr, left, mid);
        printf(" and ");
        printSubArray(arr, mid + 1, right);
        printf("\n");
        (*step)++;
        
        // Sort first half
        mergeSortRecursive(arr, left, mid, step);
        
        // Sort second half
        mergeSortRecursive(arr, mid + 1, right, step);
        
        // Merge the sorted halves
        printf("Step %d:\n", *step);
        (*step)++;
        merge(arr, left, mid, right);
    }
}

void mergeSort(int arr[], int n) {
    int step = 1;
    printf("\n=== MERGE SORT - DIVIDE AND CONQUER ===\n\n");
    mergeSortRecursive(arr, 0, n - 1, &step);
}

int main(int argc, char *argv[]) {
    int *arr = NULL;
    int n = 0;
    int i, j, k;
    
    printf("=== MERGE SORT ===\n");
    
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
    
    mergeSort(arr, n);
    
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
