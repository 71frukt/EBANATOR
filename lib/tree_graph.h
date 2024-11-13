#ifndef TREE_GRAPH_H
#define TREE_GRAPH_H

#include <stdio.h>

#include "tree.h"

#define TMP_DOTFILE_NAME   "tmp_doc_code.doc"
#define GRAPH_NAME_PREFIX  "graph_"

#define BACKGROUND_COLOR  "#FFF3D1"

const int GRAPH_IMG_WIDTH = 20;     // (%)

void  DrawGraph(tree_t *tree, char *picture_path);
void  InitNodesInDot   (tree_t *tree, FILE *dot_file);
void  MakeLinksInDot   (tree_t *tree, FILE *dot_file);
void  MakeGraphPicture (const char *dotfile_path, const char *picture_path);

#endif