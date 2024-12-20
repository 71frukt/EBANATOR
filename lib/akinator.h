#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include "tree.h"
#include "stack.h"

const int START_DATATREE_SIZE = 200;
const int ALLOC_MARKS_NUM     = 20;         // �������� ����������� labels (START_DATATREE_SIZE)^20  ������� ����� 2^20 = 1 048 576

const int ANSWER_LENGTH = 50;

const char *const ANSWER_YES_MARK = "yes";
const char *const ANSWER_NO_MARK  = "no";

const char *const MANAGER_LABEL   = "XZ chto";

const char *const SAVE_FILE_EXTENSION  = ".txt";
const char *const IMAGE_FILE_EXTENSION = ".png";

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

struct labels_alloc_marks_t
{
    char *data[ALLOC_MARKS_NUM];
    int   size;
};

struct labels_t
{
    int    size;
    int    capacity;
    
    char **data;

    labels_alloc_marks_t alloc_marks;
};


void  AkinatorRun    (tree_t *tree, labels_t *labels);
void  LabelsCtor     (labels_t *labels, int start_capacity);
void  LabelsDtor     (labels_t *labels);
void  LabelsRecalloc (labels_t *labels, int new_capacity);
char *AddToLabels    (const char *got_str, labels_t *labels);
void  AskQuestion    (node_t *cur_node, tree_t *tree, labels_t *labels);
void  PrintHeroInfo  (node_t *hero);
void  GuessHero      (node_t *cur_node, tree_t *tree, labels_t *labels, SonDir_t user_answer);

char         *GetInputLabel  (node_t *node, labels_t *labels);
Answer_t      GetYesNoAnswer ();
GameStatus_t  GameMenu        (tree_t *tree);

const char     *GetSavefileName     (const int argc, const char **argv);
TreeFuncStatus  SaveTreeInFile      (tree_t *tree, const char *dest_file_name);
TreeFuncStatus  TreePrintNodeFamily (node_t *node, FILE *dest_file);
TreeFuncStatus  GetTreeFromFile     (tree_t *tree, labels_t *labels, const char *input_file_name);
node_t         *TreeGetNodeFamily   (node_t *node, SonDir_t son_dir, tree_t *tree, labels_t *labels, FILE *input_file);
node_t         *FindHeroByName      (tree_t *tree, char *name);
void            CompareHeroes       (node_t *hero_1, node_t *hero_2, tree_t *tree);
int             PushPersonPath      (StackID stk, tree_t *tree, node_t *hero);
int             PushSamePath        (StackID path_1, StackID path_2, tree_t *tree, node_t *hero_1, node_t *hero_2, size_t max_path_len_1, size_t max_path_len_2);
void            PrintStackPath      (StackID path, size_t path_len);

#endif