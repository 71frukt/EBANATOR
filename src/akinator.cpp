#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "akinator.h"
#include "tree.h"

char ROOT_LABEL[LABEL_LENGTH] = "XZ chto";

void AkinatorRun()
{
    labels_t labels = {};
    LabelsCtor(&labels, START_LABELS_CAPACITY);

    tree_t tree = {};
    TreeCtor(&tree, START_DATATREE_SIZE);    
    tree.root_ptr = InitNewNode(&tree);

    tree.root_ptr->data = AddToLabels(ROOT_LABEL, &labels);

    // sprintf(tree.root_ptr->data, "%s", ROOT_LABEL);

    node_t *cur_node = tree.root_ptr;

    while (true)
    {
        AskQuestion(cur_node);
    }

    TreeDtor(&tree);
    LabelsDtor(&labels);
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
    char *res_str = &labels->text[labels->cur_length];

    sprintf(res_str, "%s\0", got_str);

    labels->cur_length += (strlen(got_str) + 1);        // åù¸ \0

    return res_str;
}

void AskQuestion(node_t *node)
{
    printf("Is it %s?\t(yes/no)\n", node->data);
}