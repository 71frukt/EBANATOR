#ifndef TREE_H
#define TREE_H

#include <stdio.h>

#define TREE_DEBUG

#ifdef TREE_DEBUG
#define ON_TREE_DEBUG(...)  __VA_ARGS__
#else
#define ON_TREE_DEBUG(...)
#endif

typedef char* TreeElem_t;
#define TREE_ELEM_FORMAT "s"
#define PTR_FORMAT       "p"

const int NODE_NAME_LEN = 20;
const int LABEL_LENGTH  = 50;

const int OFFSET_MAX_LEN = 50;

const int TREE_ALLOC_MARKS_NUM = 20;         // �������� ����������� labels (START_DATATREE_SIZE)^20  ������� ����� 2^20 = 1 048 576


enum SonDir_t
{
    LEFT,
    RIGHT
};

struct node_t
{
    TreeElem_t data;

    node_t   *left;
    node_t   *right;

    node_t   *father;
    SonDir_t  own_dir;

    ON_TREE_DEBUG(char name[NODE_NAME_LEN]);
};

struct tree_alloc_marks_t
{
    node_t *data[TREE_ALLOC_MARKS_NUM];
    int   size;
};

struct tree_t
{
    node_t **node_ptrs;
    node_t *root_ptr;

    int capacity;
    int size;

    tree_alloc_marks_t alloc_marks;
};

enum TreeFuncStatus
{
    TREE_FUNC_OK,
    TREE_FUNC_ERR
};

const SonDir_t ROOTPTR_OWN_DIR = LEFT; 

const TreeElem_t NODE_DATA_POISON = NULL;
node_t *const    NODE_PTR_POISON  = NULL;

const char *const NODE_DATA_POISON_MARK = "DATA#";
const char *const PTR_POISON_MARK       = "PTR#";
const char *const PTR_LEFT_MARK         = "no";
const char *const PTR_RIGHT_MARK        = "yes";

const char *const BASE_SAVE_FILE_NAME   = "save_file.txt";

TreeFuncStatus  TreeCtor            (tree_t *tree, int start_capacity);
TreeFuncStatus  TreeDtor            (tree_t *tree);
node_t         *InitNewNode         (tree_t *tree);
TreeFuncStatus  TreeReacalloc       (tree_t *tree, int new_capacity);
TreeFuncStatus  BindNodes           (node_t *pregnant, node_t *embryo, SonDir_t son_dir);
node_t         *TreeAddLeaf         (tree_t *tree, node_t *father, SonDir_t dir);
node_t         *TreePasteBetween    (tree_t *tree, node_t *pregnant, node_t *grandson, SonDir_t grandson_oun_dir);
node_t         *TreePasteByVal      (tree_t *tree, TreeElem_t val);


#define VALUE_TO_STR(val, val_type_specifier, poison_val, poison_mark, res_str)             \
{                                                                                           \
    if (val == poison_val)                                                                  \
        sprintf(res_str, "%s", poison_mark);                                                \
    else                                                                                    \
        sprintf(res_str, "%" val_type_specifier, val);                                      \
}

#endif