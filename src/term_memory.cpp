#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "tree.h"

TreeFuncStatus SaveTreeInFile(tree_t *tree, const char *dest_file_name)
{
    TREE_ASSERT(tree);
    assert(dest_file_name);
    
    FILE *dest_file = fopen(dest_file_name, "w");
    assert(dest_file);
    
    TreePrint(tree->root_ptr, dest_file);

    fclose(dest_file);

    return TREE_FUNC_OK;
}

TreeFuncStatus TreePrint(node_t *node, FILE *dest_file)
{
    static int nesting = 0;

    char offset[OFFSET_MAX_LEN] = {};
    const char *tab = "   ";

    for (int i = 0; i < nesting; i++)
        strncat(offset, tab, OFFSET_MAX_LEN - 1);

    if (node == NULL)
        return TREE_FUNC_OK;

    // char data_val_str  [LABEL_LENGTH] = {};
    // char left_ptr_str  [LABEL_LENGTH] = {};
    // char right_ptr_str [LABEL_LENGTH] = {};

    // VALUE_TO_STR(node->data,  TREE_ELEM_FORMAT, NODE_DATA_POISON, NODE_DATA_POISON_MARK, data_val_str);
    // VALUE_TO_STR(node->left,  PTR_FORMAT,       NODE_PTR_POISON,  PTR_POISON_MARK,       left_ptr_str);
    // VALUE_TO_STR(node->right, PTR_FORMAT,       NODE_PTR_POISON,  PTR_POISON_MARK,       right_ptr_str);
    
    if (node->data != NODE_DATA_POISON)
        fprintf(dest_file, "%s{ %" TREE_ELEM_FORMAT "\n" , offset, node->data);
    else
        fprintf(dest_file, "%s{ %s\n", offset, NODE_DATA_POISON_MARK);

    if (node->left != NODE_PTR_POISON)
    {
        nesting++;
        TreePrint(node->left, dest_file); 
    }

    else
        fprintf(dest_file, "%s%s{ %s }\n", tab, offset, PTR_POISON_MARK);

    if (node->right != NODE_PTR_POISON)
    {
        nesting++;
        TreePrint(node->right, dest_file);
    }

    else 
        fprintf(dest_file, "%s%s{ %s }\n", tab, offset, PTR_POISON_MARK);

    fprintf(dest_file, "%s}\n", offset);

    nesting--;
    return TREE_FUNC_OK;
}

const char *GetSaveFileName(const int argc, const char **argv)
{
    if (argc < 2)
        return BASE_SAVE_FILE_NAME;

    else
        return argv[1];
}