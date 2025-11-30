#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "stack.h"
#include "dlist.h" 
#include "list.h" 
#define MAX_EXPR 256

HANDLE hConsole;
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
WORD originalAttributes;

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

int precedencia(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;  
    if (op == 's') return 4;  
    return 0;
}


int es_operador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == 's');
}

int validar_sintaxis(const char *infija) {
    int i;
    int len = strlen(infija);
    int parentesis_balance = 0;
    int ultimo_fue_operando = 0;
    int ultimo_fue_operador = 0;
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
        
        
        if (c == ' ') continue;
        

        if (!isalnum(c) && !es_operador(c) && c != '(' && c != ')') {
            color_rojo();
            printf("\n  ERROR: Caracter invalido '%c' en la posicion %d\n", c, i + 1);
            color_normal();
            return 0;
        }
        
  
        if (c == '(') {
            parentesis_balance++;
            
            if (ultimo_fue_operando) {
                color_rojo();
                printf("\n  ERROR: Falta operador antes del parentesis '(' en posicion %d\n", i + 1);
                color_normal();
                return 0;
            }
            ultimo_fue_operador = 0;
            ultimo_fue_operando = 0;
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
        }
       
        else if (isalnum(c)) {
            
            if (ultimo_fue_operando) {
                color_rojo();
                printf("\n  ERROR: Dos operandos consecutivos en posicion %d (falta operador)\n", i + 1);
                color_normal();
                return 0;
            }
            ultimo_fue_operando = 1;
            ultimo_fue_operador = 0;
        }
       
        else if (es_operador(c)) {
        
            if (ultimo_fue_operador) {
                color_rojo();
                printf("\n  ERROR: Dos operadores consecutivos en posicion %d\n", i + 1);
                color_normal();
                return 0;
            }
            
            if (i == 0) {
                color_rojo();
                printf("\n  ERROR: La expresion no puede empezar con un operador\n");
                color_normal();
                return 0;
            }
            
            if (ultimo_char == '(') {
                color_rojo();
                printf("\n  ERROR: Operador '%c' despues de parentesis '(' en posicion %d\n", c, i + 1);
                color_normal();
                return 0;
            }
            ultimo_fue_operador = 1;
            ultimo_fue_operando = 0;
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

void infija_a_prefija(const char *infija, char *prefija) {
    Stack stack;
    int i;
    int j = 0;
    int paso = 1;
    char c;
    char *op_ptr;
    char temp_operacion[MAX_EXPR];
    int longitud;
    
    
    stack_init(&stack, free);
    
    printf("\n");
    color_verde();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|                          CONVERSION DE INFIJA A PREFIJA                                        |\n");
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
    printf("DERECHA -> IZQUIERDA");
    color_normal(); 
    printf(" (ultimo elemento primero)\n\n");
    
    printf("+-----------------------------------------------------------------------------------------------------------------+\n");
    printf("|       |                          |                         |                             |\n");
    printf("|  PASO |         ACCION           |       STACK (D -> I)    |       OPERACION (I -> D)    |\n");
    printf("|       |                          |                         |                             |\n");
    printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
    
    temp_operacion[0] = '\0';
    longitud = strlen(infija);
  
    for (i = longitud - 1; i >= 0; i--) {
        c = infija[i];
        
        /* Ignorar espacios */
        if (c == ' ')
            continue;
        
        printf("|  %3d  | ", paso);
        
       
        if (isalnum(c)) {
            temp_operacion[j++] = c;
            temp_operacion[j] = '\0';
            
            printf("AGREGAR [%c]         ", c);
            printf("|    ");
            imprimir_stack(&stack, '\0', 0);
            printf(" | ");
            imprimir_operacion_coloreada(temp_operacion, j, 1);
            printf(" |\n");
        }
     
        else if (c == ')') {
            op_ptr = (char *)malloc(sizeof(char));
            *op_ptr = c;
            stack_push(&stack, op_ptr);
            
            printf("PUSH [%c]            ", c);
            printf("|    ");
            imprimir_stack(&stack, c, 1);
            printf(" | ");
            imprimir_operacion_coloreada(temp_operacion, j, 0);
            printf(" |\n");
        }
       
        else if (c == '(') {
            printf("ENCONTRADO [%c]       ", c);
            printf("|    ");
            imprimir_stack(&stack, '\0', 0);
            printf(" | ");
            imprimir_operacion_coloreada(temp_operacion, j, 0);
            printf(" |\n");
            
            printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
            paso++;
            
            
            while (stack_size(&stack) > 0) {
                stack_pop(&stack, (void **)&op_ptr);
                
                if (*op_ptr == ')') {
                    free(op_ptr);
                    printf("|  %3d  | POP [)]             ", paso);
                    printf("|    ");
                    imprimir_stack(&stack, '\0', 0);
                    printf(" | ");
                    imprimir_operacion_coloreada(temp_operacion, j, 0);
                    printf(" |\n");
                    break;
                } else {
                    temp_operacion[j++] = *op_ptr;
                    temp_operacion[j] = '\0';
                    
                    printf("|  %3d  | POP [%c] (buscar ')') ", paso, *op_ptr);
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
       
        else if (es_operador(c)) {
           
            while (stack_size(&stack) > 0) {
                char *top_op;
                top_op = (char *)stack_peek(&stack);
                
                if (top_op && *top_op != ')' && precedencia(*top_op) >= precedencia(c)) {
                    stack_pop(&stack, (void **)&op_ptr);
                    temp_operacion[j++] = *op_ptr;
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
            }
            
            
            op_ptr = (char *)malloc(sizeof(char));
            *op_ptr = c;
            stack_push(&stack, op_ptr);
            
            printf("PUSH [%c]            ", c);
            printf("|    ");
            imprimir_stack(&stack, c, 1);
            printf(" | ");
            imprimir_operacion_coloreada(temp_operacion, j, 0);
            printf(" |\n");
        }
        
        if (i > 0) {
            printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
        }
        paso++;
    }
    
    if (stack_size(&stack) > 0) {
        printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
        color_amarillo();
        printf("|       |     VACIANDO STACK       |                         |                             |\n");
        color_normal();
        printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
    }
    
 
    while (stack_size(&stack) > 0) {
        stack_pop(&stack, (void **)&op_ptr);
        temp_operacion[j++] = *op_ptr;
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
    
    temp_operacion[j] = '\0';
    
   
    DList lista;
    dlist_init(&lista, NULL);
    
    
    for (i = 0; i < j; i++) {
        char *caracter = (char *)malloc(sizeof(char));
        *caracter = temp_operacion[i];
        dlist_ins_next(&lista, dlist_tail(&lista), caracter);
    }
    
   
    int k = 0;
    DListNode *nodo = dlist_tail(&lista);
    
    while (nodo != NULL) {
        prefija[k++] = *(char *)dlist_data(nodo);
        nodo = dlist_prev(nodo);
    }
    prefija[k] = '\0';
    
   
    dlist_destroy(&lista);
    
    printf("\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
    color_amarillo();
    printf("|                           PASO FINAL: INVERTIR RESULTADO                                       |\n");
    color_normal();
    printf("|-------------------------------------------------------------------------------------------------|\n");
    printf("|                                                                                                 |\n");
    printf("|   Antes de invertir:   %-60s |\n", temp_operacion);
    printf("|                                                                                                 |\n");
    printf("|   Despues de invertir: ");
    color_azul();
    printf("%-58s", prefija);
    color_normal();
    printf(" |\n");
    printf("|                                                                                                 |\n");
    printf("|   Metodo usado: DLIST (lectura de COLA a CABEZA)                                               |\n");
    printf("|                                                                                                 |\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
    
    
    stack_destroy(&stack);
}


int main(void) {
    char infija[MAX_EXPR];
    char prefija[MAX_EXPR];
    char continuar;
    
   
    init_colores();
    
    do {
        system("cls"); 
        
        printf("\n\n");
        color_verde();
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("|                                                                                                 |\n");
        printf("|                       CALCULADORA INFIJA A PREFIJA                                              |\n");
        printf("|                         CONVERSION PASO A PASO                                                  |\n");
        printf("|                                                                                                 |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        color_normal();
        
        printf("\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("| JERARQUIA DE OPERADORES (de mayor a menor precedencia):                                        |\n");
        printf("+-------------------------------------------------------------------------------------------------|\n");
        printf("|  1. ( )         Parentesis                                                                     |\n");
        printf("|  2. s           Raices                                                                         |\n");
        printf("|  3. ^           Exponentes                                                                     |\n");
        printf("|  4. * /         Multiplicacion y Division                                                      |\n");
        printf("|  5. + -         Suma y Resta                                                                   |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        
        printf("\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("| ALGORITMO PARA CONVERTIR DE INFIJA A PREFIJA:                                                  |\n");
        printf("+-------------------------------------------------------------------------------------------------|\n");
        printf("|                                                                                                 |\n");
        printf("|  • Se lee de DERECHA A IZQUIERDA (ultimo elemento primero)                                      |\n");
        printf("|  • La operacion infija NO se invierte (se mantiene original)                                   |\n");
        printf("|  • COLUMNA STACK: Se llena de DERECHA A IZQUIERDA                                              |\n");
        printf("|  • COLUMNA OPERACION: Se llena de IZQUIERDA A DERECHA                                          |\n");
        printf("|                                                                                                 |\n");
        printf("|  SE HACE POP CUANDO:                                                                            |\n");
        printf("|    • Se cierran parentesis: ( )                                                                |\n");
        printf("|    • Se va a hacer PUSH a operacion de MENOR O IGUAL jerarquia                                 |\n");
        printf("|    • No hay mas elementos que agregar (vaciar el STACK)                                        |\n");
        printf("|                                                                                                 |\n");
        printf("|  PASO FINAL: Se INVIERTE el resultado completo                                                 |\n");
        printf("|                                                                                                 |\n");
        printf("|  NOTA: Los elementos nuevos aparecen en ");
        color_azul();
        printf("COLOR AZUL");
        color_normal();
        printf("                                                          |\n");
        printf("|                                                                                                 |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        
        printf("\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("| EJEMPLOS DE CONVERSION:                                                                         |\n");
        printf("+-------------------------------------------------------------------------------------------------|\n");
        printf("|  • a*b+(c^2-d)     ->    +*ab-^c2d                                                             |\n");
        printf("|  • (a+b)*c         ->    *+abc                                                                 |\n");
        printf("|  • a+b*c           ->    +a*bc                                                                 |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        
        printf("\n");
        color_amarillo();
        printf("  -> Ingrese la expresion infija (sin espacios): ");
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
            while (getchar() != '\n');  /* Limpiar buffer */
            
            if (continuar != 's' && continuar != 'S') {
                break; 
            }
            continue; 
        }
        
     
        infija_a_prefija(infija, prefija);
        
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
        printf("|   Expresion Prefija:  ");
        color_azul();
        printf("%-60s", prefija);
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
