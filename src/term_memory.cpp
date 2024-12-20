#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "tree.h"
#include "tree_debug.h"
#include "akinator.h"


TreeFuncStatus SaveTreeInFile(tree_t *tree, const char *dest_file_name)
{
    TREE_ASSERT(tree);
    assert(dest_file_name);
    
    FILE *dest_file = fopen(dest_file_name, "w");
    assert(dest_file);
    
    TreePrintNodeFamily(tree->root_ptr, dest_file);

    fclose(dest_file);

    return TREE_FUNC_OK;
}

TreeFuncStatus TreePrintNodeFamily(node_t *cur_node, FILE *dest_file)
{
    static int nesting = 0;

    char offset[OFFSET_MAX_LEN] = {};
    const char *tab = "   ";

    for (int i = 0; i < nesting; i++)
        strncat(offset, tab, OFFSET_MAX_LEN - 1);

    if (cur_node == NULL)
        return TREE_FUNC_OK;

    // char data_val_str  [LABEL_LENGTH] = {};
    // char left_ptr_str  [LABEL_LENGTH] = {};
    // char right_ptr_str [LABEL_LENGTH] = {};

    // VALUE_TO_STR(cur_node->data,  TREE_ELEM_FORMAT, NODE_DATA_POISON, NODE_DATA_POISON_MARK, data_val_str);
    // VALUE_TO_STR(cur_node->left,  PTR_FORMAT,       NODE_PTR_POISON,  PTR_POISON_MARK,       left_ptr_str);
    // VALUE_TO_STR(cur_node->right, PTR_FORMAT,       NODE_PTR_POISON,  PTR_POISON_MARK,       right_ptr_str);
    
    if (cur_node->data != NODE_DATA_POISON)
        fprintf(dest_file, "%s{ '%" TREE_ELEM_FORMAT "'\n" , offset, cur_node->data);
    else
        fprintf(dest_file, "%s{ '%s'\n", offset, NODE_DATA_POISON_MARK);

    if (cur_node->left != NODE_PTR_POISON)
    {
        nesting++;
        TreePrintNodeFamily(cur_node->left, dest_file); 
    }

    else
        fprintf(dest_file, "%s%s{ '%s' }\n", tab, offset, PTR_POISON_MARK);

    if (cur_node->right != NODE_PTR_POISON)
    {
        nesting++;
        TreePrintNodeFamily(cur_node->right, dest_file);
    }

    else 
        fprintf(dest_file, "%s%s{ '%s' }\n", tab, offset, PTR_POISON_MARK);

    fprintf(dest_file, "%s}\n", offset);

    nesting--;
    return TREE_FUNC_OK;
}

const char *GetSavefileName(const int argc, const char **argv)
{
    if (argc < 2)
        return BASE_SAVE_FILE_NAME;

    else
        return argv[1];
}

TreeFuncStatus GetTreeFromFile(tree_t *tree, labels_t *labels, const char *input_file_name)
{
    TREE_ASSERT(tree);
    assert(input_file_name);
    
    FILE *input_file = fopen(input_file_name, "r");
    assert(input_file);

    node_t *shadow_node = tree->node_ptrs[0];

    char label[LABEL_LENGTH] = {};

    if (fscanf(input_file, "%s", label) != 1)
    {
        tree->root_ptr       = TreeAddLeaf(tree, shadow_node, ROOTPTR_OWN_DIR);
        tree->root_ptr->data = AddToLabels(MANAGER_LABEL, labels);
    }

    else
    {
        fseek(input_file, 0, SEEK_SET);
        tree->root_ptr = TreeGetNodeFamily(shadow_node, ROOTPTR_OWN_DIR, tree, labels, input_file);      // ������� � �������� ����
    }

    fclose(input_file);

    TREE_DUMP(tree);

    return TREE_FUNC_OK;   
}

node_t *TreeGetNodeFamily(node_t *father_node, SonDir_t son_dir, tree_t *tree, labels_t *labels, FILE *input_file)
{
    assert(father_node);
    assert(input_file);
    TREE_ASSERT(tree);

    char cur_str[LABEL_LENGTH + 6] = {};                               // �������� �� �������, ������� � �������� ������ �� �����

    fscanf(input_file, "%[^\n]%*c", cur_str);

fprintf(stderr, "cur_str: %s\n", cur_str);

    char node_label[LABEL_LENGTH] = {};  
    sscanf(cur_str, " { ' %[a-z A-Z #][^']", node_label);

    if (strcmp(node_label, PTR_POISON_MARK) == 0)     // ����� ��� 'PTR#'
        return NODE_PTR_POISON;

 // new node

    node_t *son_node = InitNewNode(tree);
    son_node->data   = AddToLabels(node_label, labels);

    BindNodes(father_node, son_node, son_dir);

    son_node->left  = TreeGetNodeFamily(son_node, LEFT,  tree, labels, input_file);
    son_node->right = TreeGetNodeFamily(son_node, RIGHT, tree, labels, input_file);

    TREE_ASSERT(tree);

    fscanf(input_file, "%[^\n]%*c", cur_str);                               // ��������� ����������� �������� ������

    return son_node;
}