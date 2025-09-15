/*Programa: P2E - Recorrido de matriz bidimensional


Realice el programa que dada una matriz de m*n, en donde el usuario introducirá el valor de m y n, 
siendo estos máximos 10, el arreglo es del tipo entero. Realice el siguiente recorrido:

1. Recorrido por fila comenzando desde la casilla 00, siguiente 01, 02, 03... luego 10, 11... hasta mn.

2. Recorrido por columnas comenzando desde mn , n-1mm ,n-2m , hasta 0m, para continuar con 
   nm-1,n-1m-1,n-2m...0m-1.. el proceso deberá continuar hasta llegar a la casilla 00

3. Realice el recorrido en diagonal desde la casilla 00, 11, 22, 33, siempre que n = m.

//424135339 REYES GONZALEZ DIEGO DE JESUS 10/09/25 

//NOTA TODOS LOS PROGRAMAS DEBEN ESTAR ESCRITOS EN INGLÉS Y USAR ARGUMENTOS SI SE REQUIERE EN LAS INSTRUCCIONES.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Variables declared outside the for loops
    int n, m;
    int i, j;
    int continuar;
    int **matriz;
    int valor;
    char *num;
    
    // Arguments code to execute using cmd
    if (argc > 1) {
        num = argv[1];
        fprintf(stdout, "argv[1] = %s \n", argv[1]);
        for (i = 0; i < strlen(argv[1]); i++)
            fprintf(stdout, "num[%d] = %c \n", i, num[i]);
        printf("\n");
    }
    
    printf("=== Program P2E - Bidimensional matrix traversal ===\n\n");
    
    do {
        // Request matrix dimensions
        printf("Enter the number of rows (n): ");
        scanf("%d", &n);
        printf("Enter the number of columns (m): ");
        scanf("%d", &m);
        
        // Validate that dimensions are valid
        if (n <= 0 || m <= 0 || n > 10 || m > 10) {
            printf("Error: Dimensions must be between 1 and 10.\n");
            continue;
        }
        
        // Dynamic memory allocation for the matrix
        matriz = (int**)malloc(n * sizeof(int*));
        for (i = 0; i < n; i++) {
            matriz[i] = (int*)malloc(m * sizeof(int));
        }
        
        // Fill the matrix with values
        printf("\nEnter the elements of the %dx%d matrix:\n", n, m);
        for (i = 0; i < n; i++) {
            for (j = 0; j < m; j++) {
                printf("Element [%d][%d]: ", i, j);
                scanf("%d", &matriz[i][j]);
            }
        }
        
        // Show original matrix
        printf("\nEntered matrix:\n");
        for (i = 0; i < n; i++) {
            for (j = 0; j < m; j++) {
                printf("%4d ", matriz[i][j]);
            }
            printf("\n");
        }
        
        printf("\n=== SHOWING THE 3 TRAVERSALS ===\n");
        
        // 1. Row traversal starting from cell 00
        printf("\n1. Row traversal (from [0,0] to [%d,%d]):\n", n-1, m-1);
        printf("Sequence: ");
        for (i = 0; i < n; i++) {
            for (j = 0; j < m; j++) {
                printf("%d ", matriz[i][j]);
            }
        }
        printf("\n");
        
        printf("Visited positions: ");
        for (i = 0; i < n; i++) {
            for (j = 0; j < m; j++) {
                printf("[%d,%d] ", i, j);
            }
        }
        printf("\n");
        
        // 2. Column traversal
        printf("\n2. Column traversal (from column 0 to column %d):\n", m-1);
        printf("Sequence: ");
        for (j = 0; j < m; j++) {
            for (i = 0; i < n; i++) {
                printf("%d ", matriz[i][j]);
            }
        }
        printf("\n");
        
        printf("Visited positions: ");
        for (j = 0; j < m; j++) {
            for (i = 0; i < n; i++) {
                printf("[%d,%d] ", i, j);
            }
        }
        printf("\n");
        
        // 3. Diagonal traversal
        printf("\n3. Diagonal traversal:\n");
        if (n == m) {
            // Main diagonal (when n = m)
            printf("Main diagonal: ");
            for (i = 0; i < n; i++) {
                printf("%d ", matriz[i][i]);
            }
            printf("\n");
            
            printf("Visited positions: ");
            for (i = 0; i < n; i++) {
                printf("[%d,%d] ", i, i);
            }
            printf("\n");
        } else {
            printf("For complete diagonal traversal, the matrix must be square (n = m).\n");
            printf("Partial diagonal traversal (elements where i = j and i < min(%d,%d)):\n", n, m);
            
            int min_dim = (n < m) ? n : m;
            printf("Diagonal: ");
            for (i = 0; i < min_dim; i++) {
                printf("%d ", matriz[i][i]);
            }
            printf("\n");
            
            printf("Visited positions: ");
            for (i = 0; i < min_dim; i++) {
                printf("[%d,%d] ", i, i);
            }
            printf("\n");
        }
        
        // Free memory
        for (i = 0; i < n; i++) {
            free(matriz[i]);
        }
        free(matriz);
        
        printf("\nDo you want to perform another traversal? (1 = Yes, 0 = No): ");
        scanf("%d", &continuar);
        printf("\n");
        
    } while (continuar == 1);
    
    printf("Program terminated. Thank you for using the P2E program!\n");
    
    return 0;
}
