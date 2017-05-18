/*
** error.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by Thomas HENON
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:27:33 2017 Thomas HENON
** Last update Thu May 18 10:27:34 2017 Thomas HENON
*/

#include "myftp.h"

void fatal_error(t_ftp_client *ftp_client) {
    send_cmd_response(&ftp_client->conn_cmd.socket_fd,
                      421,
                      "Service not available.");
    socket_close(&ftp_client->conn_cmd.socket_fd);
    socket_close(&ftp_client->conn_data.socket_fd);
    exit(1);
}

char exit_error(int returnv)
{
    printf("%s\n", strerror(errno));
    return returnv;
}

void malloc_error() {
    fprintf(stderr, "malloc error\n");
    exit(1);
}
