/*
** path.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by toune
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:06:46 2017 toune
** Last update Thu May 18 10:06:47 2017 toune
*/

#include "myftp.h"

char        *join_path(char *s1, char *s2)
{
    char    *s3;
    int     s1_len;

    s1 = strdup(s1);
    s2 = strdup(s2);
    if (!s1 || !s2)
        malloc_error();
    s3 = my_malloc(strlen(s1) + strlen(s2) + 2);
    if (s1[strlen(s1)-1] == '/' && *s2 == '/')
        s1[strlen(s1)-1] = '\0';
    if (s1[strlen(s1)-1] != '/' && *s2 != '/') {
        s1_len = strlen(s1);
        if (!(s1 = realloc(s1, s1_len + 2)))
            malloc_error();
        s1[s1_len] = '/';
        s1[s1_len + 1] = '\0';
    }
    strcpy(s3, s1);
    strcpy(&s3[strlen(s1)], s2);
    free(s1);
    free(s2);
    return s3;
}

static unsigned int     path_count_chars_to_normalize(char *path)
{
    unsigned int        i;
    unsigned int        i2;
    unsigned int        c;

    c = 0;
    i = -1;
    while (++i < strlen(path)) {
        i2 = -1;
        while (++i2 < strlen(PATH_CHARS_TO_NORMALIZE)) {
            if (path[i] == PATH_CHARS_TO_NORMALIZE[i2])
                c++;
        }
    }
    return c;
}

char                *normalize_path(char *path)
{
    unsigned int    count_chars_to_normalize;
    char            *path_normalized;
    unsigned int    i;
    unsigned int    i2;

    count_chars_to_normalize = path_count_chars_to_normalize(path);
    path_normalized = my_malloc(strlen(path) + count_chars_to_normalize + 1);
    i = 0;
    i2 = 0;
    while (i2 < strlen(path)) {
        if (in_str(path[i2], PATH_CHARS_TO_NORMALIZE))
            path_normalized[i++] = '\\';
        path_normalized[i++] = path[i2++];
    }
    return path_normalized;
}
