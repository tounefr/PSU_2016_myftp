/*
** path2.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by Thomas HENON
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:30:02 2017 Thomas HENON
** Last update Thu May 18 10:30:03 2017 Thomas HENON
*/

#include "myftp.h"

char    *path_parent_directory(char *path)
{
    int i;

    i = strlen(path) - 1;
    while (i > 0 && path[i] != '/')
        i--;
    if (strlen(path) > 1 && path[i] == '/')
        path[i] = '\0';
    return strdup(path);
}

char is_absolute_path(char *path) {
    return (path && *path == '/');
}

char *get_path_from_buffer(char *buffer) {
    while (buffer && *buffer != '\0' && *buffer != ' ')
        buffer++;
    buffer++;
    return strdup(buffer);
}

char        *get_path(char *buffer, t_ftp_client *ftp_client) {
    char    *path;
    char    *tmp;

    if (!(path = get_path_from_buffer(buffer)))
        return NULL;
    tmp = path;
    if (!(path = join_path(ftp_client->cwd, path)))
        return NULL;
    free(tmp);
    tmp = path;
    if (!(path = join_path(ftp_client->home_path, path)))
        return NULL;
    free(tmp);
    return path;
}
