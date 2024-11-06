#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree.h"
#include "tree_debug.h"
#include "tree_graph.h"

TreeFuncStatus TreeCtor(tree_t *tree, int start_capacity)
{
    assert(tree && "tree == nullptr in TreeCtor");
    assert(start_capacity > 0 && "start_capacity <= 0 in TreeCtor!");

    tree->capacity = start_capacity;
    tree->size     = 1;

    tree->nodes = (node_t *) calloc(tree->capacity, sizeof(node_t));

    tree->root_ptr         = &tree->nodes[0];
    tree->root_ptr->data   = NODE_DATA_POISON;
    tree->root_ptr->left   = NODE_PTR_POISON;
    tree->root_ptr->father = NODE_PTR_POISON;
    ON_TREE_DEBUG (sprintf(tree->root_ptr->name, NODE_NAME_PREFIX"%d", 0));

    ON_TREE_DEBUG (
        tree->logfile = OpenLogFile(LOGFILE_NAME);
        assert(tree->logfile);

        tree->drawn_graphs_num = 0;
        tree->error = TREE_OK;
    );

    TREE_ASSERT(tree);
    TREE_DUMP(tree);

    return TREE_FUNC_OK;
}

TreeFuncStatus TreeDtor(tree_t *tree)
{
    TREE_ASSERT(tree);

    free(tree->nodes);

    tree->capacity = 0;
    tree->size     = 0;

    ON_TREE_DEBUG (
        fprintf(tree->logfile,    "\t\t</pre>     \n"
                                  "\t</body       \n"
                                  "</html>");
        fclose(tree->logfile);

        tree->error = 0;
        tree->drawn_graphs_num = 0;
        remove(TMP_DOTFILE_NAME);
    );

    return TREE_FUNC_OK;
}

node_t *InitNewNode(tree_t *tree)
{
    TREE_ASSERT(tree);

    node_t *new_node = &tree->nodes[tree->size];      // TODO: TreeRecalloc
    new_node->data   = NODE_DATA_POISON;
    new_node->left   = NODE_PTR_POISON;
    new_node->right  = NODE_PTR_POISON;

    ON_TREE_DEBUG(sprintf(new_node->name, NODE_NAME_PREFIX"%d", tree->size));

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
        tree->error |= TREE_NODES_LINK_ERR;
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

node_t *TreePasteBetween(tree_t *tree, node_t *pregnant, node_t *grandson, SonDir_t son_dir)
{
    TREE_ASSERT(tree);
    assert(pregnant);
    assert(grandson);

    node_t *son = InitNewNode(tree);

    BindNodes(pregnant, son, grandson->own_dir);
    BindNodes(son, grandson, son_dir);

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
