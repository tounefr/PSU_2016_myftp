/*
** socket.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by toune
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:06:53 2017 toune
** Last update Thu May 18 10:06:53 2017 toune
*/

#include "myftp.h"

char socket_init(int *fd)
{
    if (-1 == (*fd = socket(AF_INET, SOCK_STREAM, 0)))
        EXIT_ERROR(0, "socket : %s\n", strerror(errno))
    return 1;
}

char                    socket_connect(int *fd, char *ip, unsigned short *port)
{
    struct sockaddr_in  sockaddr;
    socklen_t           socksize;

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(*port);
    inet_pton(AF_INET, ip, &(sockaddr.sin_addr));
    socksize = sizeof(sockaddr);
    if (-1 == connect(*fd, (struct sockaddr *)&sockaddr, socksize))
        EXIT_ERROR(0, "connect error : %s\n", strerror(errno))
    return 1;
}

char                    socket_infos(int *socket_fd, t_socket_infos *socket_infos)
{
    struct sockaddr_in  sockaddr;
    socklen_t           socksize;

    socksize = sizeof(sockaddr);
    if (-1 == getpeername(*socket_fd, (struct sockaddr*)&sockaddr, &socksize))
        return 0;
    socket_infos->client_ipv4 = strdup(inet_ntoa(sockaddr.sin_addr));
    socket_infos->client_port = sockaddr.sin_port;
    memset(&sockaddr, 0, sizeof(struct sockaddr_in));
    if (-1 == getsockname(*socket_fd, (struct sockaddr*)&sockaddr, &socksize))
        return 0;
    socket_infos->server_ipv4 = strdup(inet_ntoa(sockaddr.sin_addr));
    assert(socket_infos->client_ipv4 != NULL && socket_infos->server_ipv4 != NULL);
    socket_infos->server_port = sockaddr.sin_port;
    return 1;
}

char socket_send(int *fd, char *buffer)
{
    if (-1 == write(*fd, buffer, strlen(buffer)))
        return exit_error(0);
    return 1;
}

char socket_close(int *fd)
{
    shutdown(*fd, SHUT_RDWR);
    close(*fd);
    *fd = -1;
    return 1;
}