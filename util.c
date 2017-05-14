
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "myftp.h"

char exit_error(int returnv) {
    printf("%s\n", strerror(errno));
    return returnv;
}

char        *format_pasv_ipv4_address(char *ipv4) {
    char    *ipv4_formated;
    char    *start;
    char    *token;
    int     i;

    printf("ipv4 : %s\n", ipv4);
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

char *join_path(char *s1, char *s2) {
    char *s3;
    int len;
    int s1_len;

    len = strlen(s1) + strlen(s2);
    s3 = malloc(len);
    if (NULL == s3)
        EXIT_ERROR(0, "malloc error\n");
    memset(s3, 0, len);
    s1_len = strlen(s1);
    if (s1[strlen(s1)-1] == '/' && *s2 == '/')
        s2++;
    else if (s1[strlen(s1)-1] != '/' && s2[strlen(s2)-1] != '/')
        s1[strlen(s1)] = '/';
    strncpy(s3, s1, s1_len + 1);
    strcpy(&s3[strlen(s3)], s2);
    return s3;
}

char    *parent_directory(char *path) {
    int i;

    i = strlen(path) - 1;
    while (i > 0 && path[i] != '/')
        i--;
    if (strlen(path) > 1 && path[i] == '/')
        path[i] = '\0';
    return strdup(path);
}