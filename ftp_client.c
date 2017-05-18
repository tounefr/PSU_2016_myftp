/*
** ftp_client.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by Thomas HENON
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:28:55 2017 Thomas HENON
** Last update Thu May 18 10:28:56 2017 Thomas HENON
*/

#include "myftp.h"

void    free_ftp_client(t_ftp_client *ftp_client)
{
    if (!ftp_client)
        return;
    if (ftp_client->user)
        free(ftp_client->user);
    if (ftp_client->pass)
        free(ftp_client->pass);
    free(ftp_client);
    printf("ftp client freed\n");
}

void    release_ftp_client(t_ftp_client *client)
{
    printf("client pid=%d exited : %s\n", client->pid, strerror(errno));
    if (client->conn_data.socket_fd != -1)
        socket_close(&client->conn_data.socket_fd);
    if (client->conn_cmd.socket_fd != -1)
        socket_close(&client->conn_cmd.socket_fd);
    free_ftp_client(client);
}

t_ftp_client        *init_ftp_client(
        t_ftp_server *ftp_server,
        pid_t pid, int client_socket_fd)
{
    t_ftp_client    *ftp_client;

    ftp_client = NULL;
    ftp_client = my_malloc(sizeof(t_ftp_client));
    ftp_client->pid = pid;
    ftp_client->conn_cmd.socket_fd = client_socket_fd;
    ftp_client->is_logged = 0;
    ftp_client->cwd = NULL;
    ftp_client->next = NULL;
    ftp_client->user = NULL;
    ftp_client->home_path = strdup(ftp_server->anon_directory_path);
    ftp_client->cwd = strdup("/");
    ftp_client->conn_data.socket_fd = -1;
    if (!ftp_client->home_path || !ftp_client->cwd)
        malloc_error();
    ftp_client->pass = NULL;
    if (!socket_infos(&ftp_client->conn_cmd.socket_fd,
                      &ftp_client->conn_cmd.socket_infos))
        return NULL;
    return ftp_client;
}

t_ftp_client        *new_ftp_client(
        t_ftp_server *ftp_server,
        t_ftp_client **clients,
        pid_t pid,
        int client_socket_fd)
{
    t_ftp_client    *cur_client;
    t_ftp_client    *ftp_client;
    t_ftp_client    *prev_client;

    if (!(ftp_client = init_ftp_client(ftp_server, pid, client_socket_fd)))
        return NULL;
    if (NULL == *clients)
        *clients = ftp_client;
    else {
        cur_client = *clients;
        while (cur_client) {
            prev_client = cur_client;
            cur_client = cur_client->next;
        }
        prev_client->next = ftp_client;
    }
    return ftp_client;
}

void                release_ftp_clients(t_ftp_client **clients)
{
    t_ftp_client    *client;
    t_ftp_client    *next;
    t_ftp_client    *prev;
    int             status;
    int             returnv;

    prev = NULL;
    client = *clients;
    while (client) {
        next = client->next;
        if (waitpid(client->pid, &status, WNOHANG) > 0) {
            release_ftp_client(client);
            if (prev)
                prev->next = next;
            else if (!next)
                *clients = NULL;
        }
        prev = client;
        client = next;
    }
}
