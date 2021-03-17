/*
** EPITECH PROJECT, 2019
** PSU_my_ls_2019
** File description:
** No file there, just an epitech header example
*/

#include<sys/stat.h>
#include<sys/types.h>
#include"dirent.h"
#include<stdio.h>
#include<stdlib.h>
#include"lib/my/my.h"
#include<time.h>
#include<unistd.h>
#include<sys/sysmacros.h>
#include<pwd.h>
#include <grp.h>

char **add_target(char **targets, char *argv, char *comp);

char **empty_targets(char **targets, int argc);

void complement_apply(char *list, int *flags, struct stat buf, char *color);

int apply(int *flags, char **target);

void count_blocks(char **list, char *prepath, struct stat buf);

char *get_prepath(char *path);

int cmp_time(char *str1, char *str2, char *prepath);

char *color_enable(unsigned long look, char *color, struct stat buf)
{
    if (look == __S_IFLNK && my_strcmp(color, "\e[00m", NULL) == 0)
        color = "\e[36m";
    if (look == __S_IFDIR && my_strcmp(color, "\e[00m", NULL) == 0)
        color = "\e[34m";
    if ((look == __S_IFIFO || look == __S_IFBLK || look == __S_IFCHR) &&
        my_strcmp(color, "\e[00m", NULL) == 0)
        color = "\e[33m";
    if (look == __S_IFSOCK && my_strcmp(color, "\e[00m", NULL) == 0)
        color = "\e[35m";
    if (buf.st_mode & S_IXUSR && my_strcmp(color, "\e[00m", NULL) == 0)
        color = "\e[32m";
    return color;
}

void choose_color_and_print(int *flags, char **list, char *pre)
{
    struct stat buf;
    char *color = "\e[00m";
    char **targ = malloc(sizeof(char *) * 1);
    targ[0] = NULL;
    if (flags[0] == 1 && flags[2] == 0)
        count_blocks(list, pre, buf);
    for (int i = 0; list[i] != NULL; color = "\e[00m", i++) {
        char  *prep = my_strdup(pre);
        lstat(my_strcat(prep, list[i]), &buf);
        if (((buf.st_mode &__S_IFMT) == __S_IFDIR && (list[i][0] != '.' &&
            flags[1]) && !(buf.st_mode & __S_ISVTX) && (buf.st_mode & S_IROTH)))
            targ = add_target(targ, my_strcat(my_strdup(pre), list[i]), NULL);
        color = color_enable(buf.st_mode &__S_IFMT, color, buf);
        complement_apply(list[i], flags, buf, color);
        free(prep);
    }
    if (flags[1] && ((targ[0] != NULL) && (flags[2] == 0) && !my_putstr("\n")))
        apply(flags, targ);
    free(targ);
}

char **reverse_target(char **str)
{
    int len = 0;
    for (; str[len] != NULL; len++);
    char **result = malloc(sizeof(char *) * (len + 1));
    for (int i = 0; i < len; i++)
        result[i] = str[len -  i - 1];
    result[len] = NULL;
    free(str);
    return result;
}

int apply_under(int *flags, DIR *dir, char *prepath)
{
    struct dirent *sd;
    char **list = malloc(sizeof(char *) * 1);
    list[0] = NULL;
    int (* cmp)(char *, char *, char *) = (flags[4]) ? &cmp_time : &my_strcmp;
    if (dir == NULL) {
        write(2, "Error unable to open dirrectory\n", 32);
        return 84;
    }
    while ((sd = readdir(dir)) != NULL)
        list = add_target(list, sd->d_name, NULL);
    if (flags[3]) {
        my_advanced_sort_word_array(list, cmp, prepath);
        list = reverse_target(list);
    }
    else
        my_advanced_sort_word_array(list, cmp, prepath);
    choose_color_and_print(flags, list, prepath);
    free(list);
    return 0;
}

int apply(int *flags, char **target)
{
    int len_target = 0;
    int returned = 0;
    for (len_target = 0; target[len_target] != NULL; len_target++);
    if (len_target == 0 && flags[1] == 0)
        returned = apply_under(flags, opendir("."), "");
    if (len_target == 1 && flags[1] == 0)
        returned = apply_under(flags, opendir(*target), get_prepath(*target));
    if (len_target > 1 || flags[1] == 1) {
        my_advanced_sort_word_array(target, &my_strcmp, NULL);
        for (int i = 0; i < len_target - 1; i++) {
            my_putstr(my_strcat(target[i], ":\n"));
            returned = apply_under(flags, opendir(target[i]),
                get_prepath(target[i]));
            my_putstr("\n");
        }
        my_putstr(my_strcat(target[len_target - 1], ":\n"));
        returned = apply_under(flags, opendir(target[len_target - 1]),
            get_prepath(target[len_target - 1]));
    }
    return returned;
}