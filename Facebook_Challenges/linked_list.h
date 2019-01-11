#ifndef LINKED_LIST_HEADER
#define LINKED_LIST_HEADER

#include <stdlib.h>                         // malloc()
#include <string.h>                         // memcpy


#ifndef ERRORS
#define ERRORS

#define ENOLIST        -1 // the node refers to null pointers
#define EINVALID_NODE  -2 // the node is a null pointer
#define EBADLIST       -3 // the nodes do not belong to the same linked-list
#define EBADPOS        -4 // the requested position outranges the linked-list
#define EBADARG        -5 // the argument is invalid

#endif


#ifndef ARGS
#define ARGS

#define EMPTY_NODE   0
#define ABSOLUTE_POS 1

#endif


#ifndef DATA_STRUCTURE
#define DATA_STRUCTURE

/** The node data structure contains the address of the previous and next node.
**/
struct node {
        struct node *prev;
        struct node *next;
};

#endif


#ifndef FUNCTIONS
#define FUNCTIONS

/** make_node() dynamically allocates a node structure plus a given size and
    returns the address of that node. The library may also be used to implement
    a custom-made memory allocator, in which case this function is not
    neccessary since the node is to be allocated with a system allocator such as
    brk() or mmap() system calls **/
struct node *make_node(unsigned size);


/** Initializes a linked-list by giving assigning the first node's previous and
    next node to its own adress. Note that one should never be able to overbound
    from a linked-list that's being travelled through. Thus the first node
    references itself as the previous node, and the last node references itself
    as the next node. This allows the linked-list to be safe to go throu by an
    external function since no null/ inaccessible pointer addresses are present
    within it **/
int init_linked_list(struct node *init_node);


/** add_node() adds a node at the end of the linked-list. It searches the end
    node from the given node, thus any node can be referenced **/
int add_node(struct node *ref_node, struct node *node);


/** rem_node() removes the node from the linked list given it's memory address
**/
int rem_node(struct node *to_rem);


/** add_node_at() adds a node at given position from the reference node and
    subsequently pushes the current node at that position to the next position
    in the linked list. Note that ref_node may be the root node or any other
    node in the linked-list. Note that you can ask that the position be absolute
    to that of the root node if it's position isn't known by setting opt to
    ABSOLUTE_POS. **/
int add_node_at(struct node *ref_node, struct node *node, int pos, int opt);


/** rem_node_at removes a node at given position from the reference node
    and subsequently pushes back the next node in list at the position of the
    node that was just deleted from it. Note that ref_node may be the root node
    or any other node in the linked-list. Note that you can ask that the
    position be absolute to that of the root node if it's position isn't known
    by setting opt to ABSOLUTE_POS. **/
int rem_node_at(struct node *ref_node, int pos, int opt);


/**  Goes to the requested position in the linked-list and returns the node's
     address. **/
struct node *goto_pos(struct node *ref_node, int pos, int opt);

/** swap_nodes() swaps the position of two nodes in a linked_list. **/
int swap_nodes(struct node *a, struct node *b);

#endif


#endif
