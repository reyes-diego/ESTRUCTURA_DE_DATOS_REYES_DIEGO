//POST-NUM Portable Version
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// #include <windows.h> // Eliminado
#include <math.h>
#include "stack.h"
#include "dlist.h"
#include "list.h"
#define MAX_EXPR 256

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
    Initialize color system
    No longer needed with VT100
*/
/*
void init_colors() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    originalAttributes = consoleInfo.wAttributes;
}
*/

void normal_color() {
    printf(RESET_COLOR);
}

void blue_color() {
    printf(COLOR_BLUE);
}

void green_color() {
    printf(COLOR_GREEN);
}

void yellow_color() {
    printf(COLOR_YELLOW);
}

void red_color() {
    printf(COLOR_RED);
}

/*
    Determine operator precedence
*/
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;  /* Exponent - highest precedence */
    return 0;
}

/*
    Check if it's an operator
*/
int is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

/*
    Validate infix expression syntax - ONLY NUMBERS AND OPERATORS
*/
int validate_syntax(const char *infix) {
    int i;
    int len = strlen(infix);
    int parenthesis_balance = 0;
    int last_was_operand = 0;
    int last_was_operator = 0;
    int inside_number = 0;
    char c;
    char last_char = '\0';
    
    if (len == 0) {
        red_color();
        printf("\n  ERROR: The expression is empty\n");
        normal_color();
        return 0;
    }
    
    for (i = 0; i < len; i++) {
        c = infix[i];
        
        if (c == ' ') {
            inside_number = 0;
            continue;
        }
        
        /* Check for invalid characters */
        if (!isdigit(c) && !is_operator(c) && c != '(' && c != ')') {
            red_color();
            printf("\n  ERROR: Invalid character '%c' at position %d\n", c, i + 1);
            normal_color();
            return 0;
        }
        
        /* Check parentheses */
        if (c == '(') {
            parenthesis_balance++;
            if (last_was_operand && !inside_number) {
                red_color();
                printf("\n  ERROR: Missing operator before parenthesis '(' at position %d\n", i + 1);
                normal_color();
                return 0;
            }
            last_was_operator = 0;
            last_was_operand = 0;
            inside_number = 0;
        }
        else if (c == ')') {
            parenthesis_balance--;
            if (parenthesis_balance < 0) {
                red_color();
                printf("\n  ERROR: Closing parenthesis ')' without opening at position %d\n", i + 1);
                normal_color();
                return 0;
            }
            if (last_was_operator) {
                red_color();
                printf("\n  ERROR: Missing operand before parenthesis ')' at position %d\n", i + 1);
                normal_color();
                return 0;
            }
            if (last_char == '(') {
                red_color();
                printf("\n  ERROR: Empty parentheses '()' at position %d\n", i + 1);
                normal_color();
                return 0;
            }
            last_was_operator = 0;
            last_was_operand = 1;
            inside_number = 0;
        }
        else if (isdigit(c)) {
            if (inside_number) {
                /* This is part of the same number */
            } 
            else {
                if (last_was_operand) {
                    red_color();
                    printf("\n  ERROR: Missing operator between numbers at position %d\n", i + 1);
                    normal_color();
                    return 0;
                }
                last_was_operand = 1;
                inside_number = 1;
            }
            last_was_operator = 0;
        }
        else if (is_operator(c)) {
            if (last_was_operator) {
                red_color();
                printf("\n  ERROR: Two consecutive operators at position %d\n", i + 1);
                normal_color();
                return 0;
            }
            if (i == 0 && c != '-' && c != '+') {
                red_color();
                printf("\n  ERROR: The expression cannot start with an operator\n");
                normal_color();
                return 0;
            }
            if (last_char == '(' && c != '-' && c != '+') {
                red_color();
                printf("\n  ERROR: Operator '%c' after parenthesis '(' at position %d\n", c, i + 1);
                normal_color();
                return 0;
            }
            last_was_operator = 1;
            last_was_operand = 0;
            inside_number = 0;
        }
        
        last_char = c;
    }
    
    if (parenthesis_balance > 0) {
        red_color();
        printf("\n  ERROR: Missing %d closing parentheses ')'\n", parenthesis_balance);
        normal_color();
        return 0;
    }
    
    if (last_was_operator) {
        red_color();
        printf("\n  ERROR: The expression cannot end with an operator\n");
        normal_color();
        return 0;
    }
    
    green_color();
    printf("\n  Valid syntax\n");
    normal_color();
    return 1;
}

/*
    Function to calculate powers (exponentiation)
    NOTE: The ^ operator has right associativity
*/
int power(int base, int exponent) {
    int result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

/*
    Check if operator is right-associative
    Only the ^ operator is right-associative
*/
int is_right_associative(char op) {
    return (op == '^');
}

/*
    Function to evaluate postfix expression step by step - CORRECTED
    Now follows exactly the postfix evaluation algorithm
*/
void evaluate_postfix_step_by_step(const char *postfix) {
    Stack stack;
    char expression[MAX_EXPR];
    int i, j;
    int step = 1;
    int final_result;
    
    stack_init(&stack, free);
    
    printf("\n");
    green_color();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|                          POSTFIX EXPRESSION EVALUATION                                          |\n");
    printf("|                              STEP BY STEP (LEFT -> RIGHT)                                       |\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
    normal_color();
    
    printf("\n");
    yellow_color();
    printf("  Postfix Expression to Evaluate: ");
    blue_color();
    printf("%s\n", postfix);
    normal_color();
    
    printf("  Method: Traversal from LEFT TO RIGHT, using a stack\n");
    printf("  Each operation is performed when an operator is found\n\n");
    
    printf("+---------------------------------------------------------------------------------------------------------+\n");
    printf("| STEP |      CURRENT STACK      |      OPERATION PERFORMED      |               RESULT                  |\n");
    printf("+---------------------------------------------------------------------------------------------------------+\n");
    
    /* Copy the original expression */
    strcpy(expression, postfix);
    
    /* Process the expression token by token */
    i = 0;
    while (i < strlen(expression)) {
        /* Skip spaces */
        if (expression[i] == ' ') {
            i++;
            continue;
        }
        
        char token[20];
        int token_idx = 0;
        
        /* Extract token */
        while (i < strlen(expression) && expression[i] != ' ') {
            token[token_idx++] = expression[i++];
        }
        token[token_idx] = '\0';
        
        /* Show current step */
        printf("| %3d  | ", step);
        
        /* Print stack content */
        Stack temp_stack;
        stack_init(&temp_stack, NULL);
        
        /* Count elements in stack */
        int stack_count = 0;
        int *stack_elements[100];
        
        while (stack_size(&stack) > 0) {
            int *elem;
            stack_pop(&stack, (void **)&elem);
            stack_elements[stack_count++] = elem;
            stack_push(&temp_stack, elem);
        }
        
        /* Restore original stack */
        while (stack_size(&temp_stack) > 0) {
            int *elem;
            stack_pop(&temp_stack, (void **)&elem);
            stack_push(&stack, elem);
        }
        
        stack_destroy(&temp_stack);
        
        /* Print stack from right to left */
        if (stack_count == 0) {
            printf("%-22s", "[Empty]");
        } else {
            char stack_str[50] = "";
            for (int k = stack_count - 1; k >= 0; k--) {
                char num_str[10];
                sprintf(num_str, "%d", *stack_elements[k]);
                strcat(stack_str, num_str);
                if (k > 0) strcat(stack_str, " ");
            }
            printf("%-22s", stack_str);
        }
        
        /* If it's a number */
        if (isdigit(token[0])) {
            int *num_ptr = (int *)malloc(sizeof(int));
            *num_ptr = atoi(token);
            stack_push(&stack, num_ptr);
            
            printf("| READ: %-23s | %-37s |\n", token, "Push number");
        }
        /* If it's an operator */
        else if (is_operator(token[0])) {
            /* Verify there are at least two operands in the stack */
            if (stack_size(&stack) < 2) {
                printf("| ERROR: Operator without sufficient operands |\n");
                break;
            }
            
            /* Get operands (note: second operand first) */
            int *num2_ptr, *num1_ptr;
            stack_pop(&stack, (void **)&num2_ptr);
            stack_pop(&stack, (void **)&num1_ptr);
            
            int num1 = *num1_ptr;
            int num2 = *num2_ptr;
            int result;
            
            /* Perform the operation */
            switch (token[0]) {
                case '+':
                    result = num1 + num2;
                    break;
                case '-':
                    result = num1 - num2;
                    break;
                case '*':
                    result = num1 * num2;
                    break;
                case '/':
                    if (num2 != 0) {
                        result = num1 / num2;
                    } else {
                        result = 0;
                    }
                    break;
                case '^':
                    result = power(num1, num2);
                    break;
                default:
                    result = 0;
            }
            
            /* Show operation performed */
            char operation_str[50];
            sprintf(operation_str, "%d %d %c", num1, num2, token[0]);
            printf("| %-30s | ", operation_str);
            
            /* Show result */
            char result_str[40];
            sprintf(result_str, "= %d", result);
            green_color();
            printf("%-37s", result_str);
            normal_color();
            printf(" |\n");
            
            /* Free operand memory */
            free(num1_ptr);
            free(num2_ptr);
            
            /* Push result */
            int *res_ptr = (int *)malloc(sizeof(int));
            *res_ptr = result;
            stack_push(&stack, res_ptr);
        }
        
        step++;
    }
    
    printf("+---------------------------------------------------------------------------------------------------------+\n");
    
    /* Get final result */
    if (stack_size(&stack) == 1) {
        int *final_res_ptr;
        stack_pop(&stack, (void **)&final_res_ptr);
        final_result = *final_res_ptr;
        free(final_res_ptr);
    } else {
        final_result = 0;
    }
    
    /* Clean stack */
    while (stack_size(&stack) > 0) {
        int *elem;
        stack_pop(&stack, (void **)&elem);
        free(elem);
    }
    
    stack_destroy(&stack);
    
    /* Show final result */
    printf("\n");
    green_color();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|                                FINAL EVALUATION RESULT                                          |\n");
    printf("|-------------------------------------------------------------------------------------------------|\n");
    normal_color();
    printf("|                                                                                                 |\n");
    printf("|   Original Postfix Expression: ");
    yellow_color();
    printf("%-60s", postfix);
    normal_color();
    printf(" |\n");
    printf("|                                                                                                 |\n");
    printf("|   Evaluation Result: ");
    blue_color();
    printf("%-58d", final_result);
    normal_color();
    printf(" |\n");
    printf("|                                                                                                 |\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
}

/*
    Print stack content
*/
void print_stack(Stack *stack, char new_element, int highlight) {
    Stack temp_stack;
    char *op_ptr;
    char elements[MAX_EXPR];
    int count = 0;
    int i;
    int spaces;
    int total_length;
    
    if (stack_size(stack) == 0) {
        for (i = 0; i < 25; i++) printf(" ");
        return;
    }
    
    stack_init(&temp_stack, NULL);
    
    while (stack_size(stack) > 0) {
        stack_pop(stack, (void **)&op_ptr);
        elements[count++] = *op_ptr;
        stack_push(&temp_stack, op_ptr);
    }
    
    while (stack_size(&temp_stack) > 0) {
        stack_pop(&temp_stack, (void **)&op_ptr);
        stack_push(stack, op_ptr);
    }
    
    total_length = (count * 2) - 1;
    spaces = (25 - total_length) / 2;
    
    if (spaces < 0) spaces = 0;
    
    for (i = 0; i < spaces; i++) {
        printf(" ");
    }
    
    for (i = 0; i < count; i++) {
        if (highlight && i == 0 && elements[i] == new_element) {
            blue_color();
            printf("%c", elements[i]);
            normal_color();
        } else {
            printf("%c", elements[i]);
        }
        
        if (i < count - 1) printf(" ");
    }
    
    int remaining_spaces = 25 - (spaces + total_length);
    for (i = 0; i < remaining_spaces; i++) {
        printf(" ");
    }
    
    stack_destroy(&temp_stack);
}

/*
    Print expression with last element in blue
*/
void print_colored_operation(const char *operation, int length, int highlight_last) {
    int i;
    int spaces;
    int total_length;
    
    if (length == 0) {
        for (i = 0; i < 29; i++) printf(" ");
        return;
    }
    
    total_length = (length * 2) - 1;
    spaces = (29 - total_length) / 2;
    
    if (spaces < 0) spaces = 0;
    
    for (i = 0; i < spaces; i++) {
        printf(" ");
    }
    
    for (i = 0; i < length; i++) {
        if (highlight_last && i == length - 1) {
            blue_color();
            printf("%c", operation[i]);
            normal_color();
        } else {
            printf("%c", operation[i]);
        }
        if (i < length - 1) printf(" ");
    }
    
    int remaining_spaces = 29 - (spaces + total_length);
    for (i = 0; i < remaining_spaces; i++) {
        printf(" ");
    }
}

/*
    Convert infix to postfix - CORRECTED for ^ operator associativity
*/
void infix_to_postfix(const char *infix, char *postfix) {
    Stack stack;
    int i, j = 0, step = 1;
    char temp_operation[MAX_EXPR] = "";
    int length = strlen(infix);
    
    stack_init(&stack, free);
    
    printf("\n");
    green_color();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|                          INFIX TO POSTFIX CONVERSION                                           |\n");
    printf("|                                STEP-BY-STEP ALGORITHM                                          |\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
    normal_color();
    
    printf("\n");
    yellow_color();
    printf("  Entered Infix Expression: ");
    blue_color();
    printf("%s\n", infix);
    normal_color();
    
    printf("  Traversal Method: ");
    green_color();
    printf("LEFT -> RIGHT");
    normal_color();
    printf(" (first element first)\n\n");
    
    printf("+-----------------------------------------------------------------------------------------------------------------+\n");
    printf("|       |                          |                         |                             |\n");
    printf("|  STEP |         ACTION           |       STACK (D -> I)    |       OPERATION (I -> D)    |\n");
    printf("|       |                          |                         |                             |\n");
    printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
    
    /* Process each character */
    for (i = 0; i < length; i++) {
        char c = infix[i];
        if (c == ' ') continue;
        
        printf("|  %3d  | ", step);
        
        /* If it's a digit */
        if (isdigit(c)) {
            /* Extract complete number */
            char number[20];
            int num_idx = 0;
            while (i < length && isdigit(infix[i])) {
                number[num_idx++] = infix[i++];
            }
            number[num_idx] = '\0';
            i--; /* Adjust index */
            
            /* Add to postfix expression */
            for (int k = 0; k < num_idx; k++) {
                temp_operation[j++] = number[k];
            }
            temp_operation[j++] = ' ';
            temp_operation[j] = '\0';
            
            printf("ADD [%s]       ", number);
            printf("|    ");
            print_stack(&stack, '\0', 0);
            printf(" | ");
            print_colored_operation(temp_operation, j, 0);
            printf(" |\n");
        }
        /* If it's left parenthesis */
        else if (c == '(') {
            char *op_ptr = (char *)malloc(sizeof(char));
            *op_ptr = c;
            stack_push(&stack, op_ptr);
            
            printf("PUSH [%c]            ", c);
            printf("|    ");
            print_stack(&stack, c, 1);
            printf(" | ");
            print_colored_operation(temp_operation, j, 0);
            printf(" |\n");
        }
        /* If it's right parenthesis */
        else if (c == ')') {
            printf("FOUND [%c]       ", c);
            printf("|    ");
            print_stack(&stack, '\0', 0);
            printf(" | ");
            print_colored_operation(temp_operation, j, 0);
            printf(" |\n");
            
            printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
            step++;
            
            /* Empty stack until '(' is found */
            char *op_ptr;
            while (stack_size(&stack) > 0) {
                char *top = (char *)stack_peek(&stack);
                if (top && *top == '(') {
                    stack_pop(&stack, (void **)&op_ptr);
                    free(op_ptr);
                    printf("|  %3d  | POP [(]             ", step);
                    printf("|    ");
                    print_stack(&stack, '\0', 0);
                    printf(" | ");
                    print_colored_operation(temp_operation, j, 0);
                    printf(" |\n");
                    break;
                } else {
                    stack_pop(&stack, (void **)&op_ptr);
                    temp_operation[j++] = *op_ptr;
                    temp_operation[j++] = ' ';
                    temp_operation[j] = '\0';
                    
                    printf("|  %3d  | POP [%c] (find '(') ", step, *op_ptr);
                    printf("|    ");
                    print_stack(&stack, '\0', 0);
                    printf(" | ");
                    print_colored_operation(temp_operation, j, 1);
                    printf(" |\n");
                    
                    free(op_ptr);
                    step++;
                }
            }
            printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
            step--;
        }
        /* If it's an operator */
        else if (is_operator(c)) {
            /* For ^ operator (right-associative), special handling */
            while (stack_size(&stack) > 0) {
                char *top = (char *)stack_peek(&stack);
                if (top && *top != '(') {
                    int prec_top = precedence(*top);
                    int prec_current = precedence(c);
                    
                    /* For ^ (right-associative), we only pop if it has higher precedence */
                    if (prec_top > prec_current || 
                        (prec_top == prec_current && !is_right_associative(c))) {
                        char *op_ptr;
                        stack_pop(&stack, (void **)&op_ptr);
                        temp_operation[j++] = *op_ptr;
                        temp_operation[j++] = ' ';
                        temp_operation[j] = '\0';
                        
                        printf("POP [%c] (prec %d>=%d) ", *op_ptr, precedence(*op_ptr), precedence(c));
                        printf("|    ");
                        print_stack(&stack, '\0', 0);
                        printf(" | ");
                        print_colored_operation(temp_operation, j, 1);
                        printf(" |\n");
                        printf("|  %3d  | ", step + 1);
                        
                        free(op_ptr);
                        step++;
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            }
            
            /* PUSH current operator */
            char *op_ptr = (char *)malloc(sizeof(char));
            *op_ptr = c;
            stack_push(&stack, op_ptr);
            
            printf("PUSH [%c]            ", c);
            printf("|    ");
            print_stack(&stack, c, 1);
            printf(" | ");
            print_colored_operation(temp_operation, j, 0);
            printf(" |\n");
        }
        
        if (i < length - 1) {
            printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
        }
        step++;
    }
    
    /* Empty remaining stack */
    if (stack_size(&stack) > 0) {
        printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
        yellow_color();
        printf("|       |     EMPTYING STACK       |                         |                             |\n");
        normal_color();
        printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
    }
    
    while (stack_size(&stack) > 0) {
        char *op_ptr;
        stack_pop(&stack, (void **)&op_ptr);
        temp_operation[j++] = *op_ptr;
        temp_operation[j++] = ' ';
        temp_operation[j] = '\0';
        
        printf("|  %3d  | FINAL POP [%c]       ", step, *op_ptr);
        printf("|    ");
        print_stack(&stack, '\0', 0);
        printf(" | ");
        print_colored_operation(temp_operation, j, 1);
        printf(" |\n");
        
        if (stack_size(&stack) > 0) {
            printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
        }
        
        free(op_ptr);
        step++;
    }
    
    printf("+-----------------------------------------------------------------------------------------------------------------+\n");
    
    /* Remove final space if exists */
    if (j > 0 && temp_operation[j-1] == ' ') {
        temp_operation[j-1] = '\0';
    }
    
    strcpy(postfix, temp_operation);
    
    printf("\n");
    green_color();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|                                FINAL RESULT                                                    |\n");
    printf("|-------------------------------------------------------------------------------------------------|\n");
    normal_color();
    printf("|                                                                                                 |\n");
    printf("|   Postfix Expression: ");
    blue_color();
    printf("%-58s", postfix);
    normal_color();
    printf(" |\n");
    printf("|                                                                                                 |\n");
    printf("|   NOTE: The ^ operator has right associativity                                                |\n");
    printf("|                                                                                                 |\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
    
    stack_destroy(&stack);
}

/*
    Main
*/
int main(void) {
    char infix[MAX_EXPR];
    char postfix[MAX_EXPR];
    char continue_choice;
    
    // init_colors(); // Quitamos la inicialización de Windows
    
    do {
        clear_screen(); // Limpieza portable
        // system("cls"); // Quitamos dependencia de windows
        
        printf("\n\n");
        green_color();
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("|                                                                                                 |\n");
        printf("|                       INFIX TO POSTFIX CALCULATOR                                               |\n");
        printf("|                         STEP-BY-STEP CONVERSION                                                |\n");
        printf("|                                                                                                 |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        normal_color();
        
        printf("\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("| OPERATOR HIERARCHY (from highest to lowest precedence):                                        |\n");
        printf("+-------------------------------------------------------------------------------------------------|\n");
        printf("|  1. ( )         Parentheses                                                                   |\n");
        printf("|  2. ^           Exponents (right-associative)                                                 |\n");
        printf("|  3. * /         Multiplication and Division (left-associative)                                |\n");
        printf("|  4. + -         Addition and Subtraction (left-associative)                                   |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        
        printf("\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("| CONVERSION EXAMPLES:                                                                           |\n");
        printf("+-------------------------------------------------------------------------------------------------|\n");
        printf("|  • 3^2^3 + 4*(5-2)^2 - 8/2^2  ->  3 2 3 ^ ^ 4 5 2 - 2 ^ * + 8 2 2 ^ / -                       |\n");
        printf("|  • 18/3^2+(4*5-2^3)*2        ->  18 3 2 ^ / 4 5 * 2 3 ^ - 2 * +                               |\n");
        printf("|  • (12+3)*4                  ->  12 3 + 4 *                                                    |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        
        printf("\n");
        yellow_color();
        printf("  -> Enter the infix expression (you may use spaces): ");
        normal_color();
        fgets(infix, MAX_EXPR, stdin);
        
        infix[strcspn(infix, "\n")] = '\0';
        
        if (!validate_syntax(infix)) {
            printf("\n");
            red_color();
            printf("  The expression contains errors. Please correct the syntax.\n");
            normal_color();
            printf("\n");
            yellow_color();
            printf("  Do you want to try another expression? (y/n): ");
            normal_color();
            continue_choice = getchar();
            while (getchar() != '\n');
            
            if (continue_choice != 'y' && continue_choice != 'Y') {
                break;
            }
            continue;
        }
        
        infix_to_postfix(infix, postfix);
        
        printf("\n");
        green_color();
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("|                                FINAL RESULT                                                    |\n");
        printf("|-------------------------------------------------------------------------------------------------|\n");
        normal_color();
        printf("|                                                                                                 |\n");
        printf("|   Infix Expression:   ");
        yellow_color();
        printf("%-60s", infix);
        normal_color();
        printf(" |\n");
        printf("|                                                                                                 |\n");
        printf("|   Postfix Expression:  ");
        blue_color();
        printf("%-60s", postfix);
        normal_color();
        printf(" |\n");
        printf("|                                                                                                 |\n");
        green_color();
        printf("+-------------------------------------------------------------------------------------------------+\n");
        normal_color();
        
        printf("\n");
        green_color();
        printf("  Conversion completed successfully\n");
        normal_color();
        
        /* Perform step-by-step evaluation */
        evaluate_postfix_step_by_step(postfix);
        
        printf("\n");
        yellow_color();
        printf("  Do you want to convert another expression? (y/n): ");
        normal_color();
        continue_choice = getchar();
        while (getchar() != '\n');
        
    } while (continue_choice == 'y' || continue_choice == 'Y');
    
    printf("\n");
    green_color();
    printf("  Thank you for using the calculator. Goodbye!\n");
    normal_color();
    printf("\n");
    
    return 0;
}