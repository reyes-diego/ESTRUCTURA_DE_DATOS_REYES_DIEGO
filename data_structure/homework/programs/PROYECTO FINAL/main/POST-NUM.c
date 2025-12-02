#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <math.h>
#include "stack.h"
#include "dlist.h" 
#include "list.h" 
#define MAX_EXPR 256

/* Variables globales para colores */
HANDLE hConsole;
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
WORD originalAttributes;

/*
    Inicializar sistema de colores
*/
void init_colores() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    originalAttributes = consoleInfo.wAttributes;
}

void color_normal() {
    SetConsoleTextAttribute(hConsole, originalAttributes);
}

void color_azul() {
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

void color_verde() {
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

void color_amarillo() {
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

void color_rojo() {
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
}

/*
    Determinar precedencia de operadores
*/
int precedencia(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;  /* Exponente - mayor precedencia */
    return 0;
}

/*
    Verificar si es operador
*/
int es_operador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

/*
    Validar sintaxis de la expresion infija - SOLO NUMEROS Y OPERADORES
*/
int validar_sintaxis(const char *infija) {
    int i;
    int len = strlen(infija);
    int parentesis_balance = 0;
    int ultimo_fue_operando = 0;
    int ultimo_fue_operador = 0;
    int dentro_numero = 0;
    char c;
    char ultimo_char = '\0';
    
    if (len == 0) {
        color_rojo();
        printf("\n  ERROR: La expresion esta vacia\n");
        color_normal();
        return 0;
    }
    
    for (i = 0; i < len; i++) {
        c = infija[i];
        
        if (c == ' ') {
            dentro_numero = 0;
            continue;
        }
        
        /* Verificar caracteres invalidos */
        if (!isdigit(c) && !es_operador(c) && c != '(' && c != ')') {
            color_rojo();
            printf("\n  ERROR: Caracter invalido '%c' en la posicion %d\n", c, i + 1);
            color_normal();
            return 0;
        }
        
        /* Verificar parentesis */
        if (c == '(') {
            parentesis_balance++;
            if (ultimo_fue_operando && !dentro_numero) {
                color_rojo();
                printf("\n  ERROR: Falta operador antes del parentesis '(' en posicion %d\n", i + 1);
                color_normal();
                return 0;
            }
            ultimo_fue_operador = 0;
            ultimo_fue_operando = 0;
            dentro_numero = 0;
        }
        else if (c == ')') {
            parentesis_balance--;
            if (parentesis_balance < 0) {
                color_rojo();
                printf("\n  ERROR: Parentesis ')' sin apertura en posicion %d\n", i + 1);
                color_normal();
                return 0;
            }
            if (ultimo_fue_operador) {
                color_rojo();
                printf("\n  ERROR: Falta operando antes del parentesis ')' en posicion %d\n", i + 1);
                color_normal();
                return 0;
            }
            if (ultimo_char == '(') {
                color_rojo();
                printf("\n  ERROR: Parentesis vacios '()' en posicion %d\n", i + 1);
                color_normal();
                return 0;
            }
            ultimo_fue_operador = 0;
            ultimo_fue_operando = 1;
            dentro_numero = 0;
        }
        else if (isdigit(c)) {
            if (dentro_numero) {
                /* Esto es parte del mismo numero */
            } 
            else {
                if (ultimo_fue_operando) {
                    color_rojo();
                    printf("\n  ERROR: Falta operador entre numeros en posicion %d\n", i + 1);
                    color_normal();
                    return 0;
                }
                ultimo_fue_operando = 1;
                dentro_numero = 1;
            }
            ultimo_fue_operador = 0;
        }
        else if (es_operador(c)) {
            if (ultimo_fue_operador) {
                color_rojo();
                printf("\n  ERROR: Dos operadores consecutivos en posicion %d\n", i + 1);
                color_normal();
                return 0;
            }
            if (i == 0 && c != '-' && c != '+') {
                color_rojo();
                printf("\n  ERROR: La expresion no puede empezar con un operador\n");
                color_normal();
                return 0;
            }
            if (ultimo_char == '(' && c != '-' && c != '+') {
                color_rojo();
                printf("\n  ERROR: Operador '%c' despues de parentesis '(' en posicion %d\n", c, i + 1);
                color_normal();
                return 0;
            }
            ultimo_fue_operador = 1;
            ultimo_fue_operando = 0;
            dentro_numero = 0;
        }
        
        ultimo_char = c;
    }
    
    if (parentesis_balance > 0) {
        color_rojo();
        printf("\n  ERROR: Faltan %d parentesis de cierre ')'\n", parentesis_balance);
        color_normal();
        return 0;
    }
    
    if (ultimo_fue_operador) {
        color_rojo();
        printf("\n  ERROR: La expresion no puede terminar con un operador\n");
        color_normal();
        return 0;
    }
    
    color_verde();
    printf("\n  Sintaxis valida\n");
    color_normal();
    return 1;
}

/*
    Funcion para calcular potencias (exponenciacion)
    NOTA: El operador ^ tiene asociatividad a la derecha
*/
int potencia(int base, int exponente) {
    int resultado = 1;
    for (int i = 0; i < exponente; i++) {
        resultado *= base;
    }
    return resultado;
}

/*
    Verificar si operador es asociativo a la derecha
    Solo el operador ^ es asociativo a la derecha
*/
int es_asociativo_derecha(char op) {
    return (op == '^');
}

/*
    Funcion para evaluar expresion posfija paso a paso - CORREGIDA
    Ahora sigue exactamente el algoritmo de evaluacion postfija
*/
void evaluar_posfija_paso_a_paso(const char *posfija) {
    Stack pila;
    char expresion[MAX_EXPR];
    int i, j;
    int paso = 1;
    int resultado_final;
    
    stack_init(&pila, free);
    
    printf("\n");
    color_verde();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|                          EVALUACION DE EXPRESION POSFIJA                                       |\n");
    printf("|                              PASO A PASO (IZQUIERDA -> DERECHA)                                |\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
    color_normal();
    
    printf("\n");
    color_amarillo();
    printf("  Expresion Posfija a Evaluar: ");
    color_azul();
    printf("%s\n", posfija);
    color_normal();
    
    printf("  Metodo: Recorrido de IZQUIERDA A DERECHA, usando una pila\n");
    printf("  Cada operacion se realiza cuando se encuentra un operador\n\n");
    
    printf("+---------------------------------------------------------------------------------------------------------+\n");
    printf("| PASO |      PILA ACTUAL       |      OPERACION REALIZADA      |               RESULTADO                |\n");
    printf("+---------------------------------------------------------------------------------------------------------+\n");
    
    /* Copiar la expresion original */
    strcpy(expresion, posfija);
    
    /* Procesar la expresion token por token */
    i = 0;
    while (i < strlen(expresion)) {
        /* Saltar espacios */
        if (expresion[i] == ' ') {
            i++;
            continue;
        }
        
        char token[20];
        int token_idx = 0;
        
        /* Extraer token */
        while (i < strlen(expresion) && expresion[i] != ' ') {
            token[token_idx++] = expresion[i++];
        }
        token[token_idx] = '\0';
        
        /* Mostrar paso actual */
        printf("| %3d  | ", paso);
        
        /* Imprimir contenido de la pila */
        Stack temp_pila;
        stack_init(&temp_pila, NULL);
        
        /* Contar elementos en pila */
        int pila_count = 0;
        int *elementos_pila[100];
        
        while (stack_size(&pila) > 0) {
            int *elem;
            stack_pop(&pila, (void **)&elem);
            elementos_pila[pila_count++] = elem;
            stack_push(&temp_pila, elem);
        }
        
        /* Restaurar pila original */
        while (stack_size(&temp_pila) > 0) {
            int *elem;
            stack_pop(&temp_pila, (void **)&elem);
            stack_push(&pila, elem);
        }
        
        stack_destroy(&temp_pila);
        
        /* Imprimir pila de derecha a izquierda */
        if (pila_count == 0) {
            printf("%-22s", "[Vacia]");
        } else {
            char pila_str[50] = "";
            for (int k = pila_count - 1; k >= 0; k--) {
                char num_str[10];
                sprintf(num_str, "%d", *elementos_pila[k]);
                strcat(pila_str, num_str);
                if (k > 0) strcat(pila_str, " ");
            }
            printf("%-22s", pila_str);
        }
        
        /* Si es un numero */
        if (isdigit(token[0])) {
            int *num_ptr = (int *)malloc(sizeof(int));
            *num_ptr = atoi(token);
            stack_push(&pila, num_ptr);
            
            printf("| LEER: %-23s | %-37s |\n", token, "Apilar numero");
        }
        /* Si es un operador */
        else if (es_operador(token[0])) {
            /* Verificar que hay al menos dos operandos en la pila */
            if (stack_size(&pila) < 2) {
                printf("| ERROR: Operador sin operandos suficientes |\n");
                break;
            }
            
            /* Obtener operandos (nota: segundo operando primero) */
            int *num2_ptr, *num1_ptr;
            stack_pop(&pila, (void **)&num2_ptr);
            stack_pop(&pila, (void **)&num1_ptr);
            
            int num1 = *num1_ptr;
            int num2 = *num2_ptr;
            int resultado;
            
            /* Realizar la operacion */
            switch (token[0]) {
                case '+':
                    resultado = num1 + num2;
                    break;
                case '-':
                    resultado = num1 - num2;
                    break;
                case '*':
                    resultado = num1 * num2;
                    break;
                case '/':
                    if (num2 != 0) {
                        resultado = num1 / num2;
                    } else {
                        resultado = 0;
                    }
                    break;
                case '^':
                    resultado = potencia(num1, num2);
                    break;
                default:
                    resultado = 0;
            }
            
            /* Mostrar operacion realizada */
            char operacion_str[50];
            sprintf(operacion_str, "%d %d %c", num1, num2, token[0]);
            printf("| %-30s | ", operacion_str);
            
            /* Mostrar resultado */
            char resultado_str[40];
            sprintf(resultado_str, "= %d", resultado);
            color_verde();
            printf("%-37s", resultado_str);
            color_normal();
            printf(" |\n");
            
            /* Liberar memoria de operandos */
            free(num1_ptr);
            free(num2_ptr);
            
            /* Apilar resultado */
            int *res_ptr = (int *)malloc(sizeof(int));
            *res_ptr = resultado;
            stack_push(&pila, res_ptr);
        }
        
        paso++;
    }
    
    printf("+---------------------------------------------------------------------------------------------------------+\n");
    
    /* Obtener resultado final */
    if (stack_size(&pila) == 1) {
        int *res_final_ptr;
        stack_pop(&pila, (void **)&res_final_ptr);
        resultado_final = *res_final_ptr;
        free(res_final_ptr);
    } else {
        resultado_final = 0;
    }
    
    /* Limpiar pila */
    while (stack_size(&pila) > 0) {
        int *elem;
        stack_pop(&pila, (void **)&elem);
        free(elem);
    }
    
    stack_destroy(&pila);
    
    /* Mostrar resultado final */
    printf("\n");
    color_verde();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|                                RESULTADO FINAL DE EVALUACION                                     |\n");
    printf("|-------------------------------------------------------------------------------------------------|\n");
    color_normal();
    printf("|                                                                                                 |\n");
    printf("|   Expresion Posfija Original: ");
    color_amarillo();
    printf("%-60s", posfija);
    color_normal();
    printf(" |\n");
    printf("|                                                                                                 |\n");
    printf("|   Resultado de la Evaluacion: ");
    color_azul();
    printf("%-58d", resultado_final);
    color_normal();
    printf(" |\n");
    printf("|                                                                                                 |\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
}

/*
    Imprimir el contenido del stack
*/
void imprimir_stack(Stack *stack, char nuevo_elemento, int resaltar) {
    Stack temp_stack;
    char *op_ptr;
    char elementos[MAX_EXPR];
    int count = 0;
    int i;
    int espacios;
    int longitud_total;
    
    if (stack_size(stack) == 0) {
        for (i = 0; i < 25; i++) printf(" ");
        return;
    }
    
    stack_init(&temp_stack, NULL);
    
    while (stack_size(stack) > 0) {
        stack_pop(stack, (void **)&op_ptr);
        elementos[count++] = *op_ptr;
        stack_push(&temp_stack, op_ptr);
    }
    
    while (stack_size(&temp_stack) > 0) {
        stack_pop(&temp_stack, (void **)&op_ptr);
        stack_push(stack, op_ptr);
    }
    
    longitud_total = (count * 2) - 1;
    espacios = (25 - longitud_total) / 2;
    
    if (espacios < 0) espacios = 0;
    
    for (i = 0; i < espacios; i++) {
        printf(" ");
    }
    
    for (i = 0; i < count; i++) {
        if (resaltar && i == 0 && elementos[i] == nuevo_elemento) {
            color_azul();
            printf("%c", elementos[i]);
            color_normal();
        } else {
            printf("%c", elementos[i]);
        }
        
        if (i < count - 1) printf(" ");
    }
    
    int espacios_restantes = 25 - (espacios + longitud_total);
    for (i = 0; i < espacios_restantes; i++) {
        printf(" ");
    }
    
    stack_destroy(&temp_stack);
}

/*
    Imprimir expresion con el ultimo elemento en azul
*/
void imprimir_operacion_coloreada(const char *operacion, int longitud, int resaltar_ultimo) {
    int i;
    int espacios;
    int longitud_total;
    
    if (longitud == 0) {
        for (i = 0; i < 29; i++) printf(" ");
        return;
    }
    
    longitud_total = (longitud * 2) - 1;
    espacios = (29 - longitud_total) / 2;
    
    if (espacios < 0) espacios = 0;
    
    for (i = 0; i < espacios; i++) {
        printf(" ");
    }
    
    for (i = 0; i < longitud; i++) {
        if (resaltar_ultimo && i == longitud - 1) {
            color_azul();
            printf("%c", operacion[i]);
            color_normal();
        } else {
            printf("%c", operacion[i]);
        }
        if (i < longitud - 1) printf(" ");
    }
    
    int espacios_restantes = 29 - (espacios + longitud_total);
    for (i = 0; i < espacios_restantes; i++) {
        printf(" ");
    }
}

/*
    Convertir infija a posfija - CORREGIDA para asociatividad del operador ^
*/
void infija_a_posfija(const char *infija, char *posfija) {
    Stack stack;
    int i, j = 0, paso = 1;
    char temp_operacion[MAX_EXPR] = "";
    int longitud = strlen(infija);
    
    stack_init(&stack, free);
    
    printf("\n");
    color_verde();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|                          CONVERSION DE INFIJA A POSFIJA                                        |\n");
    printf("|                                ALGORITMO PASO A PASO                                           |\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
    color_normal();
    
    printf("\n");
    color_amarillo();
    printf("  Expresion Infija Ingresada: ");
    color_azul();
    printf("%s\n", infija);
    color_normal();
    
    printf("  Metodo de Recorrido: ");
    color_verde();
    printf("IZQUIERDA -> DERECHA");
    color_normal();
    printf(" (primer elemento primero)\n\n");
    
    printf("+-----------------------------------------------------------------------------------------------------------------+\n");
    printf("|       |                          |                         |                             |\n");
    printf("|  PASO |         ACCION           |       STACK (D -> I)    |       OPERACION (I -> D)    |\n");
    printf("|       |                          |                         |                             |\n");
    printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
    
    /* Procesar cada caracter */
    for (i = 0; i < longitud; i++) {
        char c = infija[i];
        if (c == ' ') continue;
        
        printf("|  %3d  | ", paso);
        
        /* Si es un digito */
        if (isdigit(c)) {
            /* Extraer numero completo */
            char numero[20];
            int num_idx = 0;
            while (i < longitud && isdigit(infija[i])) {
                numero[num_idx++] = infija[i++];
            }
            numero[num_idx] = '\0';
            i--; /* Ajustar indice */
            
            /* Agregar a la expresion posfija */
            for (int k = 0; k < num_idx; k++) {
                temp_operacion[j++] = numero[k];
            }
            temp_operacion[j++] = ' ';
            temp_operacion[j] = '\0';
            
            printf("AGREGAR [%s]       ", numero);
            printf("|    ");
            imprimir_stack(&stack, '\0', 0);
            printf(" | ");
            imprimir_operacion_coloreada(temp_operacion, j, 0);
            printf(" |\n");
        }
        /* Si es parentesis izquierdo */
        else if (c == '(') {
            char *op_ptr = (char *)malloc(sizeof(char));
            *op_ptr = c;
            stack_push(&stack, op_ptr);
            
            printf("PUSH [%c]            ", c);
            printf("|    ");
            imprimir_stack(&stack, c, 1);
            printf(" | ");
            imprimir_operacion_coloreada(temp_operacion, j, 0);
            printf(" |\n");
        }
        /* Si es parentesis derecho */
        else if (c == ')') {
            printf("ENCONTRADO [%c]       ", c);
            printf("|    ");
            imprimir_stack(&stack, '\0', 0);
            printf(" | ");
            imprimir_operacion_coloreada(temp_operacion, j, 0);
            printf(" |\n");
            
            printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
            paso++;
            
            /* Vaciar stack hasta encontrar '(' */
            char *op_ptr;
            while (stack_size(&stack) > 0) {
                char *top = (char *)stack_peek(&stack);
                if (top && *top == '(') {
                    stack_pop(&stack, (void **)&op_ptr);
                    free(op_ptr);
                    printf("|  %3d  | POP [(]             ", paso);
                    printf("|    ");
                    imprimir_stack(&stack, '\0', 0);
                    printf(" | ");
                    imprimir_operacion_coloreada(temp_operacion, j, 0);
                    printf(" |\n");
                    break;
                } else {
                    stack_pop(&stack, (void **)&op_ptr);
                    temp_operacion[j++] = *op_ptr;
                    temp_operacion[j++] = ' ';
                    temp_operacion[j] = '\0';
                    
                    printf("|  %3d  | POP [%c] (buscar '(') ", paso, *op_ptr);
                    printf("|    ");
                    imprimir_stack(&stack, '\0', 0);
                    printf(" | ");
                    imprimir_operacion_coloreada(temp_operacion, j, 1);
                    printf(" |\n");
                    
                    free(op_ptr);
                    paso++;
                }
            }
            printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
            paso--;
        }
        /* Si es operador */
        else if (es_operador(c)) {
            /* Para operador ^ (asociativo a la derecha), manejo especial */
            while (stack_size(&stack) > 0) {
                char *top = (char *)stack_peek(&stack);
                if (top && *top != '(') {
                    int prec_top = precedencia(*top);
                    int prec_actual = precedencia(c);
                    
                    /* Para ^ (asociativo a derecha), solo sacamos si tiene mayor precedencia */
                    if (prec_top > prec_actual || 
                        (prec_top == prec_actual && !es_asociativo_derecha(c))) {
                        char *op_ptr;
                        stack_pop(&stack, (void **)&op_ptr);
                        temp_operacion[j++] = *op_ptr;
                        temp_operacion[j++] = ' ';
                        temp_operacion[j] = '\0';
                        
                        printf("POP [%c] (prec %d>=%d) ", *op_ptr, precedencia(*op_ptr), precedencia(c));
                        printf("|    ");
                        imprimir_stack(&stack, '\0', 0);
                        printf(" | ");
                        imprimir_operacion_coloreada(temp_operacion, j, 1);
                        printf(" |\n");
                        printf("|  %3d  | ", paso + 1);
                        
                        free(op_ptr);
                        paso++;
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            }
            
            /* PUSH del operador actual */
            char *op_ptr = (char *)malloc(sizeof(char));
            *op_ptr = c;
            stack_push(&stack, op_ptr);
            
            printf("PUSH [%c]            ", c);
            printf("|    ");
            imprimir_stack(&stack, c, 1);
            printf(" | ");
            imprimir_operacion_coloreada(temp_operacion, j, 0);
            printf(" |\n");
        }
        
        if (i < longitud - 1) {
            printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
        }
        paso++;
    }
    
    /* Vaciar stack restante */
    if (stack_size(&stack) > 0) {
        printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
        color_amarillo();
        printf("|       |     VACIANDO STACK       |                         |                             |\n");
        color_normal();
        printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
    }
    
    while (stack_size(&stack) > 0) {
        char *op_ptr;
        stack_pop(&stack, (void **)&op_ptr);
        temp_operacion[j++] = *op_ptr;
        temp_operacion[j++] = ' ';
        temp_operacion[j] = '\0';
        
        printf("|  %3d  | POP FINAL [%c]       ", paso, *op_ptr);
        printf("|    ");
        imprimir_stack(&stack, '\0', 0);
        printf(" | ");
        imprimir_operacion_coloreada(temp_operacion, j, 1);
        printf(" |\n");
        
        if (stack_size(&stack) > 0) {
            printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
        }
        
        free(op_ptr);
        paso++;
    }
    
    printf("+-----------------------------------------------------------------------------------------------------------------+\n");
    
    /* Eliminar espacio final si existe */
    if (j > 0 && temp_operacion[j-1] == ' ') {
        temp_operacion[j-1] = '\0';
    }
    
    strcpy(posfija, temp_operacion);
    
    printf("\n");
    color_verde();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|                                RESULTADO FINAL                                                 |\n");
    printf("|-------------------------------------------------------------------------------------------------|\n");
    color_normal();
    printf("|                                                                                                 |\n");
    printf("|   Expresion Posfija: ");
    color_azul();
    printf("%-58s", posfija);
    color_normal();
    printf(" |\n");
    printf("|                                                                                                 |\n");
    printf("|   NOTA: El operador ^ tiene asociatividad a la derecha                                        |\n");
    printf("|                                                                                                 |\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
    
    stack_destroy(&stack);
}

/*
    Main
*/
int main(void) {
    char infija[MAX_EXPR];
    char posfija[MAX_EXPR];
    char continuar;
    
    init_colores();
    
    do {
        system("cls");
        
        printf("\n\n");
        color_verde();
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("|                                                                                                 |\n");
        printf("|                       CALCULADORA INFIJA A POSFIJA                                              |\n");
        printf("|                         CONVERSION PASO A PASO                                                  |\n");
        printf("|                                                                                                 |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        color_normal();
        
        printf("\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("| JERARQUIA DE OPERADORES (de mayor a menor precedencia):                                        |\n");
        printf("+-------------------------------------------------------------------------------------------------|\n");
        printf("|  1. ( )         Parentesis                                                                     |\n");
        printf("|  2. ^           Exponentes (asociativo a la derecha)                                           |\n");
        printf("|  3. * /         Multiplicacion y Division (asociativo a la izquierda)                          |\n");
        printf("|  4. + -         Suma y Resta (asociativo a la izquierda)                                       |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        
        printf("\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("| EJEMPLOS DE CONVERSION:                                                                         |\n");
        printf("+-------------------------------------------------------------------------------------------------|\n");
        printf("|  • 3^2^3 + 4*(5-2)^2 - 8/2^2  ->  3 2 3 ^ ^ 4 5 2 - 2 ^ * + 8 2 2 ^ / -                       |\n");
        printf("|  • 18/3^2+(4*5-2^3)*2        ->  18 3 2 ^ / 4 5 * 2 3 ^ - 2 * +                               |\n");
        printf("|  • (12+3)*4                  ->  12 3 + 4 *                                                    |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        
        printf("\n");
        color_amarillo();
        printf("  -> Ingrese la expresion infija (puede usar espacios): ");
        color_normal();
        fgets(infija, MAX_EXPR, stdin);
        
        infija[strcspn(infija, "\n")] = '\0';
        
        if (!validar_sintaxis(infija)) {
            printf("\n");
            color_rojo();
            printf("  La expresion contiene errores. Por favor, corrija la sintaxis.\n");
            color_normal();
            printf("\n");
            color_amarillo();
            printf("  Desea intentar con otra expresion? (s/n): ");
            color_normal();
            continuar = getchar();
            while (getchar() != '\n');
            
            if (continuar != 's' && continuar != 'S') {
                break;
            }
            continue;
        }
        
        infija_a_posfija(infija, posfija);
        
        printf("\n");
        color_verde();
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("|                                RESULTADO FINAL                                                  |\n");
        printf("|-------------------------------------------------------------------------------------------------|\n");
        color_normal();
        printf("|                                                                                                 |\n");
        printf("|   Expresion Infija:   ");
        color_amarillo();
        printf("%-60s", infija);
        color_normal();
        printf(" |\n");
        printf("|                                                                                                 |\n");
        printf("|   Expresion Posfija:  ");
        color_azul();
        printf("%-60s", posfija);
        color_normal();
        printf(" |\n");
        printf("|                                                                                                 |\n");
        color_verde();
        printf("+-------------------------------------------------------------------------------------------------+\n");
        color_normal();
        
        printf("\n");
        color_verde();
        printf("  Conversion completada exitosamente\n");
        color_normal();
        
        /* Realizar evaluacion paso a paso */
        evaluar_posfija_paso_a_paso(posfija);
        
        printf("\n");
        color_amarillo();
        printf("  Desea convertir otra expresion? (s/n): ");
        color_normal();
        continuar = getchar();
        while (getchar() != '\n');
        
    } while (continuar == 's' || continuar == 'S');
    
    printf("\n");
    color_verde();
    printf("  Gracias por usar la calculadora. Hasta pronto!\n");
    color_normal();
    printf("\n");
    
    return 0;
}
