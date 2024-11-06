#include <stdio.h>
#include <assert.h>

#include "tree.h"
#include "tree_graph.h"

void DrawGraph(tree_t *tree)
{
    TREE_ASSERT(tree);

    FILE *dot_file = fopen(TMP_DOTFILE_NAME, "w");

    fprintf(dot_file, "digraph G{           \n"   
                        // "rankdir = LR;      \n"  
                        "bgcolor = \"%s\";  \n" , BACKGROUND_COLOR);

    InitNodesInDot(tree, dot_file);

    MakeLinksInDot(tree, dot_file);

    fprintf(dot_file, "} \n");

    fclose(dot_file);

    tree->drawn_graphs_num++;
}

void InitNodesInDot(tree_t *tree, FILE *dot_file)
{
    for (int i = 0; i < tree->size; i++)
    {
        node_t *cur_node = &tree->nodes[i];

        char data_val_str  [10];
        VALUE_TO_STR(cur_node->data,  TREE_ELEM_FORMAT, NODE_DATA_POISON, NODE_DATA_POISON_MARK, data_val_str);
        
        const char *left_ptr_str  = (cur_node->left  == NODE_PTR_POISON ? PTR_POISON_MARK : PTR_LEFT_MARK);
        const char *right_ptr_str = (cur_node->right == NODE_PTR_POISON ? PTR_POISON_MARK : PTR_RIGHT_MARK);

        // VALUE_TO_STR(cur_node->left,  PTR_FORMAT,       NODE_PTR_POISON,  PTR_POISON_MARK,       left_ptr_str);
        // VALUE_TO_STR(cur_node->right, PTR_FORMAT,       NODE_PTR_POISON,  PTR_POISON_MARK,       right_ptr_str);

        fprintf(dot_file, "%s [shape = \"record\", label = \"{%s | { <%s> %s | <%s> %s } }\"]\n", 
                    cur_node->name, data_val_str, PTR_LEFT_MARK, left_ptr_str, PTR_RIGHT_MARK, right_ptr_str); 
    }
}

void MakeLinksInDot(tree_t *tree, FILE *dot_file)
{
    TREE_ASSERT(tree);
    assert(dot_file);

    for (int i = 0; i < tree->size; i++)
    {
        node_t *father = &tree->nodes[i];
        node_t *left   = tree->nodes[i].left;
        node_t *right  = tree->nodes[i].right;

        if (left != NODE_PTR_POISON)
            fprintf(dot_file, "%s: <%s> -> %s\n", father->name, PTR_LEFT_MARK, left->name);

        if (right != NODE_PTR_POISON)
            fprintf(dot_file, "%s: <%s> -> %s\n", father->name, PTR_RIGHT_MARK, right->name);
    }

    TREE_ASSERT(tree);
}