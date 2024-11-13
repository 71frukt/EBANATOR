#include <stdio.h>

#include "akinator.h"
#include "string.h"
#include "user_interaction.h"

#include "tree_graph.h"

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

Answer_t GetYesNoAnswer()
{
    while (true)
    {
        char answer[ANSWER_LENGTH] = {};

        CHANGE_INDENT_COLOR (CYAN, 
            scanf("%[^\n]", answer);
            getchar();                  // съесть \n        
        );

        if (stricmp(answer, ANSWER_NO_MARK) == 0)
            return ANSWER_NO;
        
        else if (stricmp(answer, ANSWER_YES_MARK) == 0)
            return ANSWER_YES;

        else    
            printf(CHANGE_STR_COLOR("Incorrect input. ", RED) "Use " CHANGE_STR_COLOR("%s", GREEN) "/" CHANGE_STR_COLOR("%s", GREEN) ":\n", ANSWER_YES_MARK, ANSWER_NO_MARK);
    }
}

GameStatus_t EndMenu(tree_t *tree)
{
    // printf("\nDo you want to restart the game? (" CHANGE_STR_COLOR("%s", GREEN) "/" CHANGE_STR_COLOR("%s", GREEN) "):\n", ANSWER_YES_MARK, ANSWER_NO_MARK);

    while (true)
    {
        printf("\nWhat do you want to do next?\n" 
                CHANGE_STR_COLOR("1:", GREEN) "restart the game  \n" 
                CHANGE_STR_COLOR("2:", GREEN) "save result data  \n"
                CHANGE_STR_COLOR("3:", GREEN) "draw result graph \n"
                CHANGE_STR_COLOR("4:", RED)   "exit              \n"
                "... ");
        int answer = 0;

        CHANGE_INDENT_COLOR (CYAN, 
            if (scanf("%d%*c", &answer) == 0)
            {
                printf(CHANGE_STR_COLOR("Incorrect input. ", RED) "Use " CHANGE_STR_COLOR("1 - 4", GREEN) ":\n");
                scanf("%*[^\n]");      // съесть оставшуюся строку
                continue;
            }          
        );

        switch (answer)
        {
            case 1:
            {
                printf(CHANGE_STR_COLOR("\n\nNew game\n", MAGENTA));
                return GAME_RESUME;
            }

            case 2:
            {
                printf("Write save file name:\n");

                char save_file_name[ANSWER_LENGTH] = {};

                CHANGE_INDENT_COLOR (CYAN, 
                    scanf("%[^\n]%*c", save_file_name); 
                );

                SaveTreeInFile(tree, save_file_name);
                break;
            }

            case 3:
            {
                printf("Write image file name:\n");

                char image_file_name[ANSWER_LENGTH] = {};

                CHANGE_INDENT_COLOR (CYAN, 
                    scanf("%[^\n]%*c", image_file_name); 
                );

                DrawGraph(tree, image_file_name);
                break;         
            }

            case 4:
            {
                printf(CHANGE_STR_COLOR("\nGame over\n\n", MAGENTA));
                return GAME_EXIT;
            }

            default:
            {
                printf(CHANGE_STR_COLOR("Incorrect input. ", RED) "Use " CHANGE_STR_COLOR("1 - 4", GREEN) ":\n");
                break;
            }
        }
    }   
}           // TODO: Сохранить или не сохранить базу данных
            // опция рисовать граф