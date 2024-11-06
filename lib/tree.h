#ifndef TREE_H
#define TREE_H

#include <stdio.h>

#define TREE_DEBUG
typedef int TreeElem_t;

#include "tree_debug.h"

struct node_t
{
    TreeElem_t data;

    node_t *left;
    node_t *right;
};

struct tree_t
{
    node_t *nodes;

    int capacity;
    int size; 

    ON_TREE_DEBUG(int error);
};

enum TreeFuncStatus
{
    TREE_FUNC_OK,
    TREE_FUNC_ERR
};

const size_t START_TREE_CAPACITY = 20;

TreeFuncStatus TreeCtor(tree_t *tree, int start_capacity);
TreeFuncStatus TreeDtor(tree_t *tree);

void ListAssert   (tree_t *tree, const char *file, int line, const char *func);
int  TreeVerify   (tree_t *tree);
void PrintListErr (int error);
#endif