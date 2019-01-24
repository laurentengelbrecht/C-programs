#include "linked_list.h"


void link_node        (list_t *list, node_t *to_link)
{
        if (list->node_num > 1)
        {
                list->head->next = to_link;
                to_link->prev    = list->head;
                list->head       = to_link;
                to_link->next    = NULL;

        } else if (list->node_num == 1) {

                list->tail->next = to_link;
                list->head       = to_link;

                to_link->prev    = list->tail;
                to_link->next    = NULL;

        } else { /* The list is empty. */

                list->tail       = to_link;
                list->head       = to_link;

                to_link->prev    = NULL;
                to_link->next    = NULL;
        }

        list->node_num++;

        return;
}


void link_node_in_order  (free_list_t *list, node_t *to_link)
{
        node_t *curr;

        /* Linking nodes in memory order facilitates merging. */
        if (list->node_num > 1)
        {
                curr = list->tail;

                while (curr->next != NULL)
                {
                        if(curr->next > to_link)
                                break;

                        curr = curr->next;
                }

                if (to_link < curr)
                {
                        to_link->prev    = NULL;
                        list->tail       = NULL;
                } else {
                        to_link->prev    = curr;
                        to_link->next    = curr->next;
                }

                if (curr->next != NULL)
                {
                        curr->next->prev = to_link;
                        list->head       = to_link;
                } else {
                        to_link->next    = NULL;
                        curr->next       = to_link;
                }

        } else if (list->node_num == 1) {

                if (list->head > to_link)
                {
                        list->tail       = to_link;
                        to_link->prev    = NULL;
                        list->head->prev = to_link;
                        to_link->next    = list->head;

                } else if (list->tail < to_link) {

                        list->head       = to_link;
                        to_link->prev    = list->tail;
                        list->tail->next = to_link;
                        to_link->next    = NULL;
                }

        } else {

                list->tail = to_link;
                list->head = to_link;

                to_link->prev = NULL;
                to_link->next = NULL;
        }

        /* Remember we keep track of the largest blocks. */
        if (to_link->size == list->largest_block_size)
        {
                list->largest_block_num++;

        } else if (to_link->size > list->largest_block_size) {

                list->largest_block_size = to_link->size;
                list->largest_block_num  = 1;
        }

        list->node_num++;

        return;
}


void unlink_node         (list_t *list, node_t *to_unlink)
{
        if (list->node_num > 1)
        {
                if (to_unlink == list->tail)
                {
                        to_unlink->next->prev = NULL;
                        list->tail            = to_unlink->next;

                } else if (to_unlink == list->head) {

                        to_unlink->prev->next = NULL;
                        list->head            = to_unlink->prev;
                } else {
                        to_unlink->prev->next = to_unlink->next;
                        to_unlink->next->prev = to_unlink->prev;
                }

        } else if (list->node_num == 1) {

              list->tail      = NULL;
              list->head      = NULL;
        }

        list->node_num--;

        return;
}


void unlink_free_node    (free_list_t *list, node_t *to_unlink)
{
        if (list->node_num > 1)
        {
                if (to_unlink->prev != NULL)
                        to_unlink->prev->next = to_unlink->next;

                if (to_unlink->next != NULL)
                        to_unlink->next->prev = to_unlink->prev;

        } else if (list->node_num == 1) {

              list->tail      = NULL;
              list->head      = NULL;
        }

        if (to_unlink->size == list->largest_block_size)
        {
                if (list->largest_block_num == 1)
                { /* Our block was the last largest. The size attributes need to
                     be updated. */
                        update_block_size(list);

                } else if (list->largest_block_num > 1) {

                        list->largest_block_num--;

                } else {
                        list->largest_block_size = 0;
                        list->largest_block_num  = 0;
                }
        }

        list->node_num--;

        return;
}


void update_block_size(free_list_t *list)
{
        node_t *curr = list->tail;

        while (curr != list->head)
        {
                if (curr->size == list->largest_block_size)
                {
                        list->largest_block_num++;

                } else if (curr->size > list->largest_block_size) {

                        list->largest_block_size = curr->size;
                        list->largest_block_num  = 1;
                }

                curr = curr->next;
        }

        return;
}


node_t *get_node_by_size (free_list_t *list, unsigned int size)
{
        node_t *curr = list->tail;
        node_t *best = curr;

        while (curr != list->head)
        {
                if (curr->size > size && curr->size < best->size)
                {
                        best = curr;

                } else if (curr->size == size) { /* Perfect node. */

                        best = curr;
                        break;
                }

                curr = curr->next;
        }

        return best;
}


int check_if_node        (list_t *list, node_t *to_check)
{
        node_t *curr;

        if (list->tail == NULL && list->head == NULL)
                return 0;

        if (to_check == list->tail && to_check == list->head)
                return 1;

        curr = list->tail;

        while (curr != NULL)
        {
                if (curr == to_check)
                        return 1;

                curr = curr->next;
        }

        return 0;
}


int check_if_mergeable   (node_t *to_check)
{
        int prev_curr_size = 0;
        int next_curr_size = 0;
        int flags          = NON_MERGEABLE;

        if (to_check->prev->size + sizeof(node_t) == (uint64_t) to_check)
        {
                prev_curr_size = to_check->prev->size +
                                 to_check->size       +
                                 sizeof(node_t) * 2;

                if (prev_curr_size <= mem_opt.max_heap_node_size)
                        flags |= PREV_AND_CURR;
        }

        if (to_check->size + sizeof(node_t) == (uint64_t) to_check->next)
        {
                next_curr_size = to_check->next->size +
                                 to_check->size       +
                                 sizeof(node_t) * 2;

                if(next_curr_size <= mem_opt.max_heap_node_size)
                        flags |= NEXT_AND_CURR;
        }

        if (flags == BOTH_AND_CURR)
        {
                if ((prev_curr_size + to_check->next->size +
                     sizeof(node_t)) > mem_opt.max_heap_node_size)
                { /* If merging would result in a too large node. */
                        if (prev_curr_size == next_curr_size)
                        { /* Arbitrary I admit. */
                                flags ^= NEXT_AND_CURR;

                        } else { /* The largest merging is favored. */
                                flags  = PREV_AND_CURR;
                                flags  = (next_curr_size > prev_curr_size) ?
                                         (next_curr_size < prev_curr_size) :
                                         NEXT_AND_CURR;
                        }
                } else;
        }

        return flags;
}


int merge_nodes          (free_list_t *list, node_t *to_merge, int flags)
{
        node_t *prev;
        node_t *next;

        switch (flags) {

                case PREV_AND_CURR :

                        prev = to_merge->prev;
                        unlink_free_node(list, to_merge);

                        prev->size += to_merge->size + sizeof(node_t);

                        /* We perform an update size that way instead of running
                           update_block_size() that would needlessly consume CPU
                           and time. */
                        if (prev->size > list->largest_block_size)
                        {
                                list->largest_block_size = prev->size;
                                list->largest_block_num  = 1;

                        } else if (prev->size == list->largest_block_size) {

                                list->largest_block_num++;
                        }

                        break;


                case NEXT_AND_CURR :

                        next = to_merge->prev;

                        unlink_free_node(list, next);

                        to_merge->size += next->size + sizeof(node_t);

                        if (to_merge->size > list->largest_block_size)
                        {
                                list->largest_block_size = to_merge->size;
                                list->largest_block_num  = 1;

                        } else if (to_merge->size == list->largest_block_size) {

                                list->largest_block_num++;
                        }

                        break;


                case BOTH_AND_CURR :

                        prev = to_merge->prev;
                        next = to_merge->next;

                        unlink_free_node(list, to_merge);
                        unlink_free_node(list, next);

                        prev->size += to_merge->size +
                                      next->size     +
                                     (sizeof(node_t) * 2);

                        if (prev->size > list->largest_block_size)
                        {
                               list->largest_block_size = prev->size;
                               list->largest_block_num  = 1;

                        } else if (prev->size == list->largest_block_size) {

                               list->largest_block_num++;
                        }

                        break;


                default : /* We should not get there unless the user messes with
                             the library. */
                        return -1;
        }

        return flags;

}
