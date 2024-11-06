#include <stdio.h>
#include <assert.h>

#include "tree.h"
#include "tree_debug.h"

void TreeAssert(tree_t *tree, const char *file, int line, const char *func)
{
    int error = TreeVerify(tree);

    if (tree != NULL)
        tree->error |= error;

    if (error != TREE_OK)
    {
        fprintf(stderr, "my assertion failed in\t%s:%d\t(%s)\nErrors:\t", file, line, func);
        PrintTreeErr(error);
        assert(0);
    }
}

int TreeVerify(tree_t *tree)
{
    int res_err = 0;

    if (tree == NULL)
        return TREE_PTR_ERR;

    if (tree->nodes == NULL)
        res_err |= TREE_NODES_PTR_ERR;

    if (tree->capacity <= 0)
        res_err |= TREE_CAPA_UNDERFLOW;

    if (tree->size <= 0)
        res_err |= TREE_SIZE_UNDERFLOW;

    if (tree->size >= tree->capacity)
        res_err |= TREE_SIZE_OVERFLOW;

    if (tree->root_ptr == NULL)
        res_err |= TREE_ROOT_PTR_ERR;

    return res_err;
}

void PrintTreeErr(int error)
{   
    #define PRINT_ERROR(err, code)                      \
    if (err & code)                                     \
    {                                                   \
        fprintf(stderr, #code);                         \
        fprintf(stderr, " ");                           \
    }                                                   
    
    PRINT_ERROR (error, TREE_PTR_ERR);
    PRINT_ERROR (error, TREE_NODES_PTR_ERR);
    PRINT_ERROR (error, TREE_CAPA_UNDERFLOW);
    PRINT_ERROR (error, TREE_SIZE_OVERFLOW);
    PRINT_ERROR (error, TREE_SIZE_UNDERFLOW);
    PRINT_ERROR (error, TREE_ROOT_PTR_ERR);

    #undef PRINT_ERROR  

    printf("\n");
}

char *GetFilePath(const char *name, const char *folder, char *path)
{
    sprintf(path, "%s%s", folder, name);
    return path;
}