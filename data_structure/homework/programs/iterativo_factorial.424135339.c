//REYES GONZALEZ DIEGO DE JESUS 1301 ESTRUCTURA DE DATOS TAREA 2  FACTORIAL ITERATIVO 

#include <stdio.h>

// Iterative function to calculate factorial
int factIterative(int n) {
    if (n < 0)
        return 0; 

    int result = 1;
    int i;
    for (i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    int number;

  
    printf("Enter a positive number:");
    scanf("%d", &number);


    if (number < 0) {
        printf("Factorial is not defined for negative numbers.\n");
    } else {
        int result = factIterative(number);
        printf("Factorial of %d is %d\n", number, result);
    }

    return 0;
}
