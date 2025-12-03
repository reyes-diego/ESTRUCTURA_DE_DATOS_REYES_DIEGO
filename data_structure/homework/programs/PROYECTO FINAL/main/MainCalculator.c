
//MAIN.C - Sistema de Calculadora de Expresiones
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Definiciones de Secuencias VT100 ---
#define RESET_COLOR "\033[0m"
#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_CYAN "\033[1;36m"

// --- Función para limpiar pantalla portable ---
void limpiar_pantalla() {
    printf("\033[2J\033[H");
    fflush(stdout);
}

// Prototipos de funciones de colores
void color_normal() { printf(RESET_COLOR); }
void color_verde() { printf(COLOR_GREEN); }
void color_azul() { printf(COLOR_BLUE); }
void color_amarillo() { printf(COLOR_YELLOW); }
void color_rojo() { printf(COLOR_RED); }
void color_cyan() { printf(COLOR_CYAN); }

// Función para mostrar el banner principal
void mostrar_banner() {
    color_cyan();
    printf("\n");
    printf("  ================================================================================\n");
    printf("  ||                                                                            ||\n");
    printf("  ||                   EXPRESSION CALCULATOR SYSTEM                             ||\n");
    printf("  ||              Infix, Prefix and Postfix Conversions                         ||\n");
    printf("  ||                                                                            ||\n");
    printf("  ================================================================================\n");
    color_normal();
}

// Función para mostrar el menú principal
void mostrar_menu() {
    color_verde();
    printf("\n  ================================================================================\n");
    printf("  ||                            MAIN MENU                                       ||\n");
    printf("  ================================================================================\n");
    color_normal();
    
    printf("  ||                                                                            ||\n");
    printf("  ||  ");
    color_amarillo();
    printf("[1]");
    color_normal();
    printf(" PREFIX with LETTERS    - Convert infix to prefix (symbolic)           ||\n");
    
    printf("  ||  ");
    color_amarillo();
    printf("[2]");
    color_normal();
    printf(" INFIX EVALUATOR        - Evaluate infix expressions with numbers      ||\n");
    
    printf("  ||  ");
    color_amarillo();
    printf("[3]");
    color_normal();
    printf(" POSTFIX with LETTERS   - Convert infix to postfix (symbolic)          ||\n");
    
    printf("  ||  ");
    color_amarillo();
    printf("[4]");
    color_normal();
    printf(" PREFIX with NUMBERS    - Convert infix to prefix with evaluation      ||\n");
    
    printf("  ||  ");
    color_amarillo();
    printf("[5]");
    color_normal();
    printf(" POSTFIX with NUMBERS   - Convert infix to postfix with evaluation     ||\n");
    
    printf("  ||                                                                            ||\n");
    
    printf("  ||  ");
    color_cyan();
    printf("[0]");
    color_normal();
    printf(" HELP                   - User guide and examples                       ||\n");
    
    printf("  ||  ");
    color_rojo();
    printf("[6]");
    color_normal();
    printf(" EXIT                   - Close the program                             ||\n");
    
    printf("  ||                                                                            ||\n");
    color_verde();
    printf("  ================================================================================\n");
    color_normal();
}

// Función para mostrar la ayuda
void mostrar_ayuda() {
    limpiar_pantalla();
    color_cyan();
    printf("\n  ================================================================================\n");
    printf("  ||                            USER GUIDE                                      ||\n");
    printf("  ================================================================================\n");
    color_normal();
    
    printf("\n");
    color_verde();
    printf("  1. WHAT IS THIS CALCULATOR?\n");
    color_normal();
    printf("     This system allows you to work with mathematical expressions:\n");
    printf("     - INFIX   : Standard notation (e.g., a + b * c)\n");
    printf("     - PREFIX  : Operators before operands (e.g., + a * b c)\n");
    printf("     - POSTFIX : Operators after operands (e.g., a b c * +)\n");
    
    printf("\n");
    color_verde();
    printf("  2. OPERATOR PRECEDENCE\n");
    color_normal();
    printf("     From highest to lowest priority:\n");
    printf("     1. ( )    - Parentheses\n");
    printf("     2. ^      - Exponentiation\n");
    printf("     3. * /    - Multiplication and Division\n");
    printf("     4. + -    - Addition and Subtraction\n");
    
    printf("\n");
    color_verde();
    printf("  3. MODULE DESCRIPTIONS\n");
    color_normal();
    
    printf("\n     [1] PREFIX with LETTERS\n");
    printf("         - Converts infix to prefix using variables (a, b, c...)\n");
    printf("         - Example: a+b*c -> +a*bc\n");
    
    printf("\n     [2] INFIX EVALUATOR\n");
    printf("         - Evaluates infix expressions with numbers\n");
    printf("         - Example: 3+4*5 -> 23\n");
    printf("         - Can save results to a file\n");
    
    printf("\n     [3] POSTFIX with LETTERS\n");
    printf("         - Converts infix to postfix using variables\n");
    printf("         - Example: a+b*c -> abc*+\n");
    
    printf("\n     [4] PREFIX with NUMBERS\n");
    printf("         - Converts and evaluates infix to prefix\n");
    printf("         - Example: 18/(3^2) -> /18^32 -> 2\n");
    
    printf("\n     [5] POSTFIX with NUMBERS\n");
    printf("         - Converts and evaluates infix to postfix\n");
    printf("         - Example: 3^2^3 -> 3 2 3 ^ ^ -> 6561\n");
    
    printf("\n");
    color_verde();
    printf("  4. USAGE TIPS\n");
    color_normal();
    printf("     - Always use parentheses when in doubt\n");
    printf("     - Spaces are optional\n");
    printf("     - For letters: use single lowercase letters (a-z)\n");
    printf("     - For numbers: multi-digit numbers are supported\n");
    
    printf("\n");
    color_verde();
    printf("  5. CONVERSION EXAMPLES\n");
    color_normal();
    printf("     Infix:    a*b+(c^2-d)\n");
    printf("     Prefix:   +*ab-^c2d\n");
    printf("     Postfix:  ab*c2^d-+\n");
    
    printf("\n     Infix:    (a+b)*c\n");
    printf("     Prefix:   *+abc\n");
    printf("     Postfix:  ab+c*\n");
    
    printf("\n");
    color_amarillo();
    printf("  Press ENTER to return to main menu...");
    color_normal();
    getchar();
}

// Función para pausar
void pausar() {
    printf("\n");
    color_amarillo();
    printf("  Press ENTER to return to main menu...");
    color_normal();
    while(getchar() != '\n');
    getchar();
}

// Función para ejecutar un programa externo
void ejecutar_modulo(const char* nombre_ejecutable, const char* nombre_modulo) {
    limpiar_pantalla();
    color_verde();
    printf("\n  Starting: %s...\n", nombre_modulo);
    color_normal();
    printf("  ==============================================================\n\n");
    
    int resultado = system(nombre_ejecutable);
    
    if (resultado != 0) {
        printf("\n  ");
        color_rojo();
        printf("ERROR: Could not execute %s\n", nombre_ejecutable);
        printf("       Make sure the file exists in the same directory.\n");
        color_normal();
    }
    
    pausar();
}

// Main
int main(void) {
    int opcion;
    char buffer[10];
    
    while(1) {
        limpiar_pantalla();
        mostrar_banner();
        mostrar_menu();
        
        printf("\n  ");
        color_amarillo();
        printf(">> Select an option: ");
        color_azul();
        fflush(stdout);
        
        if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        opcion = atoi(buffer);
        color_normal();
        
        switch(opcion) {
            case 1:
                ejecutar_modulo("PRE-LETTERS.exe", "PREFIX with LETTERS");
                break;
                
            case 2:
                ejecutar_modulo("Infix.exe", "INFIX EVALUATOR");
                break;
                
            case 3:
                ejecutar_modulo("POSTFIX-LETTERS.exe", "POSTFIX with LETTERS");
                break;
                
            case 4:
                ejecutar_modulo("PRE-NUM.exe", "PREFIX with NUMBERS");
                break;
                
            case 5:
                ejecutar_modulo("POST-NUM.exe", "POSTFIX with NUMBERS");
                break;
                
            case 0:
                mostrar_ayuda();
                break;
                
            case 6:
                limpiar_pantalla();
                printf("\n");
                color_cyan();
                printf("  ================================================================================\n");
                printf("  ||                                                                            ||\n");
                printf("  ||            ");
                color_verde();
                printf("Thank you for using the calculator!");
                color_cyan();
                printf("                             ||\n");
                printf("  ||                                                                            ||\n");
                printf("  ||                         ");
                color_amarillo();
                printf("Goodbye!");
                color_cyan();
                printf("                                          ||\n");
                printf("  ||                                                                            ||\n");
                printf("  ================================================================================\n");
                color_normal();
                printf("\n");
                return 0;
                
            default:
                color_rojo();
                printf("\n  ERROR: Invalid option. Please select a number from 0 to 6.\n");
                color_normal();
                pausar();
                break;
        }
    }
    
    return 0;
}
