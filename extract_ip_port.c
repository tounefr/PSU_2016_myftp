/*
** extract_ip_port.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by Thomas HENON
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:31:13 2017 Thomas HENON
** Last update Thu May 18 10:31:14 2017 Thomas HENON
*/

#include "myftp.h"

char        *format_pasv_ipv4_address(char *ipv4)
{
    char    *ipv4_formated;
    char    *start;
    char    *token;
    int     i;

    if (!(ipv4 = strdup(ipv4)))
        malloc_error();
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

char        extract_ip_port(char *str, char **ip, unsigned short *port) {
    int     i;

    i = -1;
    //TODO: check returnv
    replace_commas_to_points(str);
    *ip = extract_ip(str, &i);
    *port = extract_port(str, &i);
    return 1;
}
