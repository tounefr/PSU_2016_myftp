
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "myftp.h"

char        ftp_client_loop(t_ftp_server *ftp_server, t_ftp_client *ftp_client) {
    char    *buffer;
    int     i;

    send_cmd_response(&ftp_client->socket_fd, 220, FTP_HELLO_MESSAGE);
    while (1) {
        if (!(buffer = ftp_recv_packet_command(&ftp_client->socket_fd)))
            break;
        i = -1;
        while (++i < CS_CMD_N) {
            if (!strncmp(g_cs_cmd[i].cmd, buffer, strlen(g_cs_cmd[i].cmd))) {
                printf("Recv %s\n", buffer);
                g_cs_cmd[i].callback(ftp_client, buffer);
                break;
            }
        }
        if (i >= CS_CMD_N)
            printf("Recv unsupported cmd : %s\n", buffer);
        sleep(1);
    }
    socket_close(&ftp_client->socket_fd);
}

char send_cmd_response(int *fd, int code, char *content) {
    unsigned int buffer_size;
    char *buffer;

    buffer_size = 3 + 3 + strlen(content) + 1;
    if (NULL == (buffer = malloc(buffer_size)))
        return 0;
    snprintf(buffer, buffer_size, "%d %s\r\n", code, content);
    socket_send(fd, buffer);
    free(buffer);
}