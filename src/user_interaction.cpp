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

GameStatus_t GameMenu(tree_t *tree)
{
    // printf("\nDo you want to restart the game? (" CHANGE_STR_COLOR("%s", GREEN) "/" CHANGE_STR_COLOR("%s", GREEN) "):\n", ANSWER_YES_MARK, ANSWER_NO_MARK);

    while (true)
    {
        printf("\nWhat do you want to do next?\n" 
                CHANGE_STR_COLOR("1:", GREEN) "start new game     \n" 
                CHANGE_STR_COLOR("2:", GREEN) "save result data   \n"
                CHANGE_STR_COLOR("3:", GREEN) "draw result graph  \n"
                CHANGE_STR_COLOR("4:", GREEN) "compare two heroes \n"
                CHANGE_STR_COLOR("5:", RED)   "exit               \n"
                "... ");
        int answer = 0;

        CHANGE_INDENT_COLOR (CYAN, 
            if (scanf("%d%*c", &answer) == 0)
            {
                printf(CHANGE_STR_COLOR("Incorrect input. ", RED) "Use " CHANGE_STR_COLOR("1 - 5", GREEN) ":\n");
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

                strncat(save_file_name, SAVE_FILE_EXTENSION, strlen(save_file_name) - strlen(SAVE_FILE_EXTENSION));

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

                strncat(image_file_name, IMAGE_FILE_EXTENSION, strlen(image_file_name) - strlen(IMAGE_FILE_EXTENSION));

                DrawGraph(tree, image_file_name);
                break;         
            }

            case 4:
            {
                char hero_1_name[ANSWER_LENGTH] = {};
                char hero_2_name[ANSWER_LENGTH] = {};

                printf("Write first hero name:\n");

                CHANGE_INDENT_COLOR (CYAN, 
                    scanf("%[^\n]%*c", hero_1_name); 
                );

                printf("Write second hero name:\n");

                CHANGE_INDENT_COLOR (CYAN, 
                    scanf("%[^\n]%*c", hero_2_name); 
                );

                node_t *hero_1 = FindHeroByName(tree, hero_1_name);
                node_t *hero_2 = FindHeroByName(tree, hero_2_name);

                if (hero_1 == NULL || hero_2 == NULL)
                {
                    if (hero_1 == NULL)
                        printf("There is no hero named '%s'\n", hero_1_name);

                    if (hero_2 == NULL)
                        printf("There is no hero named '%s'\n", hero_2_name);

                    return GameMenu(tree);
                }

                PrintHeroInfo(hero_1);
                PrintHeroInfo(hero_2);
                
                CompareHeroes(hero_1, hero_2, tree);

                break;
            }

            case 5:
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

    return GAME_RESUME;
}