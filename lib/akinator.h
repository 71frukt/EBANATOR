#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include "tree.h"

const int START_DATATREE_SIZE = 200;
const int ALLOC_MARKS_NUM     = 20;         // �������� ����������� labels (START_DATATREE_SIZE)^20  ������� ����� 2^20 = 1 048 576

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

struct alloc_marks_t
{
    char *data[ALLOC_MARKS_NUM];
    int   size;
};

struct labels_t
{
    int    size;
    int    capacity;
    
    char **data;

    alloc_marks_t alloc_marks;
};

void          AkinatorRun    (tree_t *tree, labels_t *labels);
void          LabelsCtor     (labels_t *labels, int start_capacity);
void          LabelsDtor     (labels_t *labels);
void          LabelsRecalloc (labels_t *labels, int new_capacity);
char         *AddToLabels    (char *got_str, labels_t *labels);
GameStatus_t  AskQuestion    (node_t *cur_node, tree_t *tree, labels_t *labels);

char         *GetInputLabel  (node_t *node, labels_t *labels);
Answer_t      GetAnswer      ();
GameStatus_t  ResumeOrExit   ();

#endif