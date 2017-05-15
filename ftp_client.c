
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
    if (NULL == (ftp_client = malloc(sizeof(t_ftp_client))))
        EXIT_ERROR(NULL, "malloc error\n")
    ftp_client->pid = pid;
    ftp_client->conn_cmd.socket_fd = client_socket_fd;
    ftp_client->is_logged = 0;
    ftp_client->cwd = NULL;
    ftp_client->next = NULL;
    ftp_client->user = strdup("anonymous");
    ftp_client->home_path = strdup(ftp_server->anon_directory_path);
    ftp_client->cwd = strdup("/");
    ftp_client->conn_data.socket_fd = -1;
    if (!ftp_client->user || !ftp_client->home_path || !ftp_client->cwd)
        EXIT_ERROR(NULL, "malloc error\n");
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

    if (!(ftp_client = init_ftp_client(ftp_server, pid, client_socket_fd)))
        return NULL;
    if (NULL == *clients)
        *clients = ftp_client;
    else {
        cur_client = *clients;
        while (cur_client->next)
            cur_client = cur_client->next;
        cur_client->next = ftp_client;
    }
    return ftp_client;
}

void                release_ftp_clients(t_ftp_client **clients)
{
    t_ftp_client    *client;
    t_ftp_client    *next;
    t_ftp_client    *prev;
    int             status;
    int             pid;

    prev = NULL;
    client = *clients;
    while (client) {
        next = client->next;
        if (-1 == (pid = waitpid(client->pid, &status, WNOHANG))
            || pid > 0
            || !WIFEXITED(status)) {
            release_ftp_client(client);
            if (prev)
                prev->next = next;
            else
                *clients = NULL;
        }
        else
            printf("client pid=%d connected\n", client->pid);
        prev = client;
        client = next;
    }
}
