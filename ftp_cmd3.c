/*
** ftp_cmd3.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by Thomas HENON
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:29:04 2017 Thomas HENON
** Last update Thu May 18 10:29:04 2017 Thomas HENON
*/

#include "myftp.h"

void on_ftp_help_cmd(t_ftp_server *ftp_server,
                     t_ftp_client *ftp_client,
                     char *buffer)
{
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 214,
                      "The following commands are recognized.\n"
                      "USER PASS CWD CDUP QUIT DELE PWD PASV PORT "
                      "HELP NOOP RETR STOR LIST");
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 214,
                      "Help OK.");
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
