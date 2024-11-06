#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree.h"

TreeFuncStatus TreeCtor(tree_t *tree, int start_capacity)
{
    assert(tree && "tree == nullptr in TreeCtor");
    assert(start_capacity > 0 && "start_capacity <= 0 in TreeCtor!");

    tree->capacity = start_capacity;
    tree->size     = 0;

    tree->nodes = (node_t *) calloc(tree->capacity, sizeof(node_t));

    assert(tree->nodes && "fail in calloc of tree->nodes in TreeCtor");

    return TREE_FUNC_OK;
}

TreeFuncStatus TreeDtor(tree_t *tree)
{
    assert(tree && "tree == nullptr in TreeDtor");

    free(tree->nodes);

    tree->capacity = 0;
    tree->size     = 0;

    return TREE_FUNC_OK;
}

