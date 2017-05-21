/*
** util.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by Thomas HENON
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:31:06 2017 Thomas HENON
** Last update Thu May 18 10:31:06 2017 Thomas HENON
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

char        *ftp_cwd_check_path(char *buffer, t_ftp_client *ftp_client)
{
    char    *buffer_path;
    char    *file_path;
    char    *abs_path;
    char    *tmp;

    if (!(buffer_path = get_path_from_buffer(buffer)))
        fatal_error(ftp_client);
    file_path = buffer_path;
    if (!is_absolute_path(buffer_path)) {
        tmp = file_path;
        if (!(file_path = join_path(ftp_client->cwd, file_path)))
            fatal_error(ftp_client);
    }
    if (!(abs_path = join_path(ftp_client->home_path, file_path)))
        fatal_error(ftp_client);
    if (!check_directory_exists(abs_path)) {
        printf("file not found : %s\n", abs_path);
        send_cmd_response(&ftp_client->conn_cmd.socket_fd, 550,
                          "File not found.");
        free(abs_path);
        free(file_path);
        return NULL;
    }
    free(abs_path);
    return file_path;
}
