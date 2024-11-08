#ifndef USER_INTERACTION_H
#define USER_INTERACTION_H

#include <stdio.h>

#define BLACK   "30" 
#define RED     "31"
#define GREEN   "32"
#define YELLOW  "33"
#define BLUE    "34"
#define MAGENTA "35"
#define CYAN    "36"
#define WHITE   "37"

#define CHANGE_STR_COLOR(STR, COLOR)  "\x1b[" COLOR "m" STR "\x1b[0m"

#define CHANGE_INDENT_COLOR(COLOR, ...)                                          \
{                                                                               \
    printf("\x1b[" COLOR "m");                                                  \
    __VA_ARGS__;                                                                \
    printf("\x1b[0m");                                                          \
}

#endif