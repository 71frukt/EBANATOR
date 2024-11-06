#include <stdio.h>

#include "tree.h"

int main()
{
    tree_t tree = {};
    TreeCtor(&tree, 20);

    tree.root_ptr->data = -666;
    TreePasteByVal(&tree, 123);
    TreePasteByVal(&tree, 103);
    TreePasteByVal(&tree, 222);
    TreePasteByVal(&tree, 111);
    TreePasteByVal(&tree, 333);
    TreePasteByVal(&tree, 444);
    TreePasteByVal(&tree, 52);

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