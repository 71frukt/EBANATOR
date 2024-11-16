#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "akinator.h"
#include "user_interaction.h"
#include "tree.h"
#include "tree_debug.h"
#include "stack.h"

void AkinatorRun(tree_t *tree, labels_t *labels)
{
    TREE_ASSERT(tree);
    assert(labels);

    while (GameMenu(tree) == GAME_RESUME)
    {
        TREE_DUMP(tree);
        AskQuestion(tree->root_ptr, tree, labels);
    }

    TREE_ASSERT(tree);
}

void LabelsCtor(labels_t *labels, int start_capacity)
{
    assert(labels);
    assert(start_capacity > 0);

    labels->size = 0;
    labels->capacity = 0;

    labels->alloc_marks.size = 0;
    labels->alloc_marks = {};

    LabelsRecalloc(labels, start_capacity);
}

void LabelsDtor(labels_t *labels)
{
    assert(labels);

    for (int i = 0; i < labels->alloc_marks.size; i++)
        free(labels->alloc_marks.data[i]);

    free(labels->data);

    labels->capacity = 0;
    labels->size     = 0;
}

void LabelsRecalloc(labels_t *labels, int new_capacity)
{
    assert(labels);
    assert(new_capacity > 0);

    int prev_capacity = labels->capacity;
    labels->capacity  = new_capacity;
    
    labels->data =          (char **) realloc(labels->data, new_capacity * sizeof(char *));
    char *new_labels_text = (char *)   calloc((new_capacity - prev_capacity) * LABEL_LENGTH, sizeof(char));

    labels->alloc_marks.data[labels->alloc_marks.size++] = new_labels_text;

    for (int i = 0; i < new_capacity - prev_capacity; i++)
    {
        labels->data[prev_capacity + i] = new_labels_text + i * LABEL_LENGTH;
    }
}

char *AddToLabels(const char *got_str, labels_t *labels)
{
    assert(got_str);
    assert(labels);

    if (labels->size >= labels->capacity)
        LabelsRecalloc(labels, labels->capacity * 2);

    char *res_str = labels->data[labels->size];

    strncpy(res_str, got_str, LABEL_LENGTH - 1);

    labels->size++;

    return res_str;
}

void AskQuestion(node_t *cur_node, tree_t *tree, labels_t *labels)
{
    TREE_ASSERT(tree);
    assert(cur_node);
    assert(labels);

    printf("Is it " CHANGE_STR_COLOR("%s", YELLOW) "? (" CHANGE_STR_COLOR("%s", GREEN) "/" CHANGE_STR_COLOR("%s", GREEN) ")\n", cur_node->data, ANSWER_YES_MARK, ANSWER_NO_MARK);

    SonDir_t answer = (GetYesNoAnswer() == ANSWER_YES ? RIGHT : LEFT);

    if (cur_node->left == NODE_PTR_POISON && cur_node->right == NODE_PTR_POISON)        // дошли до ответа
        GuessHero(cur_node, tree, labels, answer);

    else if (answer == LEFT && cur_node->left != NODE_PTR_POISON)
        AskQuestion(cur_node->left, tree, labels);

    else if (answer == RIGHT && cur_node->right != NODE_PTR_POISON)
        AskQuestion(cur_node->right, tree, labels);

    else                                                                                // такого элемента в базе данных нет
    {
        printf("I have no ideas :(  Who is it? \n It is... ");

        node_t *new_node = TreeAddLeaf(tree, cur_node, answer);
        GetInputLabel(new_node, labels);

        BindNodes(cur_node, new_node, answer);

        printf("Ok! I'll take it into account next time\n");
        PrintHeroInfo(new_node);
    }

    TREE_ASSERT(tree);
    TREE_DUMP(tree);
}

void GuessHero(node_t *cur_node, tree_t *tree, labels_t *labels, SonDir_t user_answer)
{
    if (user_answer == RIGHT)                                                           // правильный ответ
    {
        printf("It is " CHANGE_STR_COLOR("%s", GREEN) "!\n", cur_node->data);
        PrintHeroInfo(cur_node);
    }

    else                                                                                // не тот
    {
        node_t *comparing_node = TreePasteBetween(tree, cur_node->father, cur_node, LEFT);

        node_t *new_node = TreeAddLeaf(tree, comparing_node, RIGHT);

        printf("I have no ideas :(  Who is it? \n It is...  ");

        GetInputLabel(new_node, labels);

        printf("What is difference between " CHANGE_STR_COLOR("%s", YELLOW) " and " CHANGE_STR_COLOR("%s", YELLOW) "?\n " CHANGE_STR_COLOR("%s", CYAN)  " is...  ",
                                                cur_node->data, new_node->data, new_node->data);
                                                
        GetInputLabel(comparing_node, labels);

        printf("Ok! I'll take it into account next time\n");
        PrintHeroInfo(new_node);
    }
}

void PrintHeroInfo(node_t *hero)
{
    printf(CHANGE_STR_COLOR("%s", CYAN) " is: ", hero->data);

    node_t *cur_node = hero;
    
    while (true)
    {
        if (cur_node->own_dir == LEFT)
            printf("not ");

        printf("%s", cur_node->father->data);

        cur_node = cur_node->father;

        if (cur_node->father->father == NODE_PTR_POISON)
            break;

        printf(", ");
    }

    printf("\n");
}

node_t *FindHeroByName(tree_t *tree, char *name)
{
    for (int i = 1; i < tree->size; i++)        // начиная с корня пропуская теневую ноду
    {
        node_t *cur_node = tree->node_ptrs[i];

        if (strcmp(cur_node->data, name) == 0)
        {
            return cur_node;
        }
    }

    return NULL;
}

void CompareHeroes(node_t *hero_1, node_t *hero_2, tree_t *tree)
{
    StackID path_1 = 0;
    StackID path_2 = 0;

    STACK_CTOR(&path_1, tree->size);
    STACK_CTOR(&path_2, tree->size);

    size_t node_1_path_len = PushPersonPath(path_1, tree, hero_1);
    size_t node_2_path_len = PushPersonPath(path_2, tree, hero_2);

    printf("\n'%s' and '%s' are similar in: ", hero_1->data, hero_2->data);

    size_t counter_of_same = PushSamePath(path_1, path_2, tree, hero_1, hero_2, node_1_path_len, node_2_path_len);

    // for (counter_of_same = 0; counter_of_same < node_1_path_len; counter_of_same++)
    // {
        // node_t *pathnode_1 = NULL;
        // node_t *pathnode_2 = NULL;
// 
        // StackPop(path_1, &pathnode_1);
        // StackPop(path_2, &pathnode_2);
// 
        // if (pathnode_1 == pathnode_2)
        // {
            // if (pathnode_1->own_dir == LEFT)
                // printf("not ");
// 
            // printf("%s ", pathnode_1->father->data);
        // }
// 
        // else 
        // {
            // StackPush(path_1, pathnode_1);
            // StackPush(path_2, pathnode_2);
            // break;   
        // }
    // }

    printf("\nand different in: \n");
    printf("'%s' is: ", hero_1->data);

    PrintStackPath(path_1, node_1_path_len - counter_of_same);

    printf("\n");
    printf("'%s' is: ", hero_2->data);

    PrintStackPath(path_2, node_2_path_len - counter_of_same);
    printf("\n");

    StackDtor(path_1);
    StackDtor(path_2);
}

int PushSamePath(StackID path_1, StackID path_2, tree_t *tree, node_t *hero_1, node_t *hero_2, size_t max_path_len_1, size_t max_path_len_2)
{
    size_t counter_of_same = 0;

    for (counter_of_same = 0; counter_of_same < max_path_len_1 && counter_of_same < max_path_len_2; counter_of_same++)
    {
        node_t *pathnode_1 = NULL;
        node_t *pathnode_2 = NULL;

        StackPop(path_1, &pathnode_1);
        StackPop(path_2, &pathnode_2);

        if (pathnode_1 == pathnode_2)
        {
            if (pathnode_1->own_dir == LEFT)
                printf("not ");

            printf("%s, ", pathnode_1->father->data);
        }

        else 
        {
            StackPush(path_1, pathnode_1);
            StackPush(path_2, pathnode_2);
            break;   
        }
    }

    return counter_of_same;
}

int PushPersonPath(StackID stk, tree_t *tree, node_t *hero)
{
    node_t *cur_node = hero;
    size_t path_len = 0;

    while (cur_node != tree->root_ptr)
    {
        StackPush(stk, cur_node);
        cur_node = cur_node->father;

        path_len++;
    }

    return path_len;
}

void PrintStackPath(StackID path, size_t path_len)
{
    for (size_t i = 0; i < path_len; i++)
    {
        node_t *pathnode = NULL;

        StackPop(path, &pathnode);

        if (pathnode->own_dir == LEFT)
            printf(" not");

        printf(" %s", pathnode->father->data);

        if (i + 1 < path_len)
            printf(",");
    }
}