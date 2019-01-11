#include "tree.h"
#include <stdio.h>

#define NODE_EMPTY       0


int main(int argc, char *argv[])
{
        /*
         * Our root node.
         */
        struct t_node *root_node = make_t_node(NODE_EMPTY);

        /*
         * Child to the root node.
         */
        struct t_node *child[10];

        /*
         * Grandchildren of the root node.
         */
        struct t_node *sub_child[10*10];

        int k = 0;
        for(int i = 0; i < 10; i++)
        {
                child[i] = make_t_node(0);
                link_t_node(root_node, child[i]);

                /*
                 * We assign 10 child nodes to each of the root node's children
                 */
                for(int j = 0; j < 10; j++)
                {
                        sub_child[k] = make_t_node(0);
                        link_t_node(child[i], sub_child[k]);
                } k++;
        }

        /*
         * We finally delete the tree, note that you can remove any branch
         * starting from the given node.
         */

        rem_t_branch(root_node);

}
