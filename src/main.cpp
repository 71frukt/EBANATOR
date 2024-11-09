#include <stdio.h>

#include "tree.h"

#include "akinator.h"


int main(const int argc, const char **argv)
{
    labels_t labels = {};
    LabelsCtor(&labels, START_DATATREE_SIZE);

    tree_t tree = {};
    TreeCtor(&tree, START_DATATREE_SIZE);    

    GetTreeFromFile(&tree, &labels, GetSavefileName(argc, argv));

    AkinatorRun(&tree, &labels);

    SaveTreeInFile(&tree, GetSavefileName(argc, argv));

    TREE_DUMP(&tree);

    TreeDtor(&tree);
    LabelsDtor(&labels);

    return 0;
}