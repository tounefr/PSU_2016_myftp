/*
** ftp_cmd3.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by toune
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:07:26 2017 toune
** Last update Thu May 18 10:07:26 2017 toune
*/

#include "myftp.h"

void on_ftp_help_cmd(t_ftp_server *ftp_server,
                     t_ftp_client *ftp_client,
                     char *buffer)
{
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 214,
                      "RTFM");
}

void on_ftp_noo_cmd(t_ftp_server *ftp_server,
                    t_ftp_client *ftp_client,
                    char *buffer)
{
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 200,
                      "Ok");
}

void        on_ftp_list_cmd(t_ftp_server *ftp_server,
                            t_ftp_client *ftp_client,
                            char *buffer)
{
    char    *abs_path;

    if (!(abs_path = join_path(ftp_client->home_path, ftp_client->cwd)))
        fatal_error(ftp_client);
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 150,
                      "Here comes the directory listing.");
    if (!ftp_send_list_file(abs_path, ftp_client))
        fatal_error(ftp_client);
    socket_close(&ftp_client->conn_data.socket_fd);
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 226,
                      "Directory send OK.");
}
