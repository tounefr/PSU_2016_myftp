/*
** ftp_cmd2.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by Thomas HENON
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:31:30 2017 Thomas HENON
** Last update Thu May 18 10:31:31 2017 Thomas HENON
*/

#include "myftp.h"

void on_ftp_quit_cmd(t_ftp_server *ftp_server,
                     t_ftp_client *ftp_client,
                     char *buffer)
{
    if (ftp_client->conn_cmd.socket_fd != -1) {
        send_cmd_response(&ftp_client->conn_cmd.socket_fd, 221,
                          "Service closing control connection.");
        socket_close(&ftp_client->conn_cmd.socket_fd);
    }
}

void            on_ftp_dele_cmd(t_ftp_server *ftp_server,
                                t_ftp_client *ftp_client,
                                char *buffer)
{
    char        *path;
    struct stat stat_buf;
    char        *abs_path;

    if (!(abs_path = get_path(buffer, ftp_client)))
        fatal_error(ftp_client);
    if (!check_file_exists(abs_path)) {
        //TODO: send file not exists
        return;
    }
    printf("DELETE %s\n", path);
    //TODO: activate
    //unlink(path);
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 250,
                      "Requested file action okay, completed.");
}

void        on_ftp_pwd_cmd(t_ftp_server *ftp_server,
                           t_ftp_client *ftp_client,
                           char *buffer)
{
    char    *cwd;
    char    cwd_buffer[1000];
    int     buffer_len;

    snprintf(cwd_buffer, sizeof(cwd_buffer) - 1,
             "\"%s\", is your current location", ftp_client->cwd);
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 257, cwd_buffer);
}

void                on_ftp_pasv_cmd(t_ftp_server *ftp_server,
                                    t_ftp_client *ftp_client,
                                    char *buffer)
{
    char            *listen_address;
    char            *pasv_buffer;
    unsigned short  listen_port;
    char            *server_ipv4;

    pasv_buffer = my_malloc(1000);
    listen_port = rand_port();
    server_ipv4 = ftp_client->conn_cmd.socket_infos.server_ipv4;
    listen_address = format_pasv_ipv4_address(server_ipv4);
    snprintf(pasv_buffer, 999, "Entering Passive Mode (%s,%d,%d)",
             listen_address,
             (listen_port / 256),
             (listen_port % 256));
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 227, pasv_buffer);
    free(pasv_buffer);
    if (!listen_data_conn(ftp_client, &listen_port)) {
        send_cmd_response(&ftp_client->conn_cmd.socket_fd, 425,
                          "Can't open data connection.");
        return;
    }
}

void                on_ftp_port_cmd(t_ftp_server *ftp_server,
                                    t_ftp_client *ftp_client,
                                    char *buffer)
{
    char            *ip;
    unsigned short  port;

    while (buffer && *buffer != '\0' && *buffer != ' ')
        buffer++;
    buffer++;
    if (!extract_ip_port(buffer, &ip, &port))
        fatal_error(ftp_client);
    if (!ftp_connect(ftp_client, ip, &port)) {
        send_cmd_response(&ftp_client->conn_cmd.socket_fd, 421,
                          "Service not available.");
        return;
    }
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 200,
                      "PORT command successful.");
}
