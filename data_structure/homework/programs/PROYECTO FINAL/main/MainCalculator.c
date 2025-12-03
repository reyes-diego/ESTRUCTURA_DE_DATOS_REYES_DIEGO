#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

/* Funciones para ejecutar cada archivo .c */
void run_and_log(const char *command, const char *outfile) {
    char full_cmd[256];

    /* Redirección a archivo de texto */
    snprintf(full_cmd, sizeof(full_cmd), "%s > %s", command, outfile);

    printf("\nEjecutando: %s\n", full_cmd);
    system(full_cmd);

    printf("Salida guardada en: %s\n\n", outfile);
}

int main() {
    int opcion;

    do {
        system(CLEAR);
        printf("======================================\n");
        printf("         CALCULADORA GENERAL\n");
        printf("======================================\n");
        printf("1) PRE-NUM\n");
        printf("2) POST-NUM\n");
        printf("3) PRE-LETTERS\n");
        printf("4) POSTFIX LETTERS\n");
        printf("5) INFIX\n");
        printf("0) Salir\n");
        printf("======================================\n");
        printf("Selecciona una opción: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                run_and_log("./PRE-NUM", "PRE-NUM_output.txt");
                break;

            case 2:
                run_and_log("./POST-NUM", "POST-NUM_output.txt");
                break;

            case 3:
                run_and_log("./PRE-LETTERS", "PRE-LETTERS_output.txt");
                break;

            case 4:
                run_and_log("./POSTFIX-LETTERS", "POSTFIX-LETTERS_output.txt");
                break;

            case 5:
                run_and_log("./Infix", "Infix_output.txt");
                break;

            case 0:
                printf("Saliendo...\n");
                break;

            default:
                printf("Opción inválida.\n");
        }

        if (opcion != 0) {
            printf("Presiona ENTER para continuar...");
            getchar();
            getchar();
        }

    } while (opcion != 0);

    return 0;
}