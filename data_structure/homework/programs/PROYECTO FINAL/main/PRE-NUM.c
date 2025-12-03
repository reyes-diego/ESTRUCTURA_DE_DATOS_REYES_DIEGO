//PRE-NUM Portable Version
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// #include <windows.h> // Eliminado
#include <math.h>  // Added for mathematical operations
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

/*
    Restore original color
*/
void color_normal() {
    printf(RESET_COLOR);
}

/*
    Set blue color
*/
void color_blue() {
    printf(COLOR_BLUE);
}

/*
    Set green color
*/
void color_green() {
    printf(COLOR_GREEN);
}

void color_yellow() {
    printf(COLOR_YELLOW);
}

/*
    Set red color
*/
void color_red() {
    printf(COLOR_RED);
}

/*
    Determine operator precedence
*/
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;  /* Exponent */
    if (op == 's') return 4;  /* sqrt (square root) */
    return 0;
}

/*
    Check if character is operator
*/
int is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == 's');
}

/*
    NEW FUNCTION: Validate infix expression syntax
    Returns 1 if expression is valid, 0 if it has errors
    MODIFIED: Supports multi-digit numbers
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

    /* Check for empty expression */
    if (len == 0) {
        color_red();
        printf("\n  ERROR: The expression is empty\n");
        color_normal();
        return 0;
    }

    for (i = 0; i < len; i++) {
        c = infix[i];

        /* Ignore spaces */
        if (c == ' ') {
            inside_number = 0;
            continue;
        }

        /* Check for invalid characters */
        if (!isdigit(c) && !isalpha(c) && !is_operator(c) && c != '(' && c != ')') {
            color_red();
            printf("\n  ERROR: Invalid character '%c' at position %d\n", c, i + 1);
            color_normal();
            return 0;
        }

        /* Check parentheses */
        if (c == '(') {
            parenthesis_balance++;
            /* Cannot have operand before ( without operator */
            if (last_was_operand && !inside_number) {
                color_red();
                printf("\n  ERROR: Missing operator before parenthesis '(' at position %d\n", i + 1);
                color_normal();
                return 0;
            }
            last_was_operator = 0;
            last_was_operand = 0;
            inside_number = 0;
        }
        else if (c == ')') {
            parenthesis_balance--;
            /* Check for negative balance */
            if (parenthesis_balance < 0) {
                color_red();
                printf("\n  ERROR: Closing parenthesis ')' without opening at position %d\n", i + 1);
                color_normal();
                return 0;
            }
            /* Cannot have operator before ) */
            if (last_was_operator) {
                color_red();
                printf("\n  ERROR: Missing operand before parenthesis ')' at position %d\n", i + 1);
                color_normal();
                return 0;
            }
            /* Cannot have ( followed by ) */
            if (last_char == '(') {
                color_red();
                printf("\n  ERROR: Empty parentheses '()' at position %d\n", i + 1);
                color_normal();
                return 0;
            }
            last_was_operator = 0;
            last_was_operand = 1;
            inside_number = 0;
        }
        /* If it's a digit (could be part of multi-digit number) */
        else if (isdigit(c)) {
            /* If we were already in a number, continue */
            if (inside_number) {
                /* This is part of the same number, do nothing special */
            }
            /* If we weren't in a number, check there's no previous operand */
            else {
                if (last_was_operand) {
                    color_red();
                    printf("\n  ERROR: Missing operator between numbers at position %d\n", i + 1);
                    color_normal();
                    return 0;
                }
                last_was_operand = 1;
                inside_number = 1;
            }
            last_was_operator = 0;
        }
        /* If it's a letter */
        else if (isalpha(c)) {
            if (last_was_operand && !inside_number) {
                color_red();
                printf("\n  ERROR: Missing operator between operands at position %d\n", i + 1);
                color_normal();
                return 0;
            }
            last_was_operand = 1;
            last_was_operator = 0;
            inside_number = 0;
        }
        /* If it's an operator */
        else if (is_operator(c)) {
            /* Cannot have two consecutive operators */
            if (last_was_operator) {
                color_red();
                printf("\n  ERROR: Two consecutive operators at position %d\n", i + 1);
                color_normal();
                return 0;
            }
            /* Cannot start with operator */
            if (i == 0) {
                color_red();
                printf("\n  ERROR: Expression cannot start with an operator\n");
                color_normal();
                return 0;
            }
            /* Cannot have operator after ( */
            if (last_char == '(') {
                color_red();
                printf("\n  ERROR: Operator '%c' after parenthesis '(' at position %d\n", c, i + 1);
                color_normal();
                return 0;
            }
            last_was_operator = 1;
            last_was_operand = 0;
            inside_number = 0;
        }

        last_char = c;
    }

    /* Check final parenthesis balance */
    if (parenthesis_balance > 0) {
        color_red();
        printf("\n  ERROR: Missing %d closing parenthesis(es) ')'\n", parenthesis_balance);
        color_normal();
        return 0;
    }

    /* Cannot end with operator */
    if (last_was_operator) {
        color_red();
        printf("\n  ERROR: Expression cannot end with an operator\n");
        color_normal();
        return 0;
    }

    /* If we reached here, syntax is valid */
    color_green();
    printf("\n  Syntax is valid\n");
    color_normal();
    return 1;
}

/*
    Print stack content from RIGHT to LEFT with colors
    Improved version: centered elements and ordered from right to left
    New elements are inserted to the LEFT of the first element
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
        /* When stack is empty, print centered spaces */
        for (i = 0; i < 25; i++) printf(" ");
        return;
    }

    /* Copy elements to temporary stack to preserve original order */
    stack_init(&temp_stack, NULL);

    /* Extract elements from main stack */
    while (stack_size(stack) > 0) {
        stack_pop(stack, (void **)&op_ptr);
        elements[count++] = *op_ptr;  /* Save the character */
        stack_push(&temp_stack, op_ptr);  /* Preserve to restore later */
    }

    /* Restore original stack */
    while (stack_size(&temp_stack) > 0) {
        stack_pop(&temp_stack, (void **)&op_ptr);
        stack_push(stack, op_ptr);
    }

    /* CALCULATE SPACES FOR CENTERING */
    /* Each element occupies 2 spaces (character + space) except the last */
    total_length = (count * 2) - 1;
    spaces = (25 - total_length) / 2;

    /* Ensure spaces is not negative */
    if (spaces < 0) spaces = 0;

    /* Print initial spaces for centering */
    for (i = 0; i < spaces; i++) {
        printf(" ");
    }

    /* Print elements from LEFT to RIGHT (new elements to the left) */
    /* The last entered element (top) is shown to the LEFT */
    for (i = 0; i < count; i++) {
        if (highlight && i == 0 && elements[i] == new_element) {
            color_blue();
            printf("%c", elements[i]);
            color_normal();
        } else {
            printf("%c", elements[i]);
        }

        /* Add space between elements, except after the last */
        if (i < count - 1) printf(" ");
    }

    /* Complete with spaces if needed to maintain alignment */
    int remaining_spaces = 25 - (spaces + total_length);
    for (i = 0; i < remaining_spaces; i++) {
        printf(" ");
    }

    stack_destroy(&temp_stack);
}

/*
    Print expression with last element in blue (from LEFT to RIGHT)
    Improved version: centered elements
*/
void print_colored_operation(const char *operation, int length, int highlight_last) {
    int i;
    int spaces;
    int total_length;

    if (length == 0) {
        for (i = 0; i < 29; i++) printf(" ");
        return;
    }

    /* CALCULATE SPACES FOR CENTERING */
    total_length = (length * 2) - 1;
    spaces = (29 - total_length) / 2;

    /* Ensure spaces is not negative */
    if (spaces < 0) spaces = 0;

    /* Print initial spaces for centering */
    for (i = 0; i < spaces; i++) {
        printf(" ");
    }

    for (i = 0; i < length; i++) {
        if (highlight_last && i == length - 1) {
            color_blue();
            printf("%c", operation[i]);
            color_normal();
        } else {
            printf("%c", operation[i]);
        }
        if (i < length - 1) printf(" ");
    }

    /* Complete with spaces if needed to maintain alignment */
    int remaining_spaces = 29 - (spaces + total_length);
    for (i = 0; i < remaining_spaces; i++) {
        printf(" ");
    }
}

/*
    Convert infix to prefix WITH PROFESSIONAL TABLE
    TRAVERSAL FROM RIGHT to LEFT (last element first)
    STACK is filled from RIGHT to LEFT
    OPERATION is filled from LEFT to RIGHT
    MODIFIED: Adds spaces between COMPLETE operands (not between digits of same number)
*/
void infix_to_prefix(const char *infix, char *prefix) {
    Stack stack;
    int i;
    int j = 0;
    int step = 1;
    char c;
    char *op_ptr;
    char temp_operation[MAX_EXPR];
    int length;
    int last_was_digit = 0;  /* Track if last character was a digit */
    int needs_space = 0;

    /* Initialize stack */
    stack_init(&stack, free);

    printf("\n");
    color_green();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|                          CONVERSION FROM INFIX TO PREFIX                                        |\n");
    printf("|                                 STEP BY STEP ALGORITHM                                          |\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
    color_normal();

    printf("\n");
    color_yellow();
    printf("  Entered Infix Expression: ");
    color_blue();
    printf("%s\n", infix);
    color_normal();

    printf("  Traversal Method: ");
    color_green();
    printf("RIGHT -> LEFT");
    color_normal();
    printf(" (last element first)\n\n");

    printf("+-----------------------------------------------------------------------------------------------------------------+\n");
    printf("|       |                          |                         |                             |\n");
    printf("|  STEP |         ACTION           |       STACK (R -> L)    |       OPERATION (L -> R)    |\n");
    printf("|       |                          |                         |                             |\n");
    printf("|-------+--------------------------+-------------------------+-----------------------------|\n");

    temp_operation[0] = '\0';
    length = strlen(infix);

    /* TRAVERSE FROM RIGHT to LEFT (last element first) */
    for (i = length - 1; i >= 0; i--) {
        c = infix[i];

        /* Ignore spaces */
        if (c == ' ')
            continue;

        printf("|  %3d  | ", step);

        /* If it's a digit (part of a number) */
        if (isdigit(c)) {
            /* If space needed before this complete number */
            if (needs_space && !last_was_digit) {
                temp_operation[j++] = ' ';
                needs_space = 0;
            }
            temp_operation[j++] = c;
            temp_operation[j] = '\0';
            last_was_digit = 1;
            printf("ADD [%c]         ", c);
            printf("|    ");
            print_stack(&stack, '\0', 0);
            printf(" | ");
            print_colored_operation(temp_operation, j, 1);
            printf(" |\n");
        }
        /* If it's a letter */
        else if (isalpha(c)) {
            /* Add space if needed */
            if (needs_space) {
                temp_operation[j++] = ' ';
                needs_space = 0;
            }
            temp_operation[j++] = c;
            temp_operation[j] = '\0';
            last_was_digit = 0;
            needs_space = 1;  /* Next operand will need space */
            printf("ADD [%c]         ", c);
            printf("|    ");
            print_stack(&stack, '\0', 0);
            printf(" | ");
            print_colored_operation(temp_operation, j, 1);
            printf(" |\n");
        }
        /* If it's RIGHT parenthesis - PUSH to stack */
        else if (c == ')') {
            op_ptr = (char *)malloc(sizeof(char));
            *op_ptr = c;
            stack_push(&stack, op_ptr);
            /* If there was a number before, mark that next needs space */
            if (last_was_digit) {
                needs_space = 1;
            }
            last_was_digit = 0;
            printf("PUSH [%c]            ", c);
            printf("|    ");
            print_stack(&stack, c, 1);
            printf(" | ");
            print_colored_operation(temp_operation, j, 0);
            printf(" |\n");
        }
        /* If it's LEFT parenthesis - POP until finding ) */
        else if (c == '(') {
            printf("FOUND [%c]       ", c);
            printf("|    ");
            print_stack(&stack, '\0', 0);
            printf(" | ");
            print_colored_operation(temp_operation, j, 0);
            printf(" |\n");

            printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
            step++;

            /* POP until finding the right parenthesis ) */
            while (stack_size(&stack) > 0) {
                stack_pop(&stack, (void **)&op_ptr);

                if (*op_ptr == ')') {
                    free(op_ptr);
                    printf("|  %3d  | POP [)]             ", step);
                    printf("|    ");
                    print_stack(&stack, '\0', 0);
                    printf(" | ");
                    print_colored_operation(temp_operation, j, 0);
                    printf(" |\n");
                    break;
                } else {
                    /* Add space before operator if needed */
                    if (needs_space || last_was_digit) {
                        temp_operation[j++] = ' ';
                        needs_space = 0;
                    }
                    temp_operation[j++] = *op_ptr;
                    temp_operation[j] = '\0';
                    last_was_digit = 0;
                    needs_space = 1;
                    printf("|  %3d  | POP [%c] (find ')') ", step, *op_ptr);
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
            /* POP operators of HIGHER OR EQUAL hierarchy */
            while (stack_size(&stack) > 0) {
                char *top_op;
                top_op = (char *)stack_peek(&stack);

                if (top_op && *top_op != ')' && precedence(*top_op) >= precedence(c)) {
                    stack_pop(&stack, (void **)&op_ptr);
                    /* Add space before operator if needed */
                    if (needs_space || last_was_digit) {
                        temp_operation[j++] = ' ';
                        needs_space = 0;
                    }
                    temp_operation[j++] = *op_ptr;
                    temp_operation[j] = '\0';
                    last_was_digit = 0;
                    needs_space = 1;
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
            }

            /* PUSH current operator */
            op_ptr = (char *)malloc(sizeof(char));
            *op_ptr = c;
            stack_push(&stack, op_ptr);
            /* If there was a number before, mark that next needs space */
            if (last_was_digit) {
                needs_space = 1;
            }
            last_was_digit = 0;
            printf("PUSH [%c]            ", c);
            printf("|    ");
            print_stack(&stack, c, 1);
            printf(" | ");
            print_colored_operation(temp_operation, j, 0);
            printf(" |\n");
        }

        if (i > 0) {
            printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
        }
        step++;
    }

    /* Separator before emptying stack */
    if (stack_size(&stack) > 0) {
        printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
        color_yellow();
        printf("|       |     EMPTYING STACK       |                         |                             |\n");
        color_normal();
        printf("|-------+--------------------------+-------------------------+-----------------------------|\n");
    }

    /* POP all remaining operators to empty the STACK */
    while (stack_size(&stack) > 0) {
        stack_pop(&stack, (void **)&op_ptr);
        /* Add space before operator if needed */
        if (needs_space || last_was_digit) {
            temp_operation[j++] = ' ';
            needs_space = 0;
        }
        temp_operation[j++] = *op_ptr;
        temp_operation[j] = '\0';
        last_was_digit = 0;
        needs_space = 1;
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

    temp_operation[j] = '\0';

    /* INVERT the result using DLIST (read from tail to head) */
    DList list;
    dlist_init(&list, NULL);

    /* Insert each character at the end of the list */
    for (i = 0; i < j; i++) {
        char *character = (char *)malloc(sizeof(char));
        *character = temp_operation[i];
        dlist_ins_next(&list, dlist_tail(&list), character);
    }

    /* Read from tail to head to invert */
    int k = 0;
    DListNode *node = dlist_tail(&list);

    while (node != NULL) {
        prefix[k++] = *(char *)dlist_data(node);
        node = dlist_prev(node);
    }
    prefix[k] = '\0';

    /* Destroy list and free memory */
    dlist_destroy(&list);

    printf("\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
    color_yellow();
    printf("|                           FINAL STEP: INVERT RESULT                                           |\n");
    color_normal();
    printf("|-------------------------------------------------------------------------------------------------|\n");
    printf("|                                                                                                 |\n");
    printf("|   Before inversion:   %-60s |\n", temp_operation);
    printf("|                                                                                                 |\n");
    printf("|   After inversion:    ");
    color_blue();
    printf("%-58s", prefix);
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
    Helper function to extract a complete number from a position
    Returns the length of the found number
*/
int extract_number(const char *expression, int start, char *number) {
    int j = 0;
    int i = start;
    /* Skip initial spaces */
    while (expression[i] == ' ') {
        i++;
    }
    /* Extract digits */
    while (expression[i] != '\0' && isdigit(expression[i])) {
        number[j++] = expression[i++];
    }
    number[j] = '\0';
    return i - start;  /* Returns how many characters were advanced (includes spaces) */
}

/*
    Helper function to calculate powers (exponentiation)
    Calculates base^exponent
*/
int power(int base, int exponent) {
    int result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

/*
    Function to evaluate prefix expression through traversals
    Identifies binary operations of the form: operator number(s) number(s)
    SEARCH FROM LEFT TO RIGHT to find the first complete operation
    For example: in "+ / 18 ^ 3 2 ...", first finds "^ 3 2"
    Supports multi-digit numbers
    MODIFIED: Performs mathematical operations correctly
*/
void evaluate_prefix(const char *prefix) {
    char expression[MAX_EXPR];
    char result[MAX_EXPR];
    int i, j, k;
    int step = 1;
    int new_num = 99;  /* Number that will be used to replace operations (99, 98, 97...) */
    int changes = 1;
    int length;
    char num1[MAX_EXPR], num2[MAX_EXPR];
    int advance_num1, advance_num2;
    int pos_after_num1, pos_after_num2;
    char operation_str[MAX_EXPR];
    char new_num_str[20];
    int operation_found;
    int pos_num1, pos_num2;

    strcpy(expression, prefix);

    printf("\n");
    color_green();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|                          PREFIX EXPRESSION VERIFICATION                                        |\n");
    printf("|                              STEP BY STEP EVALUATION                                           |\n");
    printf("+-------------------------------------------------------------------------------------------------+\n");
    color_normal();

    printf("\n");
    color_yellow();
    printf("  Prefix Expression to Verify: ");
    color_blue();
    printf("%s\n", expression);
    color_normal();

    printf("  Method: Search for binary operations of the form: ");
    color_green();
    printf("operator number(s) number(s)");
    color_normal();
    printf("\n");
    printf("  Note: Multi-digit numbers are recognized (ex: 123, 45, 7)\n");
    printf("  Search: ");
    color_green();
    printf("LEFT -> RIGHT");
    color_normal();
    printf(" (first complete operation)\n");

    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|  STEP |         OPERATION FOUND              |         RESULTING EXPRESSION                  |\n");
    printf("|-------------------------------------------------------------------------------------------------|\n");

    /* Iterate until no more changes */
    while (changes) {
        changes = 0;
        length = strlen(expression);
        operation_found = 0;

        /* SEARCH FROM LEFT TO RIGHT for the first complete operation */
        for (i = 0; i < length && !operation_found; i++) {
            /* Skip spaces */
            if (expression[i] == ' ') continue;

            /* If we find an operator, search FORWARD for two numbers */
            if (is_operator(expression[i])) {
                int pos_op = i;
                pos_num1 = i + 1;
                /* Skip spaces after operator */
                while (pos_num1 < length && expression[pos_num1] == ' ') {
                    pos_num1++;
                }
                /* Check if there's a number after the operator */
                if (pos_num1 < length && isdigit(expression[pos_num1])) {
                    /* Extract first number */
                    advance_num1 = extract_number(expression, pos_num1, num1);
                    pos_after_num1 = pos_num1 + advance_num1;
                    /* Skip spaces between numbers */
                    while (pos_after_num1 < length && expression[pos_after_num1] == ' ') {
                        pos_after_num1++;
                    }
                    /* Check if there's a second number (not another operator) */
                    if (pos_after_num1 < length && isdigit(expression[pos_after_num1])) {
                        /* Extract second number */
                        advance_num2 = extract_number(expression, pos_after_num1, num2);
                        pos_after_num2 = pos_after_num1 + advance_num2;

                        /* Calculate the real result of the operation */
                        int a = atoi(num1);
                        int b = atoi(num2);
                        int operation_result;

                        /* Perform the corresponding operation */
                        switch (expression[pos_op]) {
                            case '+':
                                operation_result = a + b;
                                break;
                            case '-':
                                operation_result = a - b;
                                break;
                            case '*':
                                operation_result = a * b;
                                break;
                            case '/':
                                if (b != 0) {
                                    operation_result = a / b;
                                } else {
                                    operation_result = 0;  /* Division by zero */
                                }
                                break;
                            case '^':
                                operation_result = power(a, b);
                                break;
                            default:
                                operation_result = 0;
                        }

                        /* Build the operation string for display */
                        sprintf(operation_str, "%c%s%s", expression[pos_op], num1, num2);
                        sprintf(new_num_str, "%d", operation_result);

                        /* Print the found operation */
                        printf("|  %3d  |   ", step);
                        color_blue();
                        printf("%s", operation_str);
                        color_normal();
                        printf(" = ");
                        color_green();
                        printf("%s", new_num_str);
                        color_normal();

                        /* Spaces for alignment */
                        int op_spaces = 30 - strlen(operation_str) - strlen(new_num_str);
                        for (k = 0; k < op_spaces; k++) printf(" ");

                        printf("|   ");

                        /* Build the new expression */
                        j = 0;
                        /* Copy until before the operator */
                        for (k = 0; k < pos_op; k++) {
                            result[j++] = expression[k];
                        }
                        /* Insert the new number */
                        for (k = 0; k < strlen(new_num_str); k++) {
                            result[j++] = new_num_str[k];
                        }
                        /* Copy the rest of the expression (after the second number) */
                        for (k = pos_after_num2; k < length; k++) {
                            result[j++] = expression[k];
                        }
                        result[j] = '\0';

                        /* Print the resulting expression with the new number highlighted */
                        int pos_new = pos_op;
                        int len_new = strlen(new_num_str);
                        for (k = 0; k < strlen(result); k++) {
                            if (k >= pos_new && k < pos_new + len_new) {
                                if (k == pos_new) color_green();
                                printf("%c", result[k]);
                                if (k == pos_new + len_new - 1) color_normal();
                            } else {
                                printf("%c", result[k]);
                            }
                        }

                        /* Complete spaces */
                        int spaces = 45 - strlen(result);
                        for (k = 0; k < spaces; k++) printf(" ");

                        printf(" |\n");

                        /* Update the expression */
                        strcpy(expression, result);
                        new_num--;  /* Next number (99, 98, 97, ...) */
                        changes = 1;
                        operation_found = 1;
                        step++;

                        /* Check if there are still operators */
                        int has_operators = 0;
                        for (k = 0; k < strlen(expression); k++) {
                            if (is_operator(expression[k])) {
                                has_operators = 1;
                                break;
                            }
                        }
                        if (has_operators) {
                            printf("|-------------------------------------------------------------------------------------------------|\n");
                        }
                    }
                }
            }
        }
    }

    printf("+-------------------------------------------------------------------------------------------------+\n");

    printf("\n");
    color_green();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    printf("|                                FINAL VERIFICATION RESULT                                        |\n");
    printf("|-------------------------------------------------------------------------------------------------|\n");
    color_normal();
    printf("|                                                                                                 |\n");
    printf("|   Original Expression:  ");
    color_yellow();
    printf("%-60s", prefix);
    color_normal();
    printf(" |\n");
    printf("|                                                                                                 |\n");
    printf("|   Final Result:         ");
    color_blue();
    printf("%-60s", expression);
    color_normal();
    printf(" |\n");
    printf("|                                                                                                 |\n");

    /* Check if only a number remains (could be multi-digit) */
    int only_number = 1;
    int has_digit = 0;
    for (i = 0; i < strlen(expression); i++) {
        if (isdigit(expression[i])) {
            has_digit = 1;
        } else if (expression[i] != ' ') {
            only_number = 0;
            break;
        }
    }

    if (only_number && has_digit) {
        color_green();
        printf("|   Status: SUCCESSFUL VERIFICATION - Expression reduced to a single number                     |\n");
        color_normal();
    } else {
        color_red();
        printf("|   Status: INCONCLUSIVE VERIFICATION - Expression not completely reduced                      |\n");
        color_normal();
    }

    printf("|                                                                                                 |\n");
    color_green();
    printf("+-------------------------------------------------------------------------------------------------+\n");
    color_normal();
}

/*
    Main
*/
int main(void) {
    char infix[MAX_EXPR];
    char prefix[MAX_EXPR];
    char continue_char;

    // init_colors(); // Quitamos la inicialización de Windows

    do {
        clear_screen();  /* Clear screen on each iteration - Portable version */
        // system("cls");  /* Clear screen on each iteration - Quitamos dependencia de windows */

        printf("\n\n");
        color_green();
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
        printf("|  2. s           Square roots                                                                     |\n");
        printf("|  3. ^           Exponents                                                                      |\n");
        printf("|  4. * /         Multiplication and Division                                                     |\n");
        printf("|  5. + -         Addition and Subtraction                                                       |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");

        printf("\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("| ALGORITHM TO CONVERT FROM INFIX TO PREFIX:                                                     |\n");
        printf("+-------------------------------------------------------------------------------------------------|\n");
        printf("|                                                                                                 |\n");
        printf("|   Read from RIGHT to LEFT (last element first)                                                 |\n");
        printf("|   The infix operation is NOT inverted (kept original)                                          |\n");
        printf("|   STACK COLUMN: Filled from RIGHT to LEFT                                                     |\n");
        printf("|   OPERATION COLUMN: Filled from LEFT to RIGHT                                                 |\n");
        printf("|                                                                                                 |\n");
        printf("|  POP IS DONE WHEN:                                                                              |\n");
        printf("|     Parentheses close: ( )                                                                    |\n");
        printf("|     PUSH to operation of LOWER OR EQUAL hierarchy                                             |\n");
        printf("|     No more elements to add (empty the STACK)                                                 |\n");
        printf("|                                                                                                 |\n");
        printf("|  FINAL STEP: COMPLETE result is INVERTED                                                       |\n");
        printf("|                                                                                                 |\n");
        printf("|  NOTE: New elements appear in ");
        color_blue();
        printf("BLUE COLOR");
        color_normal();
        printf("                                                          |\n");
        printf("|                                                                                                 |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");

        printf("\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("| CONVERSION EXAMPLES:                                                                           |\n");
        printf("+-------------------------------------------------------------------------------------------------|\n");
        printf("|   a*b+(c^2-d)     ->    +*ab-^c2d                                                             |\n");
        printf("|   (a+b)*c         ->    *+abc                                                                 |\n");
        printf("|   a+b*c           ->    +a*bc                                                                 |\n");
        printf("+-------------------------------------------------------------------------------------------------+\n");

        printf("\n");
        color_yellow();
        printf("  -> Enter the infix expression (without spaces): ");
        color_normal();
        fgets(infix, MAX_EXPR, stdin);

        /* Remove newline */
        infix[strcspn(infix, "\n")] = '\0';

        /* VALIDATE SYNTAX BEFORE CONVERTING */
        if (!validate_syntax(infix)) {
            printf("\n");
            color_red();
            printf("  The expression contains errors. Please correct the syntax.\n");
            color_normal();
            printf("\n");
            color_yellow();
            printf("  Do you want to try with another expression? (y/n): ");
            color_normal();
            continue_char = getchar();
            while (getchar() != '\n');  /* Clear buffer */

            if (continue_char != 'y' && continue_char != 'Y') {
                break;  /* Exit loop if doesn't want to continue */
            }
            continue;  /* Go back to ask for expression */
        }

        /* Convert to prefix */
        infix_to_prefix(infix, prefix);

        printf("\n");
        color_green();
        printf("+-------------------------------------------------------------------------------------------------+\n");
        printf("|                                FINAL RESULT                                                     |\n");
        printf("|-------------------------------------------------------------------------------------------------|\n");
        color_normal();
        printf("|                                                                                                 |\n");
        printf("|   Infix Expression:   ");
        color_yellow();
        printf("%-60s", infix);
        color_normal();
        printf(" |\n");
        printf("|                                                                                                 |\n");
        printf("|   Prefix Expression:  ");
        color_blue();
        printf("%-60s", prefix);
        color_normal();
        printf(" |\n");
        printf("|                                                                                                 |\n");
        color_green();
        printf("+-------------------------------------------------------------------------------------------------+\n");
        color_normal();

        printf("\n");
        color_green();
        printf("  Conversion completed successfully\n");
        color_normal();

        /* Perform verification of the prefix expression */
        evaluate_prefix(prefix);

        printf("\n");
        color_yellow();
        printf("  Do you want to convert another expression? (y/n): ");
        color_normal();
        continue_char = getchar();
        while (getchar() != '\n');  /* Clear buffer */

    } while (continue_char == 'y' || continue_char == 'Y');

    printf("\n");
    color_green();
    printf("  Thank you for using the calculator. Goodbye!\n");
    color_normal();
    printf("\n");

    return 0;
}