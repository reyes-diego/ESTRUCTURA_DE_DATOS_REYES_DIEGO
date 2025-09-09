//REYES GONZALEZ DIEGO DE JESUS 1301 ESTRUCTURA DE DATOS TAREA 2 USO DE ARGUMENTOS EN EL FACTORIAL ITERATIVO

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Iterative function to calculate factorial using the F(n,a) formula
int factTail(int n, int a) {
    // Base case: if n is 0 or 1, return a
    while (n > 1) {
        // Apply iteration: F(n-1, n*a)
        a = n * a;  
        n = n - 1;  
    }
    // When n <= 1, return a
    return a;
}

int main(int argc, char *argv[]) {
    int i;
    int number;
    
   
    if (argc != 2) {
      
        printf("Enter a positive number to calculate factorial: ");
        scanf("%d", &number);
    } else {
       // use the original fragment code
        char *num = argv[1];
        fprintf(stdout, "argv[1] = %s \n", argv[1]);
        for (i = 0; i < strlen(argv[1]); i++)
            fprintf(stdout, "num[%d] = %c \n", i, num[i]);
        
        // Convert string argument to integer
        number = atoi(argv[1]);
    }
    
    if (number < 0) {
        printf("Factorial is not defined for negative numbers.\n");
        return 0;
    }
    
    // Call the function with a = 1 as initial value of accumulator
    int result = factTail(number, 1);
    
    printf("The factorial of %d is: %d\n", number, result);
    
    return 0;
}
