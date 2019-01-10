#include "tree.h"


struct t_node *make_t_node(unsigned size)
{
        struct t_node *new_node,
                      *err;

        new_node = malloc(sizeof(struct t_node) + size);

        if((signed long long) new_node < 0)
                return err = new_node;

        /*
         * The node is initialized with null parent, children and siblings.
         */
        new_node->parent       = 0;
        new_node->next_sibling = 0;
        new_node->next_child   = 0;
        new_node->data_size    = size;

        return new_node;
}


int link_t_node(struct t_node *parent, struct t_node *to_link)
{
        struct t_node *child_ptr;
        int            err;

        if(!parent || !to_link)
                return EBAD_ADDRESS;

        /*
         * child_ptr must be checked, if null, our node is the first child and
         * we're done.
         */
        child_ptr = parent->next_child;
        if(child_ptr == (void*) 0)
        {
                parent->next_child = to_link;
                return 0;
        }

        while(child_ptr->next_sibling != (struct t_node*) 0)
                child_ptr = child_ptr->next_sibling;

        child_ptr->next_sibling = to_link;
        to_link->parent         = parent;
        to_link->next_sibling   = (struct t_node*) 0;

        return 0;
}


int unlink_t_node(struct t_node *to_unlink)
{
        struct t_node *child_ptr,
                      *prev_child_ptr;

        if(!to_unlink)
                return EBAD_ADDRESS;

        /*
         * A null parent means the node is root.
         */
        if(!to_unlink->parent)
                return EROOT_NODE;

        /*
         * Pointer to the parent's child list.
         */
        child_ptr = to_unlink->parent->next_child;
        
        /*
         * The current node is retrieved, prev_child_ptr contains the
         * address of the previous node that has been looked at.
         */
        while(child_ptr != to_unlink)
        {
                prev_child_ptr = child_ptr;
                child_ptr      = child_ptr->next_sibling;
        }

        if(child_ptr->next_sibling)
        {
                prev_child_ptr->next_sibling = child_ptr->next_sibling;
        } else {
                prev_child_ptr->next_sibling = (struct t_node*) 0;
        }

        /*
         * Only the parent and siblings are unlinked in case the node and it's
         * children are to be reused, linked to another tree or dysallocated.
         */
        to_unlink->parent       = (struct t_node*) 0;
        to_unlink->next_sibling = (struct t_node*) 0;

        return 0;
}


int rem_t_branch(struct t_node *branch_root_node)
{
        /*
         * All the nodes to be removed will be referenced by this pointer.
         */
        struct t_node *to_rem;
        /*
         * This pointer references the address of to_rem's sibling.
         */
        struct t_node *sibling;
        /*
         * This pointer references the address of to_rem's parent.
         */
        struct t_node *parent;

        unlink_t_node(branch_root_node);

        /*
         * We initialize our position at the leftmost leaf
         */
        to_rem  = goto_leftmost_leaf(branch_root_node);
        sibling = to_rem->next_sibling;
        parent  = to_rem->parent;

        while(to_rem != branch_root_node)
        {
                /*
                 * The left-most leaf has no child, it can thus be deleted.
                 */
                free(to_rem);

                if(!sibling)
                {
                       /*
                        * If the node has no sibling, the parent becomes the leaf.
                        */
                        to_rem = parent;
                        sibling = to_rem->next_sibling;
                        parent = to_rem->parent;

                } else if(sibling) {
                        /*
                         * If the node does have a sibling, we set our position
                         * to the left-most leaf.
                         */
                        to_rem  = goto_leftmost_leaf(sibling);
                        sibling = to_rem->next_sibling;
                        parent  = to_rem->parent;
                }
                /*
                 * And thus we go on until we reach the initial branch root node
                 * which we finally delete after the loop breaks
                 */
        }

        /*
         * And the branch's enterely deleted.
         */
        free(branch_root_node);

        return 0;
}


struct t_node *goto_leftmost_leaf(struct t_node *branch)
{
        struct t_node *leaf = branch;

        while(leaf->next_child)
                leaf = leaf->next_child;

        return leaf;
}
