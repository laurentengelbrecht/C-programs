#include "linked_list.h"
#include <stdio.h>

struct node *make_node(unsigned size)
{
        struct node *addr;
        int err;

        addr = malloc(sizeof(struct node) + size);
        if(err = addr < 0)
                return (struct node*) (long long) err;

        return addr;
}


int init_linked_list(struct node *init_node)
{
        if(!init_node)
                return EINVALID_NODE;

        init_node->prev = init_node;
        init_node->next = init_node;
}


int add_node(struct node *ref_node, struct node *node)
{
        struct node *curr_node;

        /** we make sure the given reference node is valid **/
        if(!ref_node)
                return EINVALID_NODE;

        else if(!ref_node->prev || !ref_node->next)
                return ENOLIST;

        curr_node = ref_node;

        while(curr_node->next != curr_node)
        {
                curr_node = curr_node->next;
        }

        curr_node->next = node;
        node->prev      = curr_node;
        node->next      = node;

        return 0;
}


int rem_node(struct node *to_rem)
{
        if(!to_rem)
                return EINVALID_NODE;

        else if(to_rem->prev || to_rem->next)
                return ENOLIST;

        if(to_rem->next = to_rem)
        {
                to_rem->prev->next = to_rem->prev;

        } else if(to_rem->prev = to_rem) {

                to_rem->next->prev = to_rem->next;

        } else {
                to_rem->prev->next = to_rem->next;
                to_rem->next->prev = to_rem->prev;
        }

        /** the removed node now references itself and is thus isolated **/
        to_rem->prev = to_rem;
        to_rem->next = to_rem;

        return 0;
}


int add_node_at(struct node *ref_node, struct node *node, int pos, int opt)
{
        struct node *curr_node = ref_node;
        int err;

        if(!ref_node)
                  return EINVALID_NODE;

        else if(!ref_node->prev || !ref_node->next)
                  return ENOLIST;

        curr_node = goto_pos(curr_node, pos, opt);
        if(err = curr_node < 0)
                return err;

        if(curr_node->next == curr_node)
        {
                curr_node->next = node;
                node->prev      = curr_node;
                node->next      = node;

        } else if(curr_node->prev = curr_node) {          /** Unlikely yet possible. **/

                curr_node->prev = node;
                node->next      = curr_node;
                node->prev      = node;
        } else {
                node->next = curr_node->next;
                node->prev = curr_node;
                curr_node->next = node;
                node->next->prev = curr_node;
        }

        return 0;
}


int rem_node_at(struct node *ref_node, int pos, int opt)
{
        struct node *curr_node = ref_node;
        int err;

        if(!ref_node)
                return EINVALID_NODE;

        else if(!ref_node->prev || !ref_node->next)
                return ENOLIST;

        curr_node = goto_pos(curr_node, pos, opt);
        if(err = curr_node < 0)
                return err;

        if(curr_node->next == curr_node)
        {
                curr_node->prev->next = curr_node->prev;

        } else if(curr_node->prev = curr_node) {          /** Unlikely yet possible. **/

                curr_node->next->prev = curr_node->next;

        } else {
                curr_node->prev->next = curr_node->next;
                curr_node->next->prev = curr_node->prev;
        }

        curr_node->prev = curr_node;
        curr_node->next = curr_node;

        return 0;
}


struct node *goto_pos(struct node *curr_node, int pos, int opt)
{
        if(opt == ABSOLUTE_POS)
        {
                struct node *start_point = curr_node;

                int i = 0;
                int relative_pos;

                /** We first place ourselves at the root of the linked-list. **/
                while(curr_node->prev != curr_node)
                {
                        curr_node = curr_node->prev;
                        i++;
                }

                /**  The relative position is calculated so to choose the faster
                way to travel the linked-list to the desired point. **/
                relative_pos = pos - i;

                if(relative_pos < pos)
                {
                        curr_node = start_point;

                        /**  If the relative position is positive the
                             linked-list is travalled onwards, if it is negative
                             it is then travalled throu backwards. **/
                        if(relative_pos > 0)
                        {
                                for(int j = 0; j < i; j++)
                                {
                                        /**  Reaching either the end or the
                                             begining of the list before the
                                             loop finishes means the given
                                             position outranges the size of the
                                             linked-list **/
                                        if(curr_node == curr_node->next)
                                                return (struct node*) EBADPOS;

                                        curr_node = curr_node->next;
                                }

                        } else if(relative_pos < 0) {

                                for(int j = 0; j < i; j++)
                                {
                                        if(curr_node == curr_node->prev)
                                                return (struct node*) EBADPOS;

                                        curr_node = curr_node->prev;
                                }
                        } else;

                  } else if(relative_pos > pos) {

                          for(int j = 0; j < pos; j++)
                          {
                                  if(curr_node == curr_node->next)
                                          return (struct node*) EBADPOS;

                                  curr_node = curr_node->next;
                          }
                  }

        } else {
                for(int i = 0; i < pos; i++)
                {
                        curr_node = curr_node->next;
                }
        }

        return curr_node;
}


int swap_nodes(struct node *a, struct node *b)
{
        struct node *curr_node_a = a;
        struct node *curr_node_b = b;

        struct node a_copy;

        memcpy(&a_copy, a, sizeof(struct node));

        /*
         * We initialize a starting point to compare our current position with
         * our initial position in case the linked_list is circular.
         */
        struct node *start_point_a = a;
        struct node *start_point_b = b;


        if(!a || !b)
                return EINVALID_NODE;

        else if(!a->prev || !a->next || !b->prev || !b->next)
                return ENOLIST;


        /**  We check if the two nodes belong to the same linked-list by moving
             backwards to the root node, if they aren't, EBADLIST is returned.
             If they are in a circular linked-list, the loop checks if one of
             the curr_node pointers meets the initial position of the other
             node. It might seem like a lot of checking but ensures we quickly
             find wether we are or not in the same linked-list regardless of
             whether it is or not circular.
        **/
        do
        {
                curr_node_a = curr_node_a->prev;

        } while(curr_node_a       != start_point_b &&
                curr_node_a->prev != curr_node_a   &&
                curr_node_a       != start_point_a);

        do {
                curr_node_b = curr_node_b->prev;

        } while(curr_node_b       != start_point_a &&
                curr_node_b->prev != curr_node_b   &&
                curr_node_b       != start_point_b);

        /*
         * Final check to see of our nodes are in the same linked-list.
         */
        if((curr_node_a != curr_node_b)   &&
           (curr_node_a != start_point_b) &&
           (curr_node_b != start_point_a))
                return EBADLIST;

        /*
         * The swap is done here.
         */
        a->next->prev = b;
        a->prev->next = b;

        b->next->prev = a;
        b->prev->next = a;

        a->next = b->next;
        a->prev = b->prev;

        b->next = a_copy.next;
        b->prev = a_copy.prev;

        return 0;
}
