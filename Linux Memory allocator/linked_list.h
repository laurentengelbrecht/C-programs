#ifndef LINKED_LIST_HEADER
#define LINKED_LIST_HEADER

#include "globals.h"

typedef char byte;

typedef struct _node {

        struct _node *prev;
        struct _node *next;

        unsigned int size;

        byte         data[0];

} __attribute__((packed)) node_t;
/* __attribute__((packed)) removes padding from the structure in order to get
   it's real size/ non-padded size :
   https://gcc.gnu.org/onlinedocs/gcc-3.3/gcc/Type-Attributes.html */


typedef struct _list {

        node_t *tail;
        node_t *head;

        unsigned int node_num;

} list_t;


typedef struct _free_list {

        node_t *tail;
        node_t *head;

        unsigned int node_num;

        unsigned int largest_block_size;
        unsigned int largest_block_num;

} free_list_t;



#define NON_MERGEABLE 0x00
#define PREV_AND_CURR 0x01
#define NEXT_AND_CURR 0x02
#define BOTH_AND_CURR 0x03


/* Links a node to a linked-list. Usage reserved for allocated nodes. */
void link_node           (list_t *list, node_t *to_link);

/* Links a node to a linked-list in memory order. Usage reserved for free
   nodes. */
void link_node_in_order  (free_list_t *list, node_t *to_link);

/* Unlinks a node from it's linked-list. Usage reserved for allocated nodes. */
void unlink_node         (list_t *list, node_t *to_unlink);

/* Unlinks a free node from it's linked-list. Usage reserved for free nodes. */
void unlink_free_node    (free_list_t *list, node_t *to_unlink);

/* Updates the block size parameters in the free list. Usage reserved for free
   node lists. */
void update_block_size   (free_list_t *list);

/* Returns the address of the most fitted node. Usage reserved for free node
   searching. */
node_t *get_node_by_size (free_list_t *list, unsigned int size);

/* Checks if the node belongs to the linked-list. Returns 0 on success, -1 on
   failure.*/
int check_if_node        (list_t *list, node_t *to_check);

/* Checks if a node is mergeable, returns which nodes can be merged on success,
   -1 on failure */
int check_if_mergeable   (node_t *to_check);


int merge_nodes          (free_list_t *list, node_t *to_merge, int flags);


#endif
