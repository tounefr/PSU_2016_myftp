
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "myftp.h"

char exit_error(int returnv)
{
    printf("%s\n", strerror(errno));
    return returnv;
}

char        *format_pasv_ipv4_address(char *ipv4)
{
    char    *ipv4_formated;
    char    *start;
    char    *token;
    int     i;

    if (!(ipv4_formated = calloc(1, 16)))
        EXIT_ERROR(NULL, "malloc error\n")
    start = ipv4_formated;
    i = 0;
    while ((token = strtok(ipv4, "."))) {
        if (ipv4)
            ipv4 = NULL;
        strncpy(ipv4_formated, token, 3);
        ipv4_formated += ((strlen(token) > 3) ? 3 : strlen(token));
        if (i++ < 3)
            strncpy(ipv4_formated++, ",", 1);
    }
    return start;
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

void fatal_error(t_ftp_client *ftp_client) {
    socket_close(&ftp_client->conn_cmd.socket_fd);
    socket_close(&ftp_client->conn_data.socket_fd);
    exit(1);
}