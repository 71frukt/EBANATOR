#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include "tree.h"

const int START_DATATREE_SIZE   = 200;
const int START_LABELS_CAPACITY = 1000;

const int LABEL_LENGTH = 50;

struct labels_t
{
    char   *text;
    size_t  cur_length;
    size_t  capacity;
};

void  AkinatorRun ();
void  LabelsCtor  (labels_t *labels, int start_capacity);
void  LabelsDtor  (labels_t *labels);
char *AddToLabels (char *got_str, labels_t *labels);
void  AskQuestion (node_t *node);

#endif