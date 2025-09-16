/*Arreglos bidimensionales
La declaración de un arreglo bidimensional en lenguaje C se realiza de la siguiente forma.

[data type] variable [dimension1][dimension2];
por ejemplo: Inicialización y uso de un arreglo bidimensional dinámico

int rows = 2;  
int columns = 5;  
int **matrix = (int **) malloc(rows * sizeof(int *));  

for (int i = 0; i < rows; i++) {  
	matrix[i] = (int *) malloc(columns * sizeof(int));  
}
Programa
Genere un arreglo dinámico variable bidimensional de acuerdo con la palabra y la letra del abecedario este será rellenado de la siguiente forma.

El usuario ingresará una cadena, por ejemlo COMPUTACION. y esta será segmentada de la siguiente manera

./ P2E_arreglo_variable.exe COMPUTACION.
| C | C | C | 
| O | O | O | O | O | O | O | O | O | O | O | O | O | O | O |
| M | M | M | M | M | M | M | M | M | M | M | M | M |
| P | P | P | P | P | P | P | P | P | P | P | P | P | P | P | P |
| U | U | U | U | U | U | U | U | U | U | U | U | U | U | U | U | U | U | U | U | U |
| T | T | T | T | T | T | T | T | T | T | T | T | T | T | T | T | T | T | T | T |	
| A |
| C | C | C |
| I | I | I | I | I | I | I | I | I |
| O | O | O | O | O | O | O | O | O | O | O | O | O | O | O |
| N | N | N | N | N | N | N | N | N | N | N | N |
NULL
El programa deberá ejecutarse con la cadena, como se ve en el ejemplo previo
El programa solo deberá aceptar letras mayúsculas, cualquier otra letra, dígito o caracter deberá ser marcado como null.
Considere el programa read_dynamic_strings.c que se encuentra en la carpeta de tips and tricks para la contrucción de este programa.
nota: en los ejemplos previos no se ha considerado el caso en que no sea posible asignar el espacio necesario de forma dinámica

if (ptr == NULL)
	return -1;
	
424135339 REYES GONZALEZ DIEGO DE JESUS 14/09/25 
NOTA TODOS LOS PROGRAMAS DEBEN ESTAR ESCRITOS EN INGLÉS Y USAR ARGUMENTOS SI SE REQUIERE EN LAS INSTRUCCIONES.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int i, j, position, k;
    char **jarray;                /* Changed to char** to store characters */
    char *word;
    char current_char;
    
    /* Check if an argument is provided */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <UPPERCASE_WORD>\n", argv[0]);
        return -1;
    }
    
    word = argv[1];
    fprintf(stdout, "arg[1] = %s \n", word);
    
    /* Allocate memory for the main pointer array */
    jarray = (char **) malloc(sizeof(char *) * strlen(word));
    if (jarray == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for main array\n");
        return -1;
    }
    
    /* Process each character in the word */
    for (i = 0; i < strlen(word); i++) {
        current_char = word[i];
        
        /* Check if it's an uppercase letter (A-Z) */
        if (current_char >= 'A' && current_char <= 'Z') {
            /* Calculate position in alphabet (A=1, B=2, ..., Z=26) */
            position = current_char - 'A' + 1;
            
            fprintf(stdout, "| ");
        
            jarray[i] = (char *) malloc(sizeof(char) * position);
            if (jarray[i] == NULL) {
                fprintf(stderr, "Error: Could not allocate memory for row %d\n", i);
                
              
                for (k = 0; k < i; k++) {
                    if (jarray[k] != NULL) {
                        free(jarray[k]);
                    }
                }
                free(jarray);
                return -1;
            }
            
            /* Fill the row with repeated character */
            for (j = 0; j < position; j++) {
                jarray[i][j] = current_char;
                fprintf(stdout, "%c | ", jarray[i][j]);
            }
            fprintf(stdout, "\n");
            
        } else {
            /* If not an uppercase letter, mark as NULL */
            jarray[i] = NULL;
            fprintf(stdout, "NULL\n");
        }
    }
    
    /* Free all allocated memory */
    for (i = 0; i < strlen(word); i++) {
        if (jarray[i] != NULL) {
            free(jarray[i]);
        }
    }
    free(jarray);
    
    return 0;
}
