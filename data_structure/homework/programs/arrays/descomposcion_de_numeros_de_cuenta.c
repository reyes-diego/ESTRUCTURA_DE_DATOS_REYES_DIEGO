//Descomposición del numero de cuenta.
//Generando un arreglo estatico variable bidimensional 
//de acuerdo al numero de cuneta que ingrese el usuario 
// y será rellenado con los dígitos correspondientes a su numero de cuneta 
//por ejemplo si el numero de cuenta es 410592683:

//|4|4|4|4|
//|1|
//|
//|5|5|5|5|5|5|
//|9|9|9|9|9|9|9|9|9|
//|2|2|
//|6|6|6|6|6|6|
//|8|8|8|8|8|8|8|8|
//|3|3|3|

//EJERCICIO PRUEBA REYES GONZALEZ DIEGO DE JESUS 1301 ED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_DIGITS 20      // Máximo número de dígitos en la cuenta
#define MAX_REPEAT 9       

int main(int argc, char *argv[]) {
    // Verifica que se pase el argumento
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <account_number>\n", argv[0]);
        return 1;
    }

    char *cuenta = argv[1];
    int len = strlen(cuenta);

    int i,j,k;
    for (i = 0; i < len; i++) {
        if (!isdigit(cuenta[i])) {
            fprintf(stderr, "Error: Account number must contain only digits.\n");
            return 1;
        }
    }

    // Declarar arreglo bidimensional estático: cada fila tiene hasta 9 columnas
    int filas = len;
    char matriz[filas][MAX_REPEAT];  

    for (i = 0; i < filas; i++) {
        int digito = cuenta[i] - '0';  

        for (j = 0; j < digito; j++) {
            matriz[i][j] = cuenta[i];
        }
    }

    printf("\nAccount number decomposition:\n\n");
    for (i = 0; i < filas; i++) {
        int digito = cuenta[i] - '0';
        printf("|");
        for (j = 0; j < digito; j++) {
            printf("%c|", matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}
