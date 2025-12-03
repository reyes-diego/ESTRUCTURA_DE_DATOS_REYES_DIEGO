//INFIX.C - Portable Version
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// #include <windows.h> // Eliminado
#include <math.h>
#include <time.h>
// #include <windows.h> // Eliminado
#include "list.h"
#include "stack.h"
#include "queue.h"
#include "dlist.h"

#define MAX_EXPR 256
#define MAX_PATH 512

// --- Definiciones de Secuencias VT100 ---
#define RESET_COLOR "\033[0m"
#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_BLUE "\033[1;34m"
// Opcional: Puedes definir más si lo deseas
// #define COLOR_MAGENTA "\033[1;35m"
// #define COLOR_CYAN "\033[1;36m"
// #define COLOR_WHITE "\033[1;37m"

// --- Función para limpiar pantalla portable ---
void clear_screen() {
    printf("\033[2J\033[H");
    fflush(stdout); // Asegura que la limpieza se aplique inmediatamente
}

/*
    Inicializar sistema de colores
    No longer needed with VT100
*/
/*
void init_colores() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    originalAttributes = consoleInfo.wAttributes;
}
*/

/*
    Restaurar color original
*/
void color_normal() {
    printf(RESET_COLOR);
}

/*
    Establecer color azul
*/
void color_azul() {
    printf(COLOR_BLUE);
}

/*
    Establecer color verde
*/
void color_verde() {
    printf(COLOR_GREEN);
}

void color_amarillo() {
    printf(COLOR_YELLOW);
}

/*
    Establecer color rojo
*/
void color_rojo() {
    printf(COLOR_RED);
}

// Structure for tokens
typedef struct {
    char tipo;  // 'N' = number, 'O' = operator, 'P' = parenthesis
    double valor;
    char operador;
} Token;

// Structure for evaluation steps
typedef struct {
    double operando1;
    double operando2;
    char operador;
    double resultado;
} Paso;

// Prototypes
int validarSintaxis(const char *expr);
void tokenizar(const char *expr, DList *tokens);
double evaluarExpresion(DList *tokens, Queue *pasos);
int precedencia(char op);
int esOperador(char c);
double aplicarOperacion(char op, double a, double b);
void mostrarPasos(Queue *pasos);
void liberarToken(void *data);
void liberarPaso(void *data);

// NEW FUNCTIONS FOR SAVING FILE
void guardarOperacionesEnArchivo(Queue *pasos, const char *expresion, double resultado, const char *nombreArchivo);
void mostrarPasosEnArchivo(Queue *pasos, FILE *archivo);
int obtenerRutaArchivo(char *ruta);

// Funcin para mostrar tabla con formato
void mostrarTablaEvaluacion(DList *tokens);

// Main function
int main() {
    char expresion[MAX_EXPR];
    char rutaArchivo[MAX_PATH];
    DList tokens;
    Queue pasos;
    double resultado;
    
    // init_colores(); // Quitamos la inicialización de Windows
    
    clear_screen(); // Limpieza portable
    // system("cls"); // Quitamos dependencia de windows
    
    printf("\n\n");
    color_verde();
    printf("+===============================================================================================+\n");
    printf("|                                                                                               |\n");
    printf("|                       INFIX EXPRESSION CALCULATOR                                             |\n");
    printf("|                         EVALUATION STEP BY STEP                                               |\n");
    printf("|                                                                                               |\n");
    printf("+===============================================================================================+\n");
    color_normal();
    
    printf("\n");
    color_verde();
    printf("+===============================================================================================+\n");
    printf("| OPERATOR HIERARCHY (from highest to lowest precedence):                                       |\n");
    printf("+===============================================================================================|\n");
    printf("|  1. ( )         Parentheses                                                                   |\n");
    printf("|  2. ^           Exponents                                                                     |\n");
    printf("|  3. * /         Multiplication and Division                                                   |\n");
    printf("|  4. + -         Addition and Subtraction                                                      |\n");
    printf("+===============================================================================================+\n");
    color_normal();
    
    printf("\n");
    color_verde();
    printf("+===============================================================================================+\n");
    printf("| EVALUATION ALGORITHM:                                                                         |\n");
    printf("+===============================================================================================|\n");
    printf("|                                                                                               |\n");
    printf("|   Expression is read from LEFT TO RIGHT                                                      |\n");
    printf("|   Two stacks are used: one for numbers and one for operators                                 |\n");
    printf("|   Operators are processed according to their precedence                                      |\n");
    printf("|   Parentheses change the evaluation order                                                    |\n");
    printf("|                                                                                               |\n");
    printf("+===============================================================================================+\n");
    color_normal();
    
    printf("\n");
    color_verde();
    printf("+===============================================================================================+\n");
    printf("| EXAMPLES:                                                                                     |\n");
    printf("+===============================================================================================|\n");
    printf("|   3+4*5           ->    23                                                                   |\n");
    printf("|   (3+4)*5         ->    35                                                                   |\n");
    printf("|   2^3+4*5         ->    28                                                                   |\n");
    printf("+===============================================================================================+\n");
    color_normal();

    while(1) {
        printf("\n");
        color_amarillo();
        printf("  -> Enter the expression (or 'exit' to finish): ");
        color_azul();
        printf("");
        color_normal();
        fgets(expresion, MAX_EXPR, stdin);

        // Remove newline
        expresion[strcspn(expresion, "\n")] = 0;

        // Check for exit
        if(strcmp(expresion, "exit") == 0) {
            printf("\n");
            color_verde();
            printf("  Thank you for using the calculator! Goodbye!\n");
            color_normal();
            printf("\n");
            break;
        }

        // Validate syntax
        printf("\n");
        color_amarillo();
        printf("[1] Validating syntax...\n");
        color_normal();
        if(!validarSintaxis(expresion)) {
            color_rojo();
            printf("    ERROR: The expression has syntax errors.\n\n");
            color_normal();
            
            color_amarillo();
            printf("  Do you want to try another expression? (y/n): ");
            color_normal();
            char respuesta;
            scanf(" %c", &respuesta);
            getchar();
            
            if(respuesta != 'y' && respuesta != 'Y') {
                break;
            }
            continue;
        }
        
        color_verde();
        printf("    Syntax correct!\n");
        color_normal();

        // Tokenize
        printf("\n");
        color_amarillo();
        printf("[2] Tokenizing expression...\n");
        color_normal();
        dlist_init(&tokens, liberarToken);
        tokenizar(expresion, &tokens);
        color_azul();
        printf("    Tokens processed: %d\n", dlist_size(&tokens));
        color_normal();

        // Evaluate expression step by step
        printf("\n");
        color_amarillo();
        printf("[3] Evaluating expression with operation hierarchy...\n");
        color_normal();
        
        color_verde();
        printf("+-------------------------------------------------------------------------------------------------+\n");
        color_normal();

        queue_init(&pasos, liberarPaso);
        resultado = evaluarExpresion(&tokens, &pasos);

        printf("\n");
        color_amarillo();
        printf("[4] Evaluation steps (binary operations):\n");
        color_normal();
        
        color_verde();
        printf("+-------------------------------------------------------------------------------------------------+\n");
        color_normal();
        
        mostrarPasos(&pasos);
        
        color_verde();
        printf("+-------------------------------------------------------------------------------------------------+\n");
        color_normal();
        
        printf("\n");
        color_verde();
        printf("[FINAL RESULT] = ");
        color_azul();
        printf("%.4f\n", resultado);
        color_normal();

        // NEW FEATURE: SAVE TO FILE
        printf("\n");
        color_amarillo();
        printf("==============================================\n");
        printf("Do you want to save the operations to a file? (y/n): ");
        color_normal();
        char respuesta;
        scanf(" %c", &respuesta);
        getchar(); // Clear the buffer

        if(respuesta == 'y' || respuesta == 'Y') {
            if(obtenerRutaArchivo(rutaArchivo)) {
                guardarOperacionesEnArchivo(&pasos, expresion, resultado, rutaArchivo);
                color_verde();
                printf("Operations saved to: %s\n", rutaArchivo);
                color_normal();
            }
        }
        
        color_amarillo();
        printf("==============================================\n");
        color_normal();

        // Free memory
        dlist_destroy(&tokens);
        queue_destroy(&pasos);
    }

    return 0;
}

// Validate expression syntax
int validarSintaxis(const char *expr) {
    int i, len = strlen(expr);
    int parentesis = 0;
    int esperaOperando = 1;
    char anterior = '\0';
    int enNumero = 0;

    if(len == 0) {
        color_rojo();
        printf("    Error: Empty expression\n");
        color_normal();
        return 0;
    }

    for(i = 0; i < len; i++) {
        char c = expr[i];

        if(isspace(c)) {
            enNumero = 0;
            continue;
        }

        if(c == '(') {
            if(!esperaOperando && !esOperador(anterior) && anterior != '(' && anterior != '\0') {
                color_rojo();
                printf("    Error: Unexpected opening parenthesis at position %d\n", i);
                color_normal();
                return 0;
            }
            parentesis++;
            esperaOperando = 1;
            enNumero = 0;
            anterior = c;
        }
        else if(c == ')') {
            if(esperaOperando) {
                color_rojo();
                printf("    Error: Closing parenthesis after operator at position %d\n", i);
                color_normal();
                return 0;
            }
            parentesis--;
            if(parentesis < 0) {
                color_rojo();
                printf("    Error: Closing parenthesis without opening at position %d\n", i);
                color_normal();
                return 0;
            }
            esperaOperando = 0;
            enNumero = 0;
            anterior = c;
        }
        else if(isdigit(c) || c == '.') {
            if(!esperaOperando && !enNumero) {
                color_rojo();
                printf("    Error: Unexpected number at position %d\n", i);
                color_normal();
                return 0;
            }
            esperaOperando = 0;
            enNumero = 1;
        }
        else if(esOperador(c)) {
            if((c == '-' || c == '+') && esperaOperando) {
                esperaOperando = 1;
            }
            else if(esperaOperando) {
                color_rojo();
                printf("    Error: Unexpected operator '%c' at position %d\n", c, i);
                color_normal();
                return 0;
            }
            else {
                esperaOperando = 1;
            }
            enNumero = 0;
            anterior = c;
        }
        else {
            color_rojo();
            printf("    Error: Invalid character '%c' at position %d\n", c, i);
            color_normal();
            return 0;
        }
    }

    if(parentesis != 0) {
        color_rojo();
        printf("    Error: Unclosed parentheses\n");
        color_normal();
        return 0;
    }

    if(esperaOperando) {
        color_rojo();
        printf("    Error: Incomplete expression\n");
        color_normal();
        return 0;
    }

    return 1;
}

// Tokenize the expression
void tokenizar(const char *expr, DList *tokens) {
    int i = 0, len = strlen(expr);

    while(i < len) {
        if(isspace(expr[i])) {
            i++;
            continue;
        }

        Token *token = (Token*)malloc(sizeof(Token));

        if(isdigit(expr[i]) || expr[i] == '.') {
            char numStr[50];
            int j = 0;

            while(i < len && (isdigit(expr[i]) || expr[i] == '.')) {
                numStr[j++] = expr[i++];
            }
            numStr[j] = '\0';

            token->tipo = 'N';
            token->valor = atof(numStr);

            if(dlist_size(tokens) == 0) {
                dlist_ins_next(tokens, NULL, token);
            } else {
                dlist_ins_next(tokens, dlist_tail(tokens), token);
            }
        }
        else {
            token->tipo = (expr[i] == '(' || expr[i] == ')') ? 'P' : 'O';
            token->operador = expr[i];

            if(dlist_size(tokens) == 0) {
                dlist_ins_next(tokens, NULL, token);
            } else {
                dlist_ins_next(tokens, dlist_tail(tokens), token);
            }
            i++;
        }
    }
}

// Evaluate expression using two stacks (numbers and operators)
double evaluarExpresion(DList *tokens, Queue *pasos) {
    Stack pilaNumeros, pilaOperadores;
    DListNode *actual;

    stack_init(&pilaNumeros, NULL);
    stack_init(&pilaOperadores, NULL);

    actual = dlist_head(tokens);

    while(actual != NULL) {
        Token *token = (Token*)dlist_data(actual);

        if(token->tipo == 'N') {
            // Number: push directly
            double *num = (double*)malloc(sizeof(double));
            *num = token->valor;
            stack_push(&pilaNumeros, num);
        }
        else if(token->tipo == 'O') {
            // Operator: process according to precedence
            while(stack_size(&pilaOperadores) > 0) {
                char *topOp = (char*)stack_peek(&pilaOperadores);

                if(*topOp == '(') break;

                // Check precedence
                if(precedencia(token->operador) > precedencia(*topOp)) break;

                // Right associativity for ^
                if(token->operador == '^' && *topOp == '^') break;

                // Perform operation
                char *op;
                stack_pop(&pilaOperadores, (void**)&op);

                double *num2, *num1;
                stack_pop(&pilaNumeros, (void**)&num2);
                stack_pop(&pilaNumeros, (void**)&num1);

                double resultado = aplicarOperacion(*op, *num1, *num2);

                // Save step
                Paso *paso = (Paso*)malloc(sizeof(Paso));
                paso->operando1 = *num1;
                paso->operando2 = *num2;
                paso->operador = *op;
                paso->resultado = resultado;
                queue_enqueue(pasos, paso);

                // Push result
                double *res = (double*)malloc(sizeof(double));
                *res = resultado;
                stack_push(&pilaNumeros, res);

                free(op);
                free(num1);
                free(num2);
            }

            char *nuevoOp = (char*)malloc(sizeof(char));
            *nuevoOp = token->operador;
            stack_push(&pilaOperadores, nuevoOp);
        }
        else if(token->tipo == 'P') {
            if(token->operador == '(') {
                char *par = (char*)malloc(sizeof(char));
                *par = '(';
                stack_push(&pilaOperadores, par);
            }
            else if(token->operador == ')') {
                // Process until '(' is found
                while(stack_size(&pilaOperadores) > 0) {
                    char *op;
                    stack_pop(&pilaOperadores, (void**)&op);

                    if(*op == '(') {
                        free(op);
                        break;
                    }

                    double *num2, *num1;
                    stack_pop(&pilaNumeros, (void**)&num2);
                    stack_pop(&pilaNumeros, (void**)&num1);

                    double resultado = aplicarOperacion(*op, *num1, *num2);

                    // Save step
                    Paso *paso = (Paso*)malloc(sizeof(Paso));
                    paso->operando1 = *num1;
                    paso->operando2 = *num2;
                    paso->operador = *op;
                    paso->resultado = resultado;
                    queue_enqueue(pasos, paso);

                    double *res = (double*)malloc(sizeof(double));
                    *res = resultado;
                    stack_push(&pilaNumeros, res);

                    free(op);
                    free(num1);
                    free(num2);
                }
            }
        }

        actual = dlist_next(actual);
    }

    // Process remaining operators
    while(stack_size(&pilaOperadores) > 0) {
        char *op;
        stack_pop(&pilaOperadores, (void**)&op);

        double *num2, *num1;
        stack_pop(&pilaNumeros, (void**)&num2);
        stack_pop(&pilaNumeros, (void**)&num1);

        double resultado = aplicarOperacion(*op, *num1, *num2);

        // Save step
        Paso *paso = (Paso*)malloc(sizeof(Paso));
        paso->operando1 = *num1;
        paso->operando2 = *num2;
        paso->operador = *op;
        paso->resultado = resultado;
        queue_enqueue(pasos, paso);

        double *res = (double*)malloc(sizeof(double));
        *res = resultado;
        stack_push(&pilaNumeros, res);

        free(op);
        free(num1);
        free(num2);
    }

    // Get final result
    double *resultadoFinal;
    stack_pop(&pilaNumeros, (void**)&resultadoFinal);
    double valor = *resultadoFinal;
    free(resultadoFinal);

    stack_destroy(&pilaNumeros);
    stack_destroy(&pilaOperadores);

    return valor;
}

// Apply operation
double aplicarOperacion(char op, double a, double b) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if(b == 0) {
                color_rojo();
                printf("\nERROR: Division by zero!\n");
                color_normal();
                exit(1);
            }
            return a / b;
        case '^': return pow(a, b);
        default: return 0;
    }
}

// Show evaluation steps
void mostrarPasos(Queue *pasos) {
    ListNode *actual = list_head(pasos);
    int numPaso = 1;

    // Encabezado de la tabla
    color_verde();
    printf("| %-6s | %-15s | %-10s | %-15s | %-15s |\n", 
           "Step", "Operand 1", "Operator", "Operand 2", "Result");
    color_normal();
    color_verde();
    printf("+--------+-----------------+------------+-----------------+-----------------+\n");
    color_normal();

    while(actual != NULL) {
        Paso *paso = (Paso*)list_data(actual);
        if (paso != NULL) {
            printf("| ");
            color_azul();
            printf("%-6d", numPaso++);
            color_normal();
            printf(" | ");
            color_amarillo();
            printf("%-15.4f", paso->operando1);
            color_normal();
            printf(" | ");
            color_rojo();
            printf("%-10c", paso->operador);
            color_normal();
            printf(" | ");
            color_amarillo();
            printf("%-15.4f", paso->operando2);
            color_normal();
            printf(" | ");
            color_verde();
            printf("%-15.4f", paso->resultado);
            color_normal();
            printf(" |\n");
        }
        actual = list_next(actual);
    }
}

// Get operator precedence
int precedencia(char op) {
    switch(op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

// Check if it's an operator
int esOperador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

// Free token
void liberarToken(void *data) {
    free(data);
}

// Free step
void liberarPaso(void *data) {
    free(data);
}

// NEW FUNCTIONS FOR FILE HANDLING

void guardarOperacionesEnArchivo(Queue *pasos, const char *expresion, double resultado, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "a");
    if(archivo == NULL) {
        color_rojo();
        printf("Error: Could not create/open the file '%s'\n", nombreArchivo);
        color_normal();
        return;
    }

    // Write header with date and time
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(archivo, "==============================================\n");
    fprintf(archivo, "SAVED OPERATION: %s\n", buffer);
    fprintf(archivo, "Expression: %s\n", expresion);
    fprintf(archivo, "----------------------------------------------\n");
    fprintf(archivo, "Evaluation steps:\n");

    // Write steps
    mostrarPasosEnArchivo(pasos, archivo);

    fprintf(archivo, "----------------------------------------------\n");
    fprintf(archivo, "FINAL RESULT: %.4f\n", resultado);
    fprintf(archivo, "==============================================\n\n");

    fclose(archivo);
}

void mostrarPasosEnArchivo(Queue *pasos, FILE *archivo) {
    if (pasos == NULL || archivo == NULL) return;

    ListNode *actual = list_head(pasos);
    int numPaso = 1;

    while(actual != NULL) {
        Paso *paso = (Paso*)list_data(actual);
        if (paso != NULL) {
            fprintf(archivo, "Step %d: %.4f %c %.4f = %.4f\n",
                   numPaso++,
                   paso->operando1,
                   paso->operador,
                   paso->operando2,
                   paso->resultado);
        }
        actual = list_next(actual);
    }
}

int obtenerRutaArchivo(char *ruta) {
    color_amarillo();
    printf("Enter the file path and name (e.g., operations.txt or C:/my_operations.txt):\n> ");
    color_normal();

    if(fgets(ruta, MAX_PATH, stdin) == NULL) {
        color_rojo();
        printf("Error reading the path.\n");
        color_normal();
        return 0;
    }

    // Remove newline
    ruta[strcspn(ruta, "\n")] = 0;

    // Verify the path is not empty
    if(strlen(ruta) == 0) {
        color_rojo();
        printf("Error: The path cannot be empty.\n");
        color_normal();
        return 0;
    }

    // Verify it ends with .txt, if not, add it
    if(strlen(ruta) < 4 || strcmp(ruta + strlen(ruta) - 4, ".txt") != 0) {
        strcat(ruta, ".txt");
    }

    return 1;
}