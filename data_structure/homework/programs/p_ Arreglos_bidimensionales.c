/*Programa: Arreglos bidimensionales.

Considere el siguiente arreglo matricial:

| Nombre   | Edad | Calificación |
|----------|------|--------------|
| Alberto  | 22   | 9            |
| Fernando | 19   | 4            |
| ...      | ...  | ...          |
| Brenda   | 34   | 7            |
Realice un programa que acepte una lista de n alumnos, el arreglo deberá ser de nx3, la primera columna deberá tener un espacio de 25 caracteres, la segunda y tercera deberá aceptar números enteros.

El programa debe solicitar al usuario si desea o no agregar más alumnos.
El programa después de capturar los datos, deberá devolver el promedio de edad y de calificación grupal.
El programa deberá imprimir los nombres de los alumnos en sentido inverso al que fueron ingresados o se encuentran almacenados.
Puede ser del tipo char todo el arreglo, solo recuerde hacer el "casteo" para los elementos numéricos.

//424135339 REYES GONZALEZ DIEGO DE JESUS 13/09/25 

//NOTA TODOS LOS PROGRAMAS DEBEN ESTAR ESCRITOS EN INGLÉS Y USAR ARGUMENTOS SI SE REQUIERE EN LAS INSTRUCCIONES.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_STUDENTS 100
#define MAX_NAME 25

int main() {
    // Two-dimensional array of type char [n][3]
    // Column 0: Name (25 characters)
    // Column 1: Age (as string, then convert to integer)
    // Column 2: Score (as string, then convert to float)
    char students[MAX_STUDENTS][3][MAX_NAME];
    
    int numStudents = 0;
    char continueInput;
    char tempInput[50];
    int age;
    float score;
    int validAge, validScore;
    
    printf("=== STUDENT MANAGEMENT SYSTEM ===\n\n");
    
    // Student data capture
    do {
        printf("Entering data for student #%d:\n", numStudents + 1);
        
        // Name capture
        printf("Name (max 24 characters): ");
        fgets(students[numStudents][0], MAX_NAME, stdin);
        // Remove newline if exists
        if (students[numStudents][0][strlen(students[numStudents][0])-1] == '\n') {
            students[numStudents][0][strlen(students[numStudents][0])-1] = '\0';
        }
        
        // Age validation and capture (1-100)
        do {
            validAge = 1;
            printf("Age (1-100): ");
            fgets(tempInput, sizeof(tempInput), stdin);
            
            age = atoi(tempInput);
            
            if (age < 1 || age > 100) {
                printf("Error: Age must be between 1 and 100. Try again.\n");
                validAge = 0;
            }
        } while (!validAge);
        
        // Convert age to string and store
        sprintf(students[numStudents][1], "%d", age);
        
        // Score validation and capture (0-10 with 2 decimals)
        do {
            validScore = 1;
            printf("Score (0.00-10.00): ");
            fgets(tempInput, sizeof(tempInput), stdin);
            
            score = atof(tempInput);
            
            if (score < 0.0 || score > 10.0) {
                printf("Error: Score must be between 0.00 and 10.00. Try again.\n");
                validScore = 0;
            }
        } while (!validScore);
        
        // Convert score to string and store with 2 decimals
        sprintf(students[numStudents][2], "%.2f", score);
        
        numStudents++;
        
        // Ask to continue
        if (numStudents < MAX_STUDENTS) {
            printf("\nAdd another student? (y/n): ");
            scanf(" %c", &continueInput);
            getchar(); // Clear buffer
            printf("\n");
        } else {
            printf("Maximum student limit reached (%d).\n", MAX_STUDENTS);
            continueInput = 'n';
        }
        
    } while ((continueInput == 'y' || continueInput == 'Y') && numStudents < MAX_STUDENTS);
    
    // Calculate averages
    int sumAges = 0;
    float sumScores = 0.0;
    int i;
    for (i = 0; i < numStudents; i++) {
        sumAges += atoi(students[i][1]);
        sumScores += atof(students[i][2]);
    }
    
    int averageAge = (int)round((float)sumAges / numStudents);
    float averageScore = sumScores / numStudents;
    
    // Show results
    printf("\n=== RESULTS ===\n");
    printf("Total students: %d\n", numStudents);
    printf("Average age: %d years\n", averageAge);
    printf("Average score: %.2f\n", averageScore);
    
    // Show students in reverse order in the table
    printf("\n=== STUDENT LIST (REVERSE ORDER) ===\n");
    printf("%-25s %-5s %-12s\n", "Name", "Age", "Score");
    printf("%-25s %-5s %-12s\n", "-------------------------", "-----", "------------");
    
    for (i = numStudents - 1; i >= 0; i--) {
        printf("%-25s %-5s %-12s\n", 
               students[i][0], 
               students[i][1], 
               students[i][2]);
    }
    
    printf("\nProgram finished successfully.\n");
    
    return 0;
}
