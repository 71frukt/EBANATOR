#include <stdio.h>

#include "akinator.h"
#include "string.h"
#include "user_interaction.h"

char *GetInputLabel(node_t *node, labels_t *labels)
{    
    while(true)
    {
        char hero_name[LABEL_LENGTH] = {};

        CHANGE_INDENT_COLOR (CYAN, 
            scanf("%[^\n]", hero_name);
        );

        getchar();                          // съесть /0
        printf("\n");

        if (strlen(hero_name) == 0)
        {
            printf(CHANGE_STR_COLOR("Incorrect input.", RED) " Write at least something:\n");
        }

        else
        {
            node->data = AddToLabels(hero_name, labels);
            break;
        }
    }

    return node->data;
}

Answer_t GetAnswer()
{
    while (true)
    {
        char answer[ANSWER_LENGTH] = {};

        // scanf("%[^\n]%*c", answer);          // TODO почему так не работает 
        CHANGE_INDENT_COLOR (CYAN, 
            scanf("%[^\n]", answer);            // а так работает
        );

        getchar();                              // съесть /0

        if (stricmp(answer, ANSWER_NO_MARK) == 0)
            return ANSWER_NO;
        
        else if (stricmp(answer, ANSWER_YES_MARK) == 0)
            return ANSWER_YES;

        else    
            printf(CHANGE_STR_COLOR("Incorrect input. ", RED) "Use " CHANGE_STR_COLOR("%s", GREEN) "/" CHANGE_STR_COLOR("%s", GREEN) ":\n", ANSWER_YES_MARK, ANSWER_NO_MARK);
    }
}

GameStatus_t ResumeOrExit()
{
    printf("\nDo you want to restart the game? (" CHANGE_STR_COLOR("%s", GREEN) "/" CHANGE_STR_COLOR("%s", GREEN) "):\n", ANSWER_YES_MARK, ANSWER_NO_MARK);

    Answer_t answer = GetAnswer();

    if (answer == ANSWER_YES)
    {
        printf(CHANGE_STR_COLOR("\n\nNew game\n", MAGENTA));
        return GAME_RESUME;
    }
    
    else
    {
        printf(CHANGE_STR_COLOR("\nGame over\n\n", MAGENTA));
        return GAME_EXIT;
    }
}