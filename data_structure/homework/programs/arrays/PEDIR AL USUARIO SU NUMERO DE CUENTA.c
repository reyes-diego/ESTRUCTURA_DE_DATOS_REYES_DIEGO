//PARCTICA DESCOMPOSICION DEL NUMERO DE CUENTA PIDIENDO AL USUARIO SU NUMERO DE CUENTA.
//Programa en c que genere una lista de los primeros 10 numeros pares , 
//debera imprimirlos en orden inverso,posteriormente debera modificar el 
//valor almacenado en el indice correspondiente al ultimo digito de su nuumero de 
//cuenta por -1.
//REYES GONZALEZ DIEGO DE JESUS 01/09/25 

//NOTA TODOS LOS PROGRAMAS DEBEN ESTAR ESCRITOS EN INGLÉS.

#include <stdio.h>

int main() {
    int i;
    int account_number;
    int even_numbers[10];
    char continue_program;
    
    do {
        printf("=== ACCOUNT NUMBER BREAKDOWN ===\n");
        printf("Please enter your account number: ");
        scanf("%d", &account_number);
        
        printf("Account number: %d\n", account_number);
        
        int last_digit = account_number % 10;
        printf("Last digit of account number: %d\n\n", last_digit);
        
        printf("=== GENERATING FIRST 10 EVEN NUMBERS ===\n");
        printf("Even numbers created:\n");
        
        for(i = 0; i < 10; i++) {
            even_numbers[i] = (i + 1) * 2; 
            printf("Position %d: %d\n", i, even_numbers[i]);
        }
        
        printf("\n=== PRINTING IN REVERSE ORDER ===\n");
        printf("Even numbers in reverse order:\n");
        for(i = 9; i >= 0; i--) {
            printf("Position %d: %d\n", i, even_numbers[i]);
        }
        
        printf("\n=== CHANGE BASED ON LAST DIGIT ===\n");
        if(last_digit < 10) { 
            printf("Original value at position %d: %d\n", last_digit, even_numbers[last_digit]);
            even_numbers[last_digit] = -1;
            printf("New value at position %d: %d\n", last_digit, even_numbers[last_digit]);
        } else {
            printf("The last digit (%d) is not a valid position.\n", last_digit);
        }
        
        printf("\n=== FINAL ARRAY AFTER CHANGE ===\n");
        printf("Final state of the array:\n");
        for(i = 0; i < 10; i++) {
            printf("Position %d: %d", i, even_numbers[i]);
            if(i == last_digit && last_digit < 10) {
                printf(" <- CHANGED");
            }
            printf("\n");
        }
        
        printf("\nDo you want to enter another account number? (y/n): ");
        scanf(" %c", &continue_program);
        printf("\n");
        
    } while(continue_program == 'y' || continue_program == 'Y');
    
    printf("Program terminated. Thank you!\n");
    
    return 0;
}
