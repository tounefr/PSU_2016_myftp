
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "myftp.h"

char packet_prechecks(t_ftp_client *ftp_client,
                      char *cmd,
                      int flags,
                      char *buffer)
{
    if (flags & PRECHECK_LOGGED && !ftp_client->is_logged) {
        send_cmd_response(&ftp_client->conn_cmd.socket_fd, 530,
                          "Not logged in.");
        return 0;
    }
    return 1;
}

char        ftp_client_loop(t_ftp_server *ftp_server, t_ftp_client *ftp_client)
{
    char    *buffer;
    int     i;

    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 220, FTP_HELLO_MESSAGE);
    while (1) {
        if (!(buffer = ftp_recv_packet_command(&ftp_client->conn_cmd.socket_fd))) {
            printf("ftp_recv_packet_command failed\n");
            break;
        }
        i = -1;
        while (++i < CS_CMD_N) {
            if (!strncmp(g_cs_cmd[i].cmd, buffer, strlen(g_cs_cmd[i].cmd))) {
                printf("Recv %s\n", buffer);
                if (packet_prechecks(ftp_client, g_cs_cmd[i].cmd,
                                     g_cs_cmd[i].flags_prechecks, buffer))
                    g_cs_cmd[i].callback(ftp_server, ftp_client, buffer);
                break;
            }
        }
        if (i >= CS_CMD_N) {
            printf("Recv unsupported cmd : %s\n", buffer);
            send_cmd_response(&ftp_client->conn_cmd.socket_fd, 502, NULL);
        }
        free(buffer);
    }
    socket_close(&ftp_client->conn_cmd.socket_fd);
}

char                send_cmd_response(int *fd, int code, char *content)
{
    unsigned int    buffer_size;
    char            *buffer;

    if (content == NULL)
        content = "";
    printf("Send %d : %s\n", code, content);
    buffer_size = 3 + 3 + strlen(content) + 1;
    if (NULL == (buffer = malloc(buffer_size)))
        return 0;
    snprintf(buffer, buffer_size, "%d %s\r\n", code, content);
    socket_send(fd, buffer);
    free(buffer);
}