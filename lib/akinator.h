#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include "tree.h"

const int START_DATATREE_SIZE   = 200;
const int START_LABELS_CAPACITY = 1000;

const int LABEL_LENGTH  = 50;
const int ANSWER_LENGTH = 10;

const char *const RIGHT_DIR_ANSWER = "yes";
const char *const LEFT_DIR_ANSWER  = "no";

struct labels_t
{
    char   *text;
    size_t  cur_length;
    size_t  capacity;
};

void      AkinatorRun   (tree_t *tree, labels_t *labels);
void      LabelsCtor    (labels_t *labels, int start_capacity);
void      LabelsDtor    (labels_t *labels);
char     *AddToLabels   (char *got_str, labels_t *labels);
void      AskQuestion   (node_t *cur_node, tree_t *tree, labels_t *labels);
char     *GetInputLabel (node_t *node, labels_t *labels);
SonDir_t  GetSonDirByAnswer();

#endif