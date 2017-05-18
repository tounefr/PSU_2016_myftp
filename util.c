/*
** util.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by toune
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:07:01 2017 toune
** Last update Thu May 18 10:07:02 2017 toune
*/

#include "myftp.h"

char is_number(char *str) {
    if (strlen(str) == 0)
        return 0;
    while (*str) {
        if (*str < '0' || *str > '9')
            return 0;
        str++;
    }
    return 1;
}

void        *my_malloc(size_t size) {
    void    *ptr;

    if (!(ptr = calloc(1, size))) {
        fprintf(stderr, "malloc error\n");
        exit(1);
    }
    return ptr;
}

char *extract_buffer_value(char *buffer) {
    while (buffer && *buffer != '\0' && *buffer != ' ')
        buffer++;
    buffer++;
    return strdup(buffer);
}

char    in_str(char c, char *tokens)
{
    int i;

    i = -1;
    while (++i < strlen(tokens)) {
        if (c == tokens[i])
            return 1;
    }
    return 0;
}
