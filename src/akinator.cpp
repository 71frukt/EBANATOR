#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "akinator.h"
#include "user_interaction.h"
#include "tree.h"
#include "tree_debug.h"

void AkinatorRun(tree_t *tree, labels_t *labels)
{
    TREE_ASSERT(tree);
    assert(labels);

    while (true)
    {
        TREE_DUMP(tree);

        if (AskQuestion(tree->root_ptr, tree, labels) == GAME_EXIT)
            break;
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

GameStatus_t AskQuestion(node_t *cur_node, tree_t *tree, labels_t *labels)
{
    TREE_ASSERT(tree);
    assert(cur_node);
    assert(labels);

    printf("Is it " CHANGE_STR_COLOR("%s", YELLOW) "? (" CHANGE_STR_COLOR("%s", GREEN) "/" CHANGE_STR_COLOR("%s", GREEN) ")\n", cur_node->data, ANSWER_YES_MARK, ANSWER_NO_MARK);

    SonDir_t answer = (GetYesNoAnswer() == ANSWER_YES ? RIGHT : LEFT);

    if (cur_node->left == NODE_PTR_POISON && cur_node->right == NODE_PTR_POISON)        // дошли до ответа
        GuessHero(cur_node, tree, labels, answer);

    else if (answer == LEFT && cur_node->left != NODE_PTR_POISON)
        return AskQuestion(cur_node->left, tree, labels);

    else if (answer == RIGHT && cur_node->right != NODE_PTR_POISON)
        return AskQuestion(cur_node->right, tree, labels);

    else                                                                                // такого элемента в базе данных нет
    {
        printf("I have no ideas :(  Who is it? \n It is... ");

        node_t *new_node = TreeAddLeaf(tree, cur_node, answer);
        GetInputLabel(new_node, labels);

        BindNodes(cur_node, new_node, answer);

        printf("Ok! I'll take it into account next time\n");
        PrintHeroInfo(new_node);
    }

    return EndMenu(tree);

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