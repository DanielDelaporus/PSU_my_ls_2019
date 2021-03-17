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

char *get_prepath(char *path)
{
    int index = 0;
    int i = 0;
    for (; path[i] != '\0'; i++)
        if (path[i] == '/')
            index = i + 1;
    if (path[i - 1] != '/') {
        char *c = malloc(sizeof(char) * 2);
        c[0] = '/';
        c[1] = '\0';
        char *copy = my_strdup(path);
        return my_strcat(copy, c);
    }
    char *res = malloc(sizeof(char) * my_strlen(path) * 2);
    res = my_strncpy(res, path, index);
    return res;
}

void rwx_all(struct stat buf)
{
    (buf.st_mode & S_IRUSR) ? my_putchar('r') : my_putchar('-');
    (buf.st_mode & S_IWUSR) ? my_putchar('w') : my_putchar('-');
    (buf.st_mode & S_IXUSR) ? my_putchar('x') : my_putchar('-');
    (buf.st_mode & S_IRGRP) ? my_putchar('r') : my_putchar('-');
    (buf.st_mode & S_IWGRP) ? my_putchar('w') : my_putchar('-');
    (buf.st_mode & S_IXGRP) ? my_putchar('x') : my_putchar('-');
    (buf.st_mode & S_IROTH) ? my_putstr("r") : my_putstr("-");
    (buf.st_mode & S_IWOTH) ? my_putstr("w") : my_putstr("-");
    if (buf.st_mode & S_IXOTH && !(buf.st_mode & __S_ISVTX))
        my_putstr("x ");
    if (buf.st_mode & __S_ISVTX && (buf.st_mode &__S_IFMT) == __S_IFDIR)
        my_putstr("t ");
    if (buf.st_mode & __S_ISVTX && (((buf.st_mode &__S_IFMT) == __S_IFREG)
        || (buf.st_mode & S_IXOTH) == 0))
        my_putstr("T ");
    if (!(buf.st_mode & S_IXOTH) && !(buf.st_mode & __S_ISVTX))
        my_putstr("- ");
}

void under_flag_l(struct stat buf)
{
    rwx_all(buf);
    my_put_nbr((int) buf.st_nlink);
    my_putchar(' ');
    struct passwd *pwd = getpwuid(buf.st_uid);
    my_putstr(pwd->pw_name);
    my_putchar(' ');
    my_putstr(getgrgid(buf.st_gid)->gr_name);
    my_putchar(' ');
    if ((buf.st_mode &__S_IFMT) == __S_IFCHR ||
        (buf.st_mode &__S_IFMT) == __S_IFBLK) {
        my_put_nbr(major(buf.st_rdev));
        my_putstr(", ");
        my_put_nbr(minor(buf.st_rdev));
    }
    else
        my_put_nbr((int) buf.st_size);
    my_putchar(' ');
    time_t times = buf.st_mtime;
    write(1, &ctime(&times)[4], my_strlen(ctime(&times)) - 13);
    my_putchar(' ');
}

int flag_l(struct stat buf, int *flags)
{
    if (flags[0] == 0)
        return 0;
    unsigned long look =  buf.st_mode &__S_IFMT;
    if (look == __S_IFDIR)
        my_putchar('d');
    if (look == __S_IFLNK)
        my_putchar('l');
    if (look == __S_IFIFO)
        my_putchar('p');
    if (look == __S_IFCHR)
        my_putchar('c');
    if (look == __S_IFSOCK)
        my_putchar('s');
    if (look == __S_IFBLK)
        my_putchar('b');
    if ((look != __S_IFDIR) && (look != __S_IFLNK) && (look != __S_IFIFO)
        && (look != __S_IFCHR) && (look != __S_IFBLK))
        my_putchar('-');
    under_flag_l(buf);
    return 0;
}