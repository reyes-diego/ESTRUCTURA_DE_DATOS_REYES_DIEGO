#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

/*
   Print List - Función auxiliar para imprimir la lista
*/

static void print_list (const DList *list) {
    DListNode *node;
    int *data, i;

    fprintf(stdout, "DList size is %d\n", dlist_size(list));

    i = 0;
    node = dlist_head(list);

    while (1) {
        data = dlist_data(node);
        fprintf(stdout, "dlist.node[%03d]=%03d, %14p <- %p -> %p \n", i, *data, node->prev, node, node->next);

        i++;

        if (dlist_is_tail(node))
            break;
        else
            node = dlist_next(node);
    }

   return;
}


int main (int argc, char **argv) {
    DList list;
    DListNode *node;

    int *data, i;

    // Initialize the linked list
    dlist_init(&list, free);

    // ============================================================
    //  CARGA DE DATOS DESDE LÍNEA DE COMANDOS Passing Arguments to an Application Understanding and
    // Using C Pointers p 125
    // ============================================================
    
    // Verifica que se hayan pasado argumentos (argc >= 2 porque argv[0] es el nombre del programa)
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <valor1> <valor2> ...\n", argv[0]); //para mensajes de error es una buena práctica que permite un mejor manejo de la salida del programa
        return 1;
    }

    // Recorre todos los argumentos pasados por línea de comandos
    // i = 1 porque argv[0] es el nombre del programa, argv[1] es el primer argumento
    for (i = 1; i < argc; i++) {
        // Asigna memoria para el nuevo entero
        if ((data = (int *)malloc(sizeof(int))) == NULL)
            return 1;
        
        // Convierte el argumento de texto a entero usando atoi= Función que convierte una cadena de texto a un número entero.()
        *data = atoi(argv[i]);
        
        // Inserta el nuevo valor al final de la lista
        // dlist_tail(&list) obtiene el último nodo, dlist_ins_next inserta después de él
        if (dlist_ins_next(&list, dlist_tail(&list), data) != 0)
            return 1;
    }

    // ============================================================
    // CÓDIGO ORIGINAL ELIMINADO - PRECARGA DE VALORES FIJOS
    // ============================================================
    /*
    // Este bloque original insertaba valores del 10 al 1 al principio de la lista
    node = dlist_head(&list);
    for (i = 10; i > 0; i--) {
        if ((data = (int *)malloc(sizeof(int))) == NULL)
            return 1;
        *data = i;
        if (dlist_ins_prev(&list, dlist_head(&list), data) != 0)
            return 1;
    }
    */

    // ============================================================
    // A PARTIR DE AQUÍ NO SE MODIFICÓ NADA - CÓDIGO ORIGINAL
    // ============================================================

    print_list(&list);

    node = dlist_head(&list);

    for (i = 0; i < 7; ++i)
        node = dlist_next(node);

    data = dlist_data(node);
    fprintf(stdout, "\nRemoving a node after the one containing %03d\n", *data);

    if (dlist_remove(&list, node, (void**)&data) != 0)
        return 1;

    print_list(&list);

    fprintf(stdout, "\nInserting 187 at the tail of the list\n");
    *data = 187;
    if (dlist_ins_next(&list, dlist_tail(&list), data) != 0)
        return 1;
    print_list(&list);

    fprintf(stdout, "\nRemoving a node at the tail of the list\n");
    if (dlist_remove(&list, dlist_tail(&list), (void**)&data) != 0)
        return 1;
    print_list(&list);

    fprintf(stdout, "\nInsert 975 before the tail of the list\n");
    *data = 975;
    if (dlist_ins_prev(&list, dlist_tail(&list), data) != 0)
        return 1;
    print_list(&list);    

    fprintf(stdout, "\nIterating and removing the fifth node\n");

    node = dlist_head(&list);
    node = dlist_next(node);
    node = dlist_prev(node);
    node = dlist_next(node);
    node = dlist_next(node);
    node = dlist_next(node);
    node = dlist_next(node);
    node = dlist_prev(node);

    if (dlist_remove(&list, node, (void **)&data) != 0)
        return 1;

    print_list(&list);

    fprintf(stdout, "\nInserting 607 before the head node\n");
    *data = 607;
    if (dlist_ins_prev(&list, dlist_head(&list), data) != 0)
        return 1;
    print_list(&list);
    
    fprintf(stdout, "\nRemoving a node at the head of the list\n");
    if (dlist_remove(&list, dlist_head(&list), (void**)&data) != 0)
        return 1;
    print_list(&list);    

    fprintf(stdout, "\nInserting 900 after the head node\n");
    *data = 900;
    if (dlist_ins_next(&list, dlist_head(&list), data) != 0)
        return 1;
    print_list(&list);

    fprintf(stdout, "\nInserting 080 two nodes after the head of the list\n");
    if ((data = (int *)malloc(sizeof(int))) == NULL)
        return 1;
    *data = 80;
    node = dlist_head(&list);
    node = dlist_next(node);
    if (dlist_ins_next(&list, node, data) != 0)
        return 1;

    print_list(&list);


    i = dlist_is_head(dlist_head(&list));
    fprintf(stdout, "\nTesting list_is_head... value=%d (1=OK)\n", i);
    i = dlist_is_head(dlist_tail(&list));
    fprintf(stdout, "Testing list_is_head... value=%d (1=OK)\n", i);
    i = dlist_is_tail(dlist_tail(&list));
    fprintf(stdout, "Testing list_is_tail... value=%d (1=OK)\n", i);
    i = dlist_is_tail(dlist_head(&list));
    fprintf(stdout, "Testing list_is_tail... value=%d (1=OK)\n", i);
    

    // Destroying the list
    fprintf(stdout, "\nDestroying the list\n");
    dlist_destroy(&list);

    return 0;
}
