#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

/*
   Print List
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

    // Fill the linked list
    node = dlist_head(&list);
    for (i = 10; i > 0; i--) {

        if ((data = (int *)malloc(sizeof(int))) == NULL)
            return 1;

        *data = i;

        if (dlist_ins_prev(&list, dlist_head(&list), data) != 0)
            return 1;
    }

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
