
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "myftp.h"

char socket_init(int *fd)
{
    if (-1 == (*fd = socket(AF_INET, SOCK_STREAM, 0)))
        return exit_error(0);
    return 1;
}

char                    socket_accept(int *server_socket, int *client_socket) {
    struct sockaddr_in  sin;
    socklen_t           sinsize;

    sinsize = sizeof(struct sockaddr_in);
    *client_socket = accept(*server_socket, (struct sockaddr*)&sin, &sinsize);
    if (-1 == *client_socket)
        return exit_error(0);
    return 1;
}

char                    socket_listen(int *server_fd,
                                      char *listen_address,
                                      unsigned short *listen_port) {
    struct sockaddr_in  sockaddr;
    int                 option;

    option = -1;
    if (-1 == setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR,
                         &option, sizeof(option)))
        return exit_error(0);
    if (-1 == setsockopt(*server_fd, SOL_SOCKET, SO_REUSEPORT,
                         &option, sizeof(option)))
        return exit_error(0);
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(*listen_port);
    inet_aton(listen_address, &sockaddr.sin_addr);
    if (-1 == bind(*server_fd, (struct sockaddr*)&sockaddr,
                   sizeof(sockaddr)))
        return exit_error(0);
    if (-1 == listen(*server_fd, NET_BACKLOG))
        return exit_error(0);
    return 1;
}

char                    socket_infos(int *socket_fd, t_socket_infos *socket_infos) {
    struct sockaddr_in  sockaddr;
    socklen_t           socksize;

    socksize = sizeof(sockaddr);
    if (-1 == getpeername(*socket_fd, (struct sockaddr*)&sockaddr, &socksize))
        return 0;
    socket_infos->ipv4_ip = inet_ntoa(sockaddr.sin_addr);
    socket_infos->port = sockaddr.sin_port;
    return 1;
}

char socket_send(int *fd, char *buffer) {
    if (-1 == write(*fd, buffer, strlen(buffer)))
        return exit_error(0);
    return 1;
}

char socket_close(int *fd) {
    if (-1 == shutdown(*fd, SHUT_RDWR))
        return exit_error(0);
    if (-1 == close(*fd))
        return exit_error(0);
    *fd = -1;
    return 1;
}