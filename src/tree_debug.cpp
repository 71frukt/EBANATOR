
#include <stdio.h>
#include <assert.h>

#include "tree.h"
#include "tree_debug.h"
#include "tree_graph.h"

FILE* LogFile   = OpenLogFile(); 
int   TreeError = TREE_OK;

void TreeAssert(tree_t *tree, const char *file, int line, const char *func)
{
    int error = TreeVerify(tree);

    if (tree != NULL)
        TreeError |= error;

    if (TreeError != TREE_OK)
    {
        fprintf(stderr, "my assertion failed in\t%s:%d\t(%s)\nErrors:\t", file, line, func);
        PrintTreeErr(TreeError);
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

    static int drawn_graphs_num = 0;

    fprintf(LogFile, "   TREE_DUMP called from %s:%d  (%s)\n  {\n", file, line, func);

    char picture_path[PATH_NAME_LEN] = {};
    char picture_name[PATH_NAME_LEN] = {};
    sprintf(picture_name, "%s%d.png", GRAPH_NAME_PREFIX, drawn_graphs_num);

    GetFilePath(picture_name, LOGS_FOLDER GRAPH_FOLDER, picture_path);

    DrawGraph(tree, picture_path);

    fprintf(LogFile, "<img src = %s%s%d.png width = \"%d%%\" style=\"margin-left: 3%%\">\n", GRAPH_FOLDER, GRAPH_NAME_PREFIX, drawn_graphs_num, GRAPH_IMG_WIDTH);
    // fprintf(stderr,  "<img src = %s%s%d.png width = \"%d%%\" style=\"margin-left: 3%%\">\n", GRAPH_FOLDER, GRAPH_NAME_PREFIX, drawn_graphs_num, GRAPH_IMG_WIDTH);

    fprintf(LogFile, "\n  }\n\n");

    drawn_graphs_num++;
}

FILE *OpenLogFile()
{
    char logfile_path[PATH_NAME_LEN] = {};
    GetFilePath(LOGFILE_NAME, LOGS_FOLDER, logfile_path);

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

void CloseLogFile()
{
    fprintf(LogFile,    "\t\t</pre>     \n"
                        "\t</body       \n"
                        "</html>");
    fclose(LogFile);
}