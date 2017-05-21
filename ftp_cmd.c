/*
** ftp_cmd.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by Thomas HENON
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:31:40 2017 Thomas HENON
** Last update Thu May 18 10:31:41 2017 Thomas HENON
*/

#include "myftp.h"

void on_type_list_cmd(t_ftp_server *ftp_server,
                      t_ftp_client *ftp_client,
                      char *buffer)
{
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 200,
                      "TYPE is now 8-bit binary");
}

void        on_ftp_user_cmd(t_ftp_server *ftp_server,
                            t_ftp_client *ftp_client,
                            char *buffer)
{
    char    *user;

    if (!(user = extract_buffer_value(buffer)))
        fatal_error(ftp_client);
    if (strcasecmp(user, "anonymous")) {
        send_cmd_response(&ftp_client->conn_cmd.socket_fd, 530,
                          "Not logged in.");
        return;
    }
    if (ftp_client->user)
        free(ftp_client->user);
    printf("Login : %s\n", user);
    ftp_client->user = user;
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 331,
                      "Please specify the password.");
}

void        on_ftp_pass_cmd(t_ftp_server *ftp_server,
                     t_ftp_client *ftp_client,
                     char *buffer)
{
    char    *pass;

    if (!(pass = extract_buffer_value(buffer)))
        fatal_error(ftp_client);
    printf("pass : %s\n", pass);
    if (ftp_client->user == NULL) {
        send_cmd_response(&ftp_client->conn_cmd.socket_fd, 332,
                          "Need login first.");
        return;
    }
    if (!strcasecmp(ftp_client->user, "anonymous") &&
            (!strcasecmp(pass, " ") || !strcasecmp(pass, ""))) {
        ftp_client->is_logged = 1;
        send_cmd_response(&ftp_client->conn_cmd.socket_fd, 230,
                          "Login successful.");
        return;
    }
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 530,
                      "Wrong credentials.");
}

void        on_ftp_cwd_cmd(t_ftp_server *ftp_server,
                           t_ftp_client *ftp_client,
                           char *buffer)
{
    char    *file_path;

    if (!(file_path = ftp_cwd_check_path(buffer, ftp_client)))
        return;
    if (ftp_client->cwd)
        free(ftp_client->cwd);
    if (!(ftp_client->cwd = strdup(file_path)))
        fatal_error(ftp_client);
    free(file_path);
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 250,
                      "Directory successfully changed.");
}

void        on_ftp_cdup_cmd(t_ftp_server *ftp_server,
                            t_ftp_client *ftp_client,
                            char *buffer)
{
    char    *tmp;

    tmp = ftp_client->cwd;
    if (!(ftp_client->cwd = path_parent_directory(ftp_client->cwd)))
        fatal_error(ftp_client);
    free(tmp);
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 250,
                      "Directory successfully changed.");
}
