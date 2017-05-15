
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

char                extract_ip_port(char *str) {
    char            *token;
    char            *ipv4;
    unsigned short  port;
    unsigned int    i;
    char            *ip_tmp;

    printf("extract_ip_port\n");
    if (!(ipv4 = calloc(1, 16)))
        EXIT_ERROR(0, "malloc error\n")
    i = 0;
    ip_tmp = ipv4;
    port = 0;
    while ((token = strtok(str, ","))) {
        str = NULL;
        if (++i <= 3) {
            strncpy(ip_tmp, token, 3);
            ip_tmp += 3;
            if (i < 3)
                strcpy(ip_tmp++, ".");
        }
        else if (i == 4)
            port += atoi(token) * 256;
        else if (i == 5)
            port += atoi(token);
    }
    printf("%s %d\n", ipv4, port);
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