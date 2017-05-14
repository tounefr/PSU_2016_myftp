
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "myftp.h"

static char *allocate_new_command_packet(char *buffer,
                                  unsigned int buffer_size,
                                  unsigned int limit) {
    char    *packet;

    if (NULL == (packet = malloc(limit + 1)))
        EXIT_ERROR(NULL, "malloc error\n")
    return memcpy(packet, buffer, limit);
}

char        *ftp_recv_packet_command(int *fd)
{
    char    *buffer;
    int     i;

    buffer = malloc(NET_BUFFER_SIZE);
    memset(buffer, 0, NET_BUFFER_SIZE);
    if (-1 == read(*fd, buffer, NET_BUFFER_SIZE - 1))
        EXIT_ERROR(NULL, "read error %s\n", strerror(errno))
    i = -1;
    while (++i < strlen(buffer)) {
        if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
            buffer[i] = '\0';
            return buffer;
        }
    }
    return NULL;
}

char socket_data_conn(t_ftp_client *ftp_client) {
    int fd;

    if (!socket_init(&fd))
        return 0;
    if (!socket_accept(&fd, &ftp_client->conn_data.socket_fd))
        return 0;
    socket_close(&fd);
    return 1;
}

char                listen_data_conn(t_ftp_client *ftp_client, unsigned short *listen_port) {
    char            *pasv_buffer;
    pid_t           child_pid;
    int             server_fd;

    if (ftp_client->conn_data.socket_fd != -1)
        socket_close(&ftp_client->conn_data.socket_fd);
    *listen_port = rand_port();
    if (!socket_init(&server_fd))
        EXIT_ERROR(0, "%s\n", strerror(errno))
    if (!socket_listen(&server_fd, NET_SERVER_ADDRESS, listen_port))
        EXIT_ERROR(0, "%s\n", strerror(errno))
    if (!socket_accept(&server_fd, &ftp_client->conn_data.socket_fd))
        EXIT_ERROR(0, "%s\n", strerror(errno))
    socket_close(&server_fd);
    return 1;
}

unsigned short rand_port() {
    static char init = 0;
    int min;
    int max;
    unsigned short num;

    if (!init)
        srand(time(NULL));
    min = 1025;
    max = 65535;
    num = (rand() % max) + min + 1;
    if (socket_port_used(num))
        return rand_port();
    return num;
}