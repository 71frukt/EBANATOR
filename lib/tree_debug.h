#ifndef TREE_DEBUG_H
#define TREE_DEBUG_H

#define LOGS_FOLDER      "logs/"
#define GRAPH_FOLDER     "graphs/"

#define LOGFILE_NAME     "tree_logfile.html"
#define NODE_NAME_PREFIX "node_"

enum TreeError_t
{
    TREE_OK = 0,

    TREE_PTR_ERR        = 1 << 0,
    TREE_NODES_PTR_ERR  = 1 << 1,
    TREE_CAPA_UNDERFLOW = 1 << 2,
    TREE_SIZE_UNDERFLOW = 1 << 3,
    TREE_SIZE_OVERFLOW  = 1 << 4,
    TREE_ROOT_PTR_ERR   = 1 << 5,
    TREE_NODES_LINK_ERR = 1 << 6
};

#ifdef TREE_DEBUG
#define ON_TREE_DEBUG(...)  __VA_ARGS__
#define TREE_ASSERT(tree)  TreeAssert(tree, __FILE__, __LINE__, __func__)
#define TREE_DUMP(tree)    TreeDump  (tree, __FILE__, __LINE__, __func__)


#else
#define ON_TREE_DEBUG(...)
#define TREE_ASSERT(tree)
#define TREE_DUMP(tree)
#endif

#define VALUE_TO_STR(val, val_type_specifier, poison_val, poison_mark, res_str)             \
{                                                                                           \
    if (val == poison_val)                                                                  \
        sprintf(res_str, "%s", poison_mark);                                                \
    else                                                                                    \
        sprintf(res_str, "%" val_type_specifier, val);                                      \
}

const int PATH_NAME_LEN   = 100;
const int CMD_COMMAND_LEN = 100;


#endif