/*
** ftp_socket.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by Thomas HENON
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:32:21 2017 Thomas HENON
** Last update Thu May 18 10:32:29 2017 Thomas HENON
*/

#include "myftp.h"

char    socket_data_conn(t_ftp_client *ftp_client)
{
    int fd;

    if (!socket_init(&fd))
        return 0;
    if (!socket_accept(&fd, &ftp_client->conn_data.socket_fd))
        return 0;
    socket_close(&fd);
    return 1;
}

char        listen_data_conn(t_ftp_client *ftp_client, unsigned short *listen_port)
{
    char    *pasv_buffer;
    pid_t   child_pid;
    int     server_fd;

    if (ftp_client->conn_data.socket_fd != -1)
        socket_close(&ftp_client->conn_data.socket_fd);
    *listen_port = rand_port();
    if (!socket_init(&server_fd))
        EXIT_ERROR(0, "%s\n", strerror(errno))
    if (!socket_listen(&server_fd, NET_SERVER_ADDRESS, listen_port))
        EXIT_ERROR(0, "%s\n", strerror(errno))
    if (!socket_accept(&server_fd, &ftp_client->conn_data.socket_fd))
        EXIT_ERROR(0, "%s\n", strerror(errno))
    printf("New data conn\n");
    socket_close(&server_fd);
    return 1;
}

unsigned short      rand_port()
{
    static char     init = 0;
    int             min;
    int             max;
    unsigned short  num;

    if (!init)
        srand(time(NULL));
    min = 1025;
    max = 65535;
    num = (rand() % max) + min + 1;
    if (socket_port_used(num))
        return rand_port();
    return num;
}
