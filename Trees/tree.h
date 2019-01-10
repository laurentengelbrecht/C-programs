#ifndef TREE_HEADER
#define TREE_HEADER

#include <stdlib.h>                                                  // malloc()
#include <stdio.h>

#define EBAD_ADDRESS    -1
#define EROOT_NODE      -2

/*
 * parent contains the address of the parent node. siblings and children are
 * one-way null-pointer-terminated linked-lists. data_size references the user
 * given size of the node in bytes.
 */
struct t_node {
        struct t_node  *parent;
        struct t_node  *next_sibling;
        struct t_node  *next_child;
        int             data_size;
};


/*
 * Dynamically allocates a tree-type node using malloc() and returns it's
 * address. On error, the malloc() error is returned.
 */
struct t_node *make_t_node(unsigned size);


/*
 * Links the node to the child-list of the given parent. On success, the
 * function returns 0.
 */
int link_t_node(struct t_node *parent, struct t_node *to_link);


/*
 * Unlinks the node from it's parent. On success, the function returns 0.
 */
int unlink_t_node(struct t_node *to_unlink);


/*
 * Unlinks the branch root and dynamically dysallocates all the nodes contained
 * in the branch.
 */
int rem_t_branch(struct t_node *branch_root_node);


/*
 * I use 'leaf' as metaphore for the end of a branch. The function goes to the
 * left-most leaf from the given branch and returns it's address.
 */
struct t_node *goto_leftmost_leaf(struct t_node *branch);


#endif
