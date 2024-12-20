#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "tree.h"
#include "tree_debug.h"
#include "tree_graph.h"

extern int TreeError;

TreeFuncStatus TreeCtor(tree_t *tree, int start_capacity)
{
    assert(tree && "tree == nullptr in TreeCtor");
    assert(start_capacity > 0 && "start_capacity <= 0 in TreeCtor!");

    tree->capacity = 0;
    tree->size     = 0;

    TreeReacalloc(tree, start_capacity);

    InitNewNode(tree);          // init shadow_node

    TREE_ASSERT(tree);
    TREE_DUMP(tree);

    return TREE_FUNC_OK;
}

TreeFuncStatus TreeDtor(tree_t *tree)
{
    TREE_ASSERT(tree);

    for (int i = 0; i < tree->alloc_marks.size; i++)
        free(tree->alloc_marks.data[i]);

    free(tree->node_ptrs);

    tree->capacity = 0;
    tree->size     = 0;

    ON_TREE_DEBUG ( remove(TMP_DOTFILE_NAME) );

    return TREE_FUNC_OK;
}

node_t *InitNewNode(tree_t *tree)
{
    TREE_ASSERT(tree);

    if (tree->size >= tree->capacity)
    {
        int new_capacity = tree->capacity * 2;
        TreeReacalloc(tree, new_capacity);
    }

    node_t *new_node = tree->node_ptrs[tree->size];
    assert(new_node);
    new_node->data   = NODE_DATA_POISON;
    new_node->left   = NODE_PTR_POISON;
    new_node->right  = NODE_PTR_POISON;

    ON_TREE_DEBUG(sprintf(new_node->name, NODE_NAME_PREFIX"%d", tree->size));

// fprintf(stderr, "new_node->name = '%s'\n", new_node->name);

    tree->size++;

    return new_node;

    TREE_ASSERT(tree);
}

node_t *TreeAddLeaf(tree_t *tree, node_t *father, SonDir_t son_dir)
{
    TREE_ASSERT(tree);
    assert(father && "father == NULL in TreeAddLeaf()");
    
    node_t *son = InitNewNode(tree);

    BindNodes(father, son, son_dir);

    if ((son_dir == LEFT  && father->left  != NODE_PTR_POISON) 
     && (son_dir == RIGHT && father->right != NODE_PTR_POISON))
    {
        fprintf(stderr, "ERROR: attempt to bind a leaf to a occupied vertex\n");
        ON_TREE_DEBUG (TreeError |= TREE_NODES_LINK_ERR);
        son = NODE_PTR_POISON;
    }

    TREE_ASSERT(tree);
    TREE_DUMP(tree);

    return son;   
}

node_t *TreePasteByVal(tree_t *tree, TreeElem_t val)
{
    TREE_ASSERT(tree);

    node_t *cur_node = tree->root_ptr;
    node_t *res_node = NULL;

    while (true)
    {
        assert(cur_node->data != NODE_DATA_POISON && "comparison with the poison value in TreePasteByVal()");

        if (val < cur_node->data)
        {
            if (cur_node->left == NODE_PTR_POISON)
            {
                res_node = TreeAddLeaf(tree, cur_node, LEFT);
                res_node->data = val;

                break;
            }

            else
                cur_node = cur_node->left;
        }

        else 
        {
            if (cur_node->right == NODE_PTR_POISON)
            {
                res_node = TreeAddLeaf(tree, cur_node, RIGHT);
                res_node->data = val;

                break;
            }

            else
                cur_node = cur_node->right;
        }
    }

    TREE_ASSERT(tree);
    TREE_DUMP(tree);

    return res_node;
}

node_t *TreePasteBetween(tree_t *tree, node_t *pregnant, node_t *grandson, SonDir_t grandson_oun_dir)
{
    TREE_ASSERT(tree);
    assert(pregnant);
    assert(grandson);

    node_t *son = InitNewNode(tree);

    BindNodes(pregnant, son, grandson->own_dir);
    BindNodes(son, grandson, grandson_oun_dir);

    if (grandson == tree->root_ptr)
        tree->root_ptr = son;

    TREE_ASSERT(tree);
    TREE_DUMP(tree);

    return son;
}

TreeFuncStatus BindNodes(node_t *pregnant, node_t *embryo, SonDir_t son_dir)
{
    assert(pregnant);
    assert(embryo);

    embryo->father  = pregnant;
    embryo->own_dir = son_dir;

    if (son_dir == LEFT)
        pregnant->left = embryo;
    
    else
        pregnant->right = embryo;

    return TREE_FUNC_OK;
}

TreeFuncStatus TreeReacalloc(tree_t *tree, int new_capacity)
{   
    assert(tree);
    assert(new_capacity > 0);

    int prev_capacity = tree->capacity;
    tree->capacity    = new_capacity;

    tree->node_ptrs   = (node_t **) realloc(tree->node_ptrs, new_capacity * sizeof(node_t *));
    node_t *new_nodes = (node_t *)   calloc(new_capacity - prev_capacity,   sizeof(node_t));

    tree->alloc_marks.data[tree->alloc_marks.size++] = new_nodes;

    assert(tree->node_ptrs);
    assert(new_nodes);

    for (int i = 0; i < new_capacity - prev_capacity; i++)
        tree->node_ptrs[prev_capacity + i] = new_nodes + i;

    TREE_ASSERT(tree);

    return TREE_FUNC_OK;
}