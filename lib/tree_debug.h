#ifndef TREE_DEBUG_H
#define TREE_DEBUG_H

enum TreeError_t
{
    TREE_OK = 0,

    TREE_PTR_ERR        = 1 << 0,
    TREE_NODES_PTR_ERR  = 1 << 1,
    TREE_CAPA_UNDERFLOW = 1 << 2,
    TREE_SIZE_UNDERFLOW = 1 << 3,
    TREE_SIZE_OVERFLOW  = 1 << 4
};

#ifdef TREE_DEBUG
#define ON_TREE_DEBUG(...)  __VA_ARGS__
#define LIST_ASSERT(list)  ListAssert(list, __FILE__, __LINE__, __func__)

#else
#define ON_TREE_DEBUG(...)
#define LIST_ASSERT(list)
#endif

#endif