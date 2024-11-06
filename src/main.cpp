#include <stdio.h>

#include "tree.h"

int main()
{
    tree_t tree = {};
    TreeCtor(&tree, 20);

    node_t *node_1 = TreeAddLeaf(&tree, tree.root_ptr, RIGHT);
    node_t *node_2 = TreeAddLeaf(&tree, node_1,        RIGHT);
    node_t *node_3 = TreeAddLeaf(&tree, node_2,        RIGHT);

    node_1->data = 123;
    node_2->data = 456;
    node_3->data = 789;

    TreePasteBetween(&tree, node_2, node_3, LEFT);

/*
    node_t *node_4 = TreeAddLeaf(&tree, node_1, LEFT);
    node_t *node_5 = TreeAddLeaf(&tree, node_2, LEFT);
    node_t *node_6 = TreeAddLeaf(&tree, node_3, LEFT);

    TreeAddLeaf(&tree, node_3, RIGHT);

    TreeAddLeaf(&tree, node_4, LEFT);
    TreeAddLeaf(&tree, node_5, LEFT);
    TreeAddLeaf(&tree, node_6, LEFT);
    TreeAddLeaf(&tree, node_4, RIGHT);
    TreeAddLeaf(&tree, node_5, RIGHT);
    TreeAddLeaf(&tree, node_6, RIGHT);
*/

    TreeDtor(&tree);

    return 0;
}