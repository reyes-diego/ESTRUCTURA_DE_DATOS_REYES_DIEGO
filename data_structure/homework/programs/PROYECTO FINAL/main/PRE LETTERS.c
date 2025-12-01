#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "stack.h"
#include "dlist.h"
#include "list.h"
#define MAX_EXPR 256

/* Global variables for colors */
HANDLE hConsole;
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
WORD originalAttributes;

/*
    Initialize color system
*/
void init_colores() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    originalAttributes = consoleInfo.wAttributes;
}

/*
    Restore original color
*/
void color_normal() {
    SetConsoleTextAttribute(hConsole, originalAttributes);
}

/*
    Set blue color
*/
void color_azul() {
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
}

/*
    Set green color
*/
void color_verde() {
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

void color_amarillo() {
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

/*
    Set red color
*/
void color_rojo() {
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
}

/*
    Determine operator precedence
*/
int precedencia(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;  /* Exponent */
    if (op == 's') return 4;  /* sqrt (square root) */
    return 0;
}

/*
    Check if it's an operator
*/
int es_operador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == 's');
}

/*
    NEW FUNCTION: Validate infix expression syntax
    Returns 1 if expression is valid, 0 if it has errors
*/
int validar_sintaxis(const char *infija) {
    int i;
    int len = strlen(infija);
    int parentesis_balance = 0;
    int ultimo_fue_operando = 0;
    int ultimo_fue_operador = 0;
    char c;
    char ultimo_char = '\0';
    
    /* Check for empty expression */
    if (len == 0) {
        color_rojo();
        printf("\n  ERROR: The expression is empty\n");
        color_normal();
        return 0;
    }
    
    for (i = 0; i < len; i++) {
        c = infija[i];
        
        /* Ignore spaces */
        if (c == ' ') continue;
        
        /* Check for invalid characters */
        if (!isalnum(c) && !es_operador(c) && c != '(' && c != ')') {
            color_rojo();
            printf("\n  ERROR: Invalid character '%c' at position %d\n", c, i + 1);
            color_normal();
            return 0;
        }
        
        /* Check parentheses */
        if (c == '(') {
            parentesis_balance++;
            /* Cannot have operand before ( */
            if (ultimo_fue_operando) {
                color_rojo();
                printf("\n  ERROR: Missing operator before parenthesis '(' at position %d\n", i + 1);
                color_normal();
                return 0;
            }
            ultimo_fue_operador = 0;
            ultimo_fue_operando = 0;
        }
        else if (c == ')') {
            parentesis_balance--;
            /* Check for negative balance */
            if (parentesis_balance < 0) {
                color_rojo();
                printf("\n  ERROR: Closing parenthesis ')' without opening at position %d\n", i + 1);
                color_normal();
                return 0;
            }
            /* Cannot have operator before ) */
            if (ultimo_fue_operador) {
                color_rojo();
                printf("\n  ERROR: Missing operand before parenthesis ')' at position %d\n", i + 1);
                color_normal();
                return 0;
            }
            /* Cannot have ( followed by ) */
            if (ultimo_char == '(') {
                color_rojo();
                printf("\n  ERROR: Empty parentheses '()' at position %d\n", i + 1);
                color_normal();
                return 0;
            }
            ultimo_fue_operador = 0;
            ultimo_fue_operando = 1;
        }
        /* If it's an operand */
        else if (isalnum(c)) {
            /* Cannot have two consecutive operands */
            if (ultimo_fue_operando) {
                color_rojo();
                printf("\n  ERROR: Two consecutive operands at position %d (missing operator)\n", i + 1);
                color_normal();
                return 0;
            }
            ultimo_fue_operando = 1;
            ultimo_fue_operador = 0;
        }
        /* If it's an operator */
        else if (es_operador(c)) {
            /* Cannot have two consecutive operators */
            if (ultimo_fue_operador) {
                color_rojo();
                printf("\n  ERROR: Two consecutive operators at position %d\n", i + 1);
                color_normal();
                return 0;
            }
            /* Cannot start with operator (except - for negatives, but simplified here) */
            if (i == 0) {
                color_rojo();
                printf("\n  ERROR: Expression cannot start with an operator\n");
                color_normal();
                return 0;
            }
            /* Cannot have operator after ( */
            if (ultimo_char == '(') {
                color_rojo();
                printf("\n  ERROR: Operator '%c' after parenthesis '(' at position %d\n", c, i + 1);
                color_normal();
                return 0;
            }
            ultimo_fue_operador = 1;
            ultimo_fue_operando = 0;
        }
        
        ultimo_char = c;
    }
    
    /* Check final parentheses balance */
    if (parentesis_balance > 0) {
        color_rojo();
        printf("\n  ERROR: Missing %d closing parentheses ')'\n", parentesis_balance);
        color_normal();
        return 0;
    }
    
    /* Cannot end with operator */
    if (ultimo_fue_operador) {
        color_rojo();
        printf("\n  ERROR: Expression cannot end with an operator\n");
        color_normal();
        return 0;
    }
    
    /* If we got here, syntax is valid */
    color_verde();
    printf("\n  Valid syntax\n");
    color_normal();
    return 1;
}

/*
    Print stack content from RIGHT TO LEFT with colors
    Improved version: centered elements and ordered from right to left
    New elements are inserted to the LEFT of the first element
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
        /* When stack is empty, print centered spaces */
        for (i = 0; i < 25; i++) printf(" ");
        return;
    }
    
    /* Copy elements to temporary stack to preserve original order */
    stack_init(&temp_stack, NULL);
    
    /* Extract elements from main stack */
    while (stack_size(stack) > 0) {
        stack_pop(stack, (void **)&op_ptr);
        elementos[count++] = *op_ptr;  /* Save the character */
        stack_push(&temp_stack, op_ptr);  /* Preserve to restore later */
    }
    
    /* Restore original stack */
    while (stack_size(&temp_stack) > 0) {
        stack_pop(&temp_stack, (void **)&op_ptr);
        stack_push(stack, op_ptr);
    }
    
    /* CALCULATE SPACES FOR CENTERING */
    /* Each element takes 2 spaces (character + space) except the last one */
    longitud_total = (count * 2) - 1;
    espacios = (25 - longitud_total) / 2;
    
    /* Ensure spaces is not negative */
    if (espacios < 0) espacios = 0;
    
    /* Print initial spaces for centering */
    for (i = 0; i < espacios; i++) {
        printf(" ");
    }
    
    /* Print elements from LEFT TO RIGHT (new elements to the left) */
    /* The last entered element (top) is shown on the LEFT */
    for (i = 0; i < count; i++) {
        if (resaltar && i == 0 && elementos[i] == nuevo_elemento) {
            color_azul();
            printf("%c", elementos[i]);
            color_normal();
        } else {
            printf("%c", elementos[i]);
        }
        
        /* Add space between elements, except after the last one */
        if (i < count - 1) printf(" ");
    }
    
    /* Complete with spaces if needed to maintain alignment */
    int espacios_restantes = 25 - (espacios + longitud_total);
    for (i = 0; i < espacios_restantes; i++) {
        printf(" ");
    }
    
    stack_destroy(&temp_stack);
}

/*
    Print expression with last element in blue (from LEFT TO RIGHT)
    Improved version: centered elements
*/
void imprimir_operacion_coloreada(const char *operacion, int longitud, int resaltar_ultimo) {
    int i;
    int espacios;
    int longitud_total;
    
    if (longitud == 0) {
        for (i = 0; i < 29; i++) printf(" ");
        return;
    }
    
    /* CALCULATE SPACES FOR CENTERING */
    longitud_total = (longitud * 2) - 1;
    espacios = (29 - longitud_total) / 2;
    
    /* Ensure spaces is not negative */
    if (espacios < 0) espacios = 0;
    
    /* Print initial spaces for centering */
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
    
    /* Complete with spaces if needed to maintain alignment */
    int espacios_restantes = 29 - (espacios + longitud_total);
    for (i = 0; i < espacios_restantes; i++) {
        printf(" ");
    }
}

/*
    Convert infix to prefix WITH PROFESSIONAL TABLE
    TRAVERSAL FROM RIGHT TO LEFT (last element first)
    STACK is filled from RIGHT TO LEFT
    OPERATION is filled from LEFT TO RIGHT
*/
void infija_a_prefija(const char *infija, char *prefija) {
    Stack stack;
    int i;
    int j = 0;
    int paso = 1;
    char c;
    char *op_ptr;
    char temp_operacion[MAX_EXPR];
    int longitud;
    
    /* Initialize stack */
    stack_init(&stack, free);
    
    printf("\n");
    color_verde();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|                          CONVERSION FROM INFIX TO PREFIX                                        |\n");
    printf("|                                STEP BY STEP ALGORITHM                                           |\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
    color_normal();
    
    printf("\n");
    color_amarillo();
    printf("  Entered Infix Expression: ");
    color_azul();
    printf("%s\n", infija);
    color_normal();
    
    printf("  Traversal Method: ");
    color_verde();
    printf("RIGHT -> LEFT");
    color_normal();
    printf(" (last element first)\n\n");
    
    printf("+-----------------------------------------------------------------------------------------------------------------+\n");
    printf("|       |                          |                         |                             |\n");
    printf("|  STEP |         ACTION           |       STACK (R -> L)    |       OPERATION (L -> R)    |\n");
    printf("|       |                          |                         |                             |\n");
    printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
    
    temp_operacion[0] = '\0';
    longitud = strlen(infija);
    
    /* TRAVERSE FROM RIGHT TO LEFT (last element first) */
    for (i = longitud - 1; i >= 0; i--) {
        c = infija[i];
        
        /* Ignore spaces */
        if (c == ' ')
            continue;
        
        printf("|  %3d  | ", paso);
        
        /* If it's an operand (letter or number) - goes directly to OPERATION */
        if (isalnum(c)) {
            temp_operacion[j++] = c;
            temp_operacion[j] = '\0';
            
            printf("ADD [%c]          ", c);
            printf("|    ");
            imprimir_stack(&stack, '\0', 0);
            printf(" | ");
            imprimir_operacion_coloreada(temp_operacion, j, 1);
            printf(" |\n");
        }
        /* If it's a RIGHT parenthesis - PUSH to stack */
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
        /* If it's a LEFT parenthesis - POP until finding ) */
        else if (c == '(') {
            printf("FOUND [%c]       ", c);
            printf("|    ");
            imprimir_stack(&stack, '\0', 0);
            printf(" | ");
            imprimir_operacion_coloreada(temp_operacion, j, 0);
            printf(" |\n");
            
            printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
            paso++;
            
            /* POP until finding the right parenthesis ) */
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
                    
                    printf("|  %3d  | POP [%c] (search ')') ", paso, *op_ptr);
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
        /* If it's an operator */
        else if (es_operador(c)) {
            /* POP operators of HIGHER OR EQUAL hierarchy */
            while (stack_size(&stack) > 0) {
                char *top_op;
                top_op = (char *)stack_peek(&stack);
                
                if (top_op && *top_op != ')' && precedencia(*top_op) >= precedencia(c)) {
                    stack_pop(&stack, (void **)&op_ptr);
                    temp_operacion[j++] = *op_ptr;
                    temp_operacion[j] = '\0';
                    
                    printf("POP [%c] (prec %d>=%d) ", *op_ptr, precedencia(*top_op), precedencia(c));
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
            
            /* PUSH current operator */
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
    
    /* Separator before emptying stack */
    if (stack_size(&stack) > 0) {
        printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
        color_amarillo();
        printf("|       |     EMPTYING STACK       |                         |                             |\n");
        color_normal();
        printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
    }
    
    /* POP all remaining operators to empty the STACK */
    while (stack_size(&stack) > 0) {
        stack_pop(&stack, (void **)&op_ptr);
        temp_operacion[j++] = *op_ptr;
        temp_operacion[j] = '\0';
        
        printf("|  %3d  | FINAL POP [%c]       ", paso, *op_ptr);
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
    
    /* INVERT the result using DLIST (read from tail to head) */
    DList lista;
    dlist_init(&lista, NULL);
    
    /* Insert each character at the end of the list */
    for (i = 0; i < j; i++) {
        char *caracter = (char *)malloc(sizeof(char));
        *caracter = temp_operacion[i];
        dlist_ins_next(&lista, dlist_tail(&lista), caracter);
    }
    
    /* Read from tail to head to invert */
    int k = 0;
    DListNode *nodo = dlist_tail(&lista);
    
    while (nodo != NULL) {
        prefija[k++] = *(char *)dlist_data(nodo);
        nodo = dlist_prev(nodo);
    }
    prefija[k] = '\0';
    
    /* Destroy list and free memory */
    dlist_destroy(&lista);
    
    printf("\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
    color_amarillo();
    printf("|                           FINAL STEP: INVERT RESULT                                            |\n");
    color_normal();
    printf("|-------------------------------------------------------------------------------------------------|\n");
    printf("|                                                                                                 |\n");
    printf("|   Before inversion:   %-60s |\n", temp_operacion);
    printf("|                                                                                                 |\n");
    printf("|   After inversion:    ");
    color_azul();
    printf("%-58s", prefija);
    color_normal();
    printf(" |\n");
    printf("|                                                                                                 |\n");
    printf("|   Method used: DLIST (reading from TAIL to HEAD)                                               |\n");
    printf("|                                                                                                 |\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
    
    /* Destroy stack */
    stack_destroy(&stack);
}

/*
    Function to evaluate prefix expression through traversals
    Identifies binary operations of the form: operator letter letter
    For example: +ab is evaluated and replaced with a new variable
*/
void evaluar_prefija(const char *prefija) {
    char expresion[MAX_EXPR];
    char resultado[MAX_EXPR];
    int i, j, k;
    int paso = 1;
    char nueva_var = 'z';  /* Variable to be used to replace operations */
    int cambios = 1;
    int longitud;
    
    strcpy(expresion, prefija);
    
    printf("\n");
    color_verde();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|                          PREFIX EXPRESSION VERIFICATION                                        |\n");
    printf("|                              STEP BY STEP EVALUATION                                           |\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
    color_normal();
    
    printf("\n");
    color_amarillo();
    printf("  Prefix Expression to Verify: ");
    color_azul();
    printf("%s\n", expresion);
    color_normal();
    
    printf("  Method: Search for binary operations of the form: ");
    color_verde();
    printf("operator letter letter");
    color_normal();
    printf("\n\n");
    
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|  STEP |         OPERATION FOUND              |         RESULTING EXPRESSION                    |\n");
    printf("|-------------------------------------------------------------------------------------------------|\n");
    
    /* Iterate until no more changes */
    while (cambios) {
        cambios = 0;
        longitud = strlen(expresion);
        
        /* Search for binary operations: operator followed by two operands */
        for (i = 0; i < longitud - 2; i++) {
            /* If we find an operator followed by two letters */
            if (es_operador(expresion[i]) &&
                isalpha(expresion[i+1]) &&
                isalpha(expresion[i+2])) {
                
                /* Print the found operation */
                printf("|  %3d  |   ", paso);
                color_azul();
                printf("%c%c%c", expresion[i], expresion[i+1], expresion[i+2]);
                color_normal();
                printf(" = ");
                color_verde();
                printf("%c", nueva_var);
                color_normal();
                printf("                       |   ");
                
                /* Build the new expression */
                j = 0;
                /* Copy until before the operation */
                for (k = 0; k < i; k++) {
                    resultado[j++] = expresion[k];
                }
                /* Insert the new variable */
                resultado[j++] = nueva_var;
                /* Copy the rest of the expression (skipping the 3 characters) */
                for (k = i + 3; k < longitud; k++) {
                    resultado[j++] = expresion[k];
                }
                resultado[j] = '\0';
                
                /* Print the resulting expression with the new variable highlighted */
                for (k = 0; k < strlen(resultado); k++) {
                    if (k == i) {
                        color_verde();
                        printf("%c", resultado[k]);
                        color_normal();
                    } else {
                        printf("%c", resultado[k]);
                    }
                    if (k < strlen(resultado) - 1) printf(" ");
                }
                
                /* Complete spaces */
                int espacios = 45 - (strlen(resultado) * 2 - 1);
                for (k = 0; k < espacios; k++) printf(" ");
                printf("|\n");
                
                /* Update the expression */
                strcpy(expresion, resultado);
                nueva_var--;  /* Next variable (z, y, x, ...) */
                cambios = 1;
                paso++;
                
                if (strlen(expresion) > 1) {
                    printf("|-------------------------------------------------------------------------------------------------|\n");
                }
                
                break;  /* Restart search from the beginning */
            }
        }
    }
    
    printf("+-------------------------------------------------------------------------------------------------+\n");
    
    printf("\n");
    color_verde();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|                                FINAL VERIFICATION RESULT                                        |\n");
    printf("|-------------------------------------------------------------------------------------------------|\n");
    color_normal();
    printf("|                                                                                                 |\n");
    printf("|   Original Expression:  ");
    color_amarillo();
    printf("%-60s", prefija);
    color_normal();
    printf(" |\n");
    printf("|                                                                                                 |\n");
    printf("|   Final Result:         ");
    color_azul();
    printf("%-60s", expresion);
    color_normal();
    printf(" |\n");
    printf("|                                                                                                 |\n");
    
    if (strlen(expresion) == 1) {
        color_verde();
        printf("|   Status: SUCCESSFUL VERIFICATION - Expression reduced to a single variable                    |\n");
        color_normal();
    } else {
        color_rojo();
        printf("|   Status: INCONCLUSIVE VERIFICATION - Expression not completely reduced                        |\n");
        color_normal();
    }
    
    printf("|                                                                                                 |\n");
    color_verde();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    color_normal();
}

/*
    Main
*/
int main(void) {
    char infija[MAX_EXPR];
    char prefija[MAX_EXPR];
    char continuar;
    
    /* Initialize color system */
    init_colores();
    
    do {
        system("cls");  /* Clear screen on each iteration */
        
        printf("\n\n");
        color_verde();
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("|                                                                                                 |\n");
        printf("|                       INFIX TO PREFIX CALCULATOR                                                |\n");
        printf("|                         STEP BY STEP CONVERSION                                                 |\n");
        printf("|                                                                                                 |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        color_normal();
        
        printf("\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("| OPERATOR HIERARCHY (from highest to lowest precedence):                                         |\n");
        printf("+-------------------------------------------------------------------------------------------------|\n");
        printf("|  1. ( )         Parentheses                                                                     |\n");
        printf("|  2. s           Roots                                                                           |\n");
        printf("|  3. ^           Exponents                                                                       |\n");
        printf("|  4. * /         Multiplication and Division                                                     |\n");
        printf("|  5. + -         Addition and Subtraction                                                        |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        
        printf("\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("| ALGORITHM TO CONVERT FROM INFIX TO PREFIX:                                                      |\n");
        printf("+-------------------------------------------------------------------------------------------------|\n");
        printf("|                                                                                                 |\n");
        printf("|  • Read from RIGHT TO LEFT (last element first)                                                 |\n");
        printf("|  • The infix operation is NOT inverted (remains original)                                       |\n");
        printf("|  • STACK COLUMN: Filled from RIGHT TO LEFT                                                      |\n");
        printf("|  • OPERATION COLUMN: Filled from LEFT TO RIGHT                                                  |\n");
        printf("|                                                                                                 |\n");
        printf("|  POP IS DONE WHEN:                                                                              |\n");
        printf("|    • Parentheses are closed: ( )                                                                |\n");
        printf("|    • About to PUSH to operation of LOWER OR EQUAL hierarchy                                     |\n");
        printf("|    • No more elements to add (empty the STACK)                                                  |\n");
        printf("|                                                                                                 |\n");
        printf("|  FINAL STEP: INVERT the complete result                                                         |\n");
        printf("|                                                                                                 |\n");
        printf("|  NOTE: New elements appear in ");
        color_azul();
        printf("RED COLOR");
        color_normal();
        printf("                                                          |\n");
        printf("|                                                                                                 |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        
        printf("\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("| CONVERSION EXAMPLES:                                                                            |\n");
        printf("+-------------------------------------------------------------------------------------------------|\n");
        printf("|  • a*b+(c^2-d)     ->    +*ab-^c2d                                                             |\n");
        printf("|  • (a+b)*c         ->    *+abc                                                                 |\n");
        printf("|  • a+b*c           ->    +a*bc                                                                 |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        
        printf("\n");
        color_amarillo();
        printf("  -> Enter the infix expression (without spaces): ");
        color_normal();
        fgets(infija, MAX_EXPR, stdin);
        
        /* Remove newline */
        infija[strcspn(infija, "\n")] = '\0';
        
        /* VALIDATE SYNTAX BEFORE CONVERTING */
        if (!validar_sintaxis(infija)) {
            printf("\n");
            color_rojo();
            printf("  The expression contains errors. Please correct the syntax.\n");
            color_normal();
            printf("\n");
            color_amarillo();
            printf("  Want to try another expression? (y/n): ");
            color_normal();
            continuar = getchar();
            while (getchar() != '\n');  /* Clear buffer */
            
            if (continuar != 's' && continuar != 'S') {
                break;  /* Exit loop if doesn't want to continue */
            }
            continue;  /* Ask for expression again */
        }
        
        /* Convert to prefix */
        infija_a_prefija(infija, prefija);
        
        printf("\n");
        color_verde();
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("|                                FINAL RESULT                                                     |\n");
        printf("|-------------------------------------------------------------------------------------------------|\n");
        color_normal();
        printf("|                                                                                                 |\n");
        printf("|   Infix Expression:   ");
        color_amarillo();
        printf("%-60s", infija);
        color_normal();
        printf(" |\n");
        printf("|                                                                                                 |\n");
        printf("|   Prefix Expression:  ");
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
        printf("  Conversion successfully completed\n");
        color_normal();
        
        /* Perform prefix expression verification */
        evaluar_prefija(prefija);
        
        printf("\n");
        color_amarillo();
        printf("  Want to convert another expression? (y/n): ");
        color_normal();
        continuar = getchar();
        while (getchar() != '\n');  /* Clear buffer */
        
    } while (continuar == 's' || continuar == 'S');
    
    printf("\n");
    color_verde();
    printf("  Thank you for using the calculator. Goodbye!\n");
    color_normal();
    printf("\n");
    
    return 0;
}
