/*
** EPITECH PROJECT, 2019
** PSU_my_ls_2019
** File description:
** No file there, just an epitech header example
*/

#include<sys/stat.h>
#include<sys/types.h>
#include<time.h>
#include<stdio.h>
#include<unistd.h>
#include"lib/my/my.h"
#include<stdlib.h>
#include<sys/sysmacros.h>

int flag_l(struct stat buf, int *flags);

void count_blocks(char **list, char *prepath, struct stat buf)
{
    int tot = 0;
    char  *prep;
    for (int i = 0; list[i] != NULL; i++) {
        prep = my_strdup(prepath);
        lstat(my_strcat(prep, list[i]), &buf);
        if (list[i][0] != '.')
            tot += buf.st_blocks;
    }
    write(1, "total ", 6);
    my_put_nbr(tot/2);
    my_putchar('\n');
    free(prep);
}

int cmp_time(char *str1, char *str2, char *prepath)
{
    struct stat buf1;
    struct stat buf2;
    lstat(my_strcat(prepath, str1), &buf1);
    lstat(my_strcat(prepath, str2), &buf2);
    time_t time1 = buf1.st_mtime;
    time_t time2 = buf2.st_mtime;
    return time2 - time1;
}

void complement_apply(char *list, int *flags, struct stat buf)
{
    if (list[0] != '.' || flags[2] == 1) {
        flag_l(buf, flags);
        my_putstr(list);
        my_putstr("\n");
    }
}

void complement_apply_c(char *list, int *flags, struct stat buf, char *color)
{
    if (list[0] != '.' || flags[2] == 1) {
        flag_l(buf, flags);
        my_putstr(color);
        my_putstr(list);
        my_putstr("\e[00m");
        my_putstr("\n");
    }
}