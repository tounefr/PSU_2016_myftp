
#include "myftp.h"

char                    socket_accept(int *server_socket, int *client_socket)
{
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
                                      unsigned short *listen_port)
{
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
