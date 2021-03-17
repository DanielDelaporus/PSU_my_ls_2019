/*
** EPITECH PROJECT, 2019
** PSU_my_ls_2019
** File description:
** No file there, just an epitech header example
*/


#include<sys/stat.h>
#include<sys/types.h>
#include<stdio.h>
#include"lib/my/my.h"
#include<stdlib.h>
#include<unistd.h>
#include<sys/sysmacros.h>

void choose_color_and_print(int *flags, char **list, char *prepath);

int apply(int *flags, char **target);

void set_flags(int *flags, char *argv)
{
    int len =  my_strlen(argv);
    for (int i = 1; i < len; i++) {
        if (argv[i] == 'l')
            flags[0] = 1;
        if (argv[i] == 'R')
            flags[1] = 1;
        if (argv[i] == 'd')
            flags[2] = 1;
        if (argv[i] == 'r')
            flags[3] = 1;
        if (argv[i] == 't')
            flags[4] = 1;
        if (argv[i] != 'l' && argv[i] != 'R' && argv[i] != 'd' &&
            argv[i] != 'r' && argv[i] != 't') {
            write(2, "ls: invalid option\n", 19);
            exit(84);
        }
    }
}

char **add_target(char **targets, char *argv, char *comp)
{
    int len;
    for (len = 0; targets[len] != comp; len++);
    targets = my_realloc(targets, len, argv);
    return targets;
}

void empty_flags_and_targets(int *flags, char **targets)
{
    int i = 0;
    for (; i < 5; i++)
        flags[i] = 0;
    i = 0;
    targets[i] = NULL;
}

int main(int argc, char *argv[])
{
    int *flags = malloc(sizeof(int) * 5);
    char **targets = malloc(sizeof(char *) * (argc + 1));
    empty_flags_and_targets(flags, targets);
    int retrn = 0;
    for (int i = 1; argv[i] != NULL; i++) {
        if (argv[i][0] == '-')
            set_flags(flags, &argv[i][0]);
        else
            targets = add_target(targets, argv[i], NULL);
    }
    if (targets[0] == NULL)
        targets = add_target(targets, ".", NULL);
    if (flags[2] == 1) {
        my_advanced_sort_word_array(targets, &my_strcmp, NULL);
        choose_color_and_print(flags, targets, "");
        return 0;
    }
    retrn = apply(flags, targets);
    return retrn;
}