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
    
    node_t *son_node = InitNewNode(tree);

    BindNodes(father, son_node, son_dir);

    if ((son_dir == LEFT  && father->left  != NODE_PTR_POISON) 
     && (son_dir == RIGHT && father->right != NODE_PTR_POISON))
    {
        fprintf(stderr, "ERROR: attempt to bind a leaf to a occupied vertex\n");
        tree->error |= TREE_NODES_LINK_ERR;
        son_node = NODE_PTR_POISON;
    }

    TREE_ASSERT(tree);
    TREE_DUMP(tree);

    return son_node;   
}
/*
node_t TreePasteByVal(tree_t *tree, TreeElem_t val)
{
    node_t *cur_node = tree->root_ptr;

    if (val < cur_node->data)
    {
        
    }
}

node_t *TreePasteAfter(tree_t *tree, node_t *pregnant, SonDir_t son_dir, SonDir_t grandson_dir)
{
    node_t *new_node = InitNewNode(tree);
    new_node->father = pregnant;

    if (son_dir == LEFT)
    {

    }
}
*/

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
