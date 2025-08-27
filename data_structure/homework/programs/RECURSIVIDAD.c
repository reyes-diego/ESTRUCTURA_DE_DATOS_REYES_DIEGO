#include <stdio.h>


int factRecursion(int n) {
    if (n < 0)
        return 0; 
    else if (n == 0 || n == 1)
        return 1; 
    else
        return n * factRecursion(n - 1); // Recursive call
}

int main() {
    int number;

    printf("Enter a positive number: ");
    scanf("%d", &number);


    if (number < 0) {
        printf("Factorial is not defined for negative numbers.\n");
    } else {
        int result = factRecursion(number);
        printf("Factorial of %d is %d\n", number, result);
    }

    return 0;
}
