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

    TreePrint(tree.root_ptr);

    TreeDtor(&tree);

    return 0;
}