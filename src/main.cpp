#include <stdio.h>

#include "tree.h"

#include "akinator.h"

char MANAGER_LABEL[LABEL_LENGTH] = "XZ chto";

int main()
{
    labels_t labels = {};
    LabelsCtor(&labels, START_DATATREE_SIZE);

    tree_t tree = {};
    TreeCtor(&tree, START_DATATREE_SIZE);    

    node_t *manager = TreeAddLeaf(&tree, tree.root_ptr, RIGHT);
    manager->data   = AddToLabels(MANAGER_LABEL, &labels);
    
/*
    node_t *node_1 = InitNewNode(&tree);
    BindNodes(tree.root_ptr, node_1, LEFT);
    node_1->data = AddToLabels("color", &labels);
        TREE_DUMP(&tree);

    node_t *node_2 = InitNewNode(&tree);
    BindNodes(node_1, node_2, RIGHT);
    node_2->data = AddToLabels("red", &labels);
    TREE_DUMP(&tree);

    node_t *node_3 = InitNewNode(&tree);
    BindNodes(tree.root_ptr, node_3, RIGHT);
    node_3->data = AddToLabels("negr", &labels);

*/

    AkinatorRun(&tree, &labels);

    TREE_DUMP(&tree);

    TreeDtor(&tree);
    LabelsDtor(&labels);

    // int a = 1;
    // int b = 2;
    // ptrdiff_t huy = (int) (&b - &a);
    // fprintf(stderr, "PROVERKA:\n a[%llu] b[%llu], &a - &b = %lld\n\n\n", &a, &b, huy);


    // tree_t tree = {};
    // TreeCtor(&tree, 2);

//     tree.root_ptr->data = -666;
//     TreePasteByVal(&tree, 123);
// fprintf(stderr, "\n\t\t\t\tHUILA\n");
//     TreePasteByVal(&tree, 103);
//     TreePasteByVal(&tree, 222);
//     TreePasteByVal(&tree, 111);
//     TreePasteByVal(&tree, 333);
//     TreePasteByVal(&tree, 444);
//     TreePasteByVal(&tree, 52);

    // TreePrint(tree.root_ptr);

    // TreeDtor(&tree);
    return 0;
}