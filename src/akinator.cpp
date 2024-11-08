#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "akinator.h"
#include "tree.h"

void AkinatorRun(tree_t *tree, labels_t *labels)
{
    TREE_ASSERT(tree);
    assert(labels);

    while (true)
    {
        node_t *cur_node = tree->root_ptr->right;

        if (AskQuestion(cur_node, tree, labels) == GAME_EXIT)
            break;
    }

    TREE_ASSERT(tree);
}

void LabelsCtor(labels_t *labels, int start_capacity)
{
    assert(labels);
    assert(start_capacity > 0);

    labels->text = (char *) calloc(start_capacity, sizeof(char));

    labels->capacity   = start_capacity;
    labels->cur_length = 0;
}

void LabelsDtor(labels_t *labels)
{
    assert(labels);

    free(labels->text);

    labels->capacity   = 0;
    labels->cur_length = 0;
}

char *AddToLabels(char *got_str, labels_t *labels)
{
    assert(got_str);
    assert(labels);

    char *res_str = &labels->text[labels->cur_length];

    sprintf(res_str, "%s%c", got_str, '\0');

    labels->cur_length += (strlen(got_str) + 1);        // ещё \0

    return res_str;
}

GameStatus_t AskQuestion(node_t *cur_node, tree_t *tree, labels_t *labels)
{
    TREE_ASSERT(tree);
    assert(cur_node);
    assert(labels);

    printf("Is it %s?\t(%s/%s)\n", cur_node->data, ANSWER_YES_MARK, ANSWER_NO_MARK);

    SonDir_t dir = (GetAnswer() == ANSWER_YES ? RIGHT : LEFT);

    if (cur_node->left == NODE_PTR_POISON && cur_node->right == NODE_PTR_POISON)        // дошли до ответа
    {
        if (dir == RIGHT)                                                               // правильный ответ
        {
            printf("It is %s!\n", cur_node->data);
            printf("Game over\n\n\n");

            return ResumeOrExit();
        }

        else                                                                            // не тот
        {
            node_t *comparing_node = TreePasteBetween(tree, cur_node->father, cur_node, LEFT);

            node_t *new_node = TreeAddLeaf(tree, comparing_node, RIGHT);

            printf("I have no ideas :(  Who is it? \n It is...  ");
            GetInputLabel(new_node, labels);
            printf("What is difference between %s and %s?\n%s is...  ", cur_node->data, new_node->data, new_node->data);
            GetInputLabel(comparing_node, labels);

            printf("Ok! I'll take it into account next time\n");
            printf("Game over\n\n\n");
            return ResumeOrExit();
        }
    }

    else if (dir == LEFT && cur_node->left != NODE_PTR_POISON)
        AskQuestion(cur_node->left, tree, labels);

    else if (dir == RIGHT && cur_node->right != NODE_PTR_POISON)
        AskQuestion(cur_node->right, tree, labels);

    else                                                                                // такого элемента в базе данных нет
    {
        printf("I have no ideas :(  Who is it? \n It is... ");

        node_t *new_node = TreeAddLeaf(tree, cur_node, dir);
        GetInputLabel(new_node, labels);

        BindNodes(cur_node, new_node, dir);

        printf("Ok! I'll take it into account next time\n");
        printf("Game over\n\n\n");
        return ResumeOrExit();
    }

    TREE_ASSERT(tree);
    TREE_DUMP(tree);

    return GAME_EXIT;
}

Answer_t GetAnswer()
{
    while (true)
    {
        char answer[ANSWER_LENGTH] = {};

        scanf("%[^\n]%*c", answer);
        // scanf("%s", answer);

        if (stricmp(answer, ANSWER_NO_MARK) == 0)
            return ANSWER_NO;
        
        else if (stricmp(answer, ANSWER_YES_MARK) == 0)
            return ANSWER_YES;

        else    
            printf("Incorrect input. Use %s/%s:\n", ANSWER_YES_MARK, ANSWER_NO_MARK);
    }
}

char *GetInputLabel(node_t *node, labels_t *labels)
{
    char hero_name[LABEL_LENGTH] = {};
        
    scanf("%[^\n]%*c", hero_name);
    printf("\n");

    node->data = AddToLabels(hero_name, labels);

    return node->data;
}

GameStatus_t ResumeOrExit()
{
    printf("Do you want to restart the game? (%s/%s)\n", ANSWER_YES_MARK, ANSWER_NO_MARK);

    return (GetAnswer() == ANSWER_YES ? GAME_RESUME : GAME_EXIT);
}