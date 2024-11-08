#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include "tree.h"

const int START_DATATREE_SIZE   = 200;
const int START_LABELS_CAPACITY = 1000;

const int ANSWER_LENGTH = 50;

const char *const ANSWER_YES_MARK = "yes";
const char *const ANSWER_NO_MARK  = "no";

enum Answer_t
{
    ANSWER_YES,
    ANSWER_NO
};

enum GameStatus_t
{
    GAME_RESUME,
    GAME_EXIT
};

struct labels_t
{
    char   *text;
    size_t  cur_length;
    size_t  capacity;
};

void  AkinatorRun   (tree_t *tree, labels_t *labels);
void  LabelsCtor    (labels_t *labels, int start_capacity);
void  LabelsDtor    (labels_t *labels);
char *AddToLabels   (char *got_str, labels_t *labels);
char *GetInputLabel (node_t *node, labels_t *labels);

GameStatus_t  AskQuestion (node_t *cur_node, tree_t *tree, labels_t *labels);
GameStatus_t  ResumeOrExit();
Answer_t      GetAnswer();

#endif