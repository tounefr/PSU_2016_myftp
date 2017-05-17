
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "myftp.h"

char exit_error(int returnv)
{
    printf("%s\n", strerror(errno));
    return returnv;
}

void malloc_error() {
    fprintf(stderr, "malloc error\n");
    exit(1);
}

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

char        *format_pasv_ipv4_address(char *ipv4)
{
    char    *ipv4_formated;
    char    *start;
    char    *token;
    int     i;

    if (!(ipv4 = strdup(ipv4))) {
        fprintf(stderr, "malloc error\n");
        exit(1);
    }
    ipv4_formated = my_malloc(16);
    start = ipv4_formated;
    i = 0;
    while ((token = strtok(ipv4, "."))) {
        ipv4 = NULL;
        strncpy(ipv4_formated, token, 3);
        ipv4_formated += ((strlen(token) > 3) ? 3 : strlen(token));
        if (i++ < 3)
            strncpy(ipv4_formated++, ",", 1);
    }
    free(ipv4);
    return start;
}

void replace_commas_to_points(char *str) {
    int i;

    i = -1;
    while (str[++i]) {
        if (str[i] == ',')
            str[i] = '.';
    }
}

static unsigned short   extract_port(char *str, int *i) {
    int                 p1;
    int                 p2;
    unsigned short      port;

    p1 = atoi(&str[*i]);
    while (str[*i] && str[*i] != '.')
        (*i)++;
    if (!str[*i])
        return 0;
    p2 = atoi(&str[++(*i)]);
    port = p1 * 256 + p2;
    return port;
}

static char *extract_ip(char *str, int *i) {
    int     c;
    char    *ip;

    ip = my_malloc(16);
    c = 0;
    while (str[*i]) {
        if (str[*i] == '.')
            c++;
        if (c == 4) {
            strncpy(ip, str, *i);
            (*i)++;
            return ip;
        }
        (*i)++;
    }
    free(ip);
    return NULL;
}

char *extract_buffer_value(char *buffer) {
    while (buffer && *buffer != '\0' && *buffer != ' ')
        buffer++;
    buffer++;
    return strdup(buffer);
}

char        extract_ip_port(char *str, char **ip, unsigned short *port) {
    int     i;

    i = -1;
    //TODO: check returnv
    replace_commas_to_points(str);
    *ip = extract_ip(str, &i);
    *port = extract_port(str, &i);
    return 1;
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

char            check_directory_exists(char *abs_path) {
    struct stat stat_buf;

    return (stat(abs_path, &stat_buf) != -1 && S_ISDIR(stat_buf.st_mode));
}

char            check_file_exists(char *abs_path) {
    struct stat stat_buf;

    return (stat(abs_path, &stat_buf) != -1 && !S_ISDIR(stat_buf.st_mode));
}

void fatal_error(t_ftp_client *ftp_client) {
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 421, "Service not available.");
    socket_close(&ftp_client->conn_cmd.socket_fd);
    socket_close(&ftp_client->conn_data.socket_fd);
    exit(1);
}