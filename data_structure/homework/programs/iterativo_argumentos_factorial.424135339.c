//REYES GONZALEZ DIEGO DE JESUS 1301 ESTRUCTURA DE DATOS TAREA 2 ARREGLOS EN EL FACTORIAL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
void runFactorialInteractive() {
    int number;

    printf("Enter a positive number:");
    scanf("%d", &number);

    if (number < 0) {
        printf("Factorial is not defined for negative numbers.\n");
    } else {
        int result = factIterative(number);
        printf("Factorial of %d is %d\n", number, result);
    }
}


void runArgPrinter(int argc, char *argv[]) {
    int i;
    char *num = argv[1];

    fprintf(stdout, "argv[1] = %s \n", argv[1]);

    for (i = 0; i < strlen(argv[1]); i++)
        fprintf(stdout, "num[%d] = %c \n", i, num[i]);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        runArgPrinter(argc, argv); 
    } else {
        runFactorialInteractive();
    }

    return 0;
}
