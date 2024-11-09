#include <stdio.h>
#include <assert.h>

#include "tree.h"
#include "tree_debug.h"
#include "tree_graph.h"

void TreeAssert(tree_t *tree, const char *file, int line, const char *func)
{
    int error = TreeVerify(tree);

    if (tree != NULL)
        tree->error |= error;

    if (tree->error != TREE_OK)
    {
        fprintf(stderr, "my assertion failed in\t%s:%d\t(%s)\nErrors:\t", file, line, func);
        PrintTreeErr(tree->error);
        assert(0);
    }
}

int TreeVerify(tree_t *tree)
{
    int res_err = 0;

    if (tree == NULL)
        return TREE_PTR_ERR;

    if (tree->node_ptrs == NULL)
        res_err |= TREE_NODES_PTR_ERR;

    for (int i = 0; i < tree->capacity; i++)
        if (tree->node_ptrs[i] == NULL)
            res_err |= TREE_NODES_PTR_ERR;

    if (tree->capacity <= 0)
        res_err |= TREE_CAPA_UNDERFLOW;

    if (tree->size < 0)
        res_err |= TREE_SIZE_UNDERFLOW;

    if (tree->size > tree->capacity)
        res_err |= TREE_SIZE_OVERFLOW;

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
    PRINT_ERROR (error, TREE_NODES_LINK_ERR);

    #undef PRINT_ERROR  

    printf("\n");
}

char *GetFilePath(const char *name, const char *folder, char *path)
{
    sprintf(path, "%s%s", folder, name);
    return path;
}

void TreeDump(tree_t *tree, const char *file, int line, const char *func)
{
    TREE_ASSERT(tree);
    assert(file);
    assert(func);

    FILE *logfile = tree->logfile;

    fprintf(logfile, "   TREE_DUMP called from %s:%d  (%s)\n  {\n", file, line, func);

    DrawGraph(tree);

    fprintf(logfile, "<img src = %s%s%d.png width = \"%d%%\" style=\"margin-left: 3%%\">\n", GRAPH_FOLDER, GRAPH_NAME_PREFIX, tree->drawn_graphs_num - 1, GRAPH_IMG_WIDTH);
    // fprintf(stderr,  "<img src = %s%s%d.png width = \"%d%%\" style=\"margin-left: 3%%\">\n", GRAPH_FOLDER, GRAPH_NAME_PREFIX, tree->drawn_graphs_num - 1, GRAPH_IMG_WIDTH);

    fprintf(logfile, "\n  }\n\n");
}

FILE *OpenLogFile(const char *logfile_name)
{
    assert(logfile_name);

    char logfile_path[PATH_NAME_LEN] = {};
    GetFilePath(logfile_name, LOGS_FOLDER, logfile_path);

    FILE *logfile = fopen(logfile_path, "w");

    fprintf(logfile, "<html>                                                                                                            \n"
                            "\t<head>                                                                                                   \n"
                            "\t<title>Tree Logs</title>                                                                                 \n"
                            "\t<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css\"> \n"
                            "\t</head>                                                                                                  \n"
                            "\t<body>                                                                                                   \n"
                            "\t<title>Tree Logs</title>                                                                                 \n"
                            "\t<div class=\"jumbotron text-center\">                                                                    \n"
                            "\t\t<h1>List logs</h1>                                                                                     \n"
                            "\t</div>                                                                                                   \n"
                            "\t<pre>                                                                                                    \n"
                        );

    return (logfile);
}