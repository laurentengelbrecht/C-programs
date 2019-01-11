#include "linked_list.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
        struct node *node[10];

        /*
         * Initialize our linked-list
         */
        node[0] = make_node(EMPTY_NODE);
        init_linked_list(node[0]);

        for(int i = 1; i < 10; i++)
        {
                node[i] = make_node(EMPTY_NODE);
                add_node(node[i-1], node[i]);
                printf("node %d : %p\n", i, node[i]);

        }

        write(1, "\n", 1);

        /*
         * Swap two choosen nodes.
         */
        int a = swap_nodes(node[2], node[5]);

        struct node *read_list = node[0];


        for(int i = 0; i < 10; i++)
        {
                printf("node %d : %p\n", i, read_list);
                read_list = read_list->next;
        }

        printf("\nhere you see node 2 and 5 have been swapped\n");

}
