#ifndef TREE_H
#define TREE_H

#include <stdio.h>

#define TREE_DEBUG

typedef int TreeElem_t;
#define TREE_ELEM_FORMAT "d"
#define PTR_FORMAT       "p"

#include "tree_debug.h"

const int NODE_NAME_LEN = 20;

struct node_t
{
    TreeElem_t data;

    node_t *left;
    node_t *right;

    ON_TREE_DEBUG(char name[NODE_NAME_LEN]);
};

struct tree_t
{
    node_t *nodes;
    node_t *root_ptr;

    int capacity;
    int size; 

    ON_TREE_DEBUG(
        FILE *logfile;
        int error;
        int drawn_graphs_num;
    );
};

enum TreeFuncStatus
{
    TREE_FUNC_OK,
    TREE_FUNC_ERR
};

enum SonDir_t
{
    LEFT,
    RIGHT
};

const TreeElem_t NODE_DATA_POISON = 0xDA4ADED;
node_t *const    NODE_PTR_POISON  = NULL;

const char *const NODE_DATA_POISON_MARK = "DATA#";
const char *const PTR_POISON_MARK       = "PTR#";
const char *const PTR_LEFT_MARK         = "left";
const char *const PTR_RIGHT_MARK        = "right";


const size_t START_TREE_CAPACITY = 20;

TreeFuncStatus  TreeCtor(tree_t *tree, int start_capacity);
TreeFuncStatus  TreeDtor(tree_t *tree);
node_t         *TreeAddLeaf(tree_t *tree, node_t *father, SonDir_t dir);

// debug
void  TreeAssert   (tree_t *tree, const char *file, int line, const char *func);
int   TreeVerify   (tree_t *tree);
void  TreeDump     (tree_t *tree, const char *file, int line, const char *func);
void  PrintTreeErr (int error);
char *GetFilePath  (const char *name, const char *folder, char *path);

// graph
void  DrawGraph        (tree_t *tree);
void  InitNodesInDot   (tree_t *tree, FILE *dot_file);
void  MakeLinksInDot   (tree_t *tree, FILE *dot_file);
void  MakeGraphPicture (const char *dotfile_path, const char *picture_path);
FILE *OpenLogFile      (const char *logfile_name);

#endif