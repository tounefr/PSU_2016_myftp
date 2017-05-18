
#include "myftp.h"

char    socket_port_used(unsigned short port)
{
    int fd;

    if (!socket_init(&fd))
        return 1;
    if (!socket_listen(&fd, NET_SERVER_ADDRESS, &port)) {
        socket_close(&fd);
        return 1;
    }
    socket_close(&fd);
    return 0;
}