
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include "myftp.h"

t_ftp_server ftp_server;

void sig_handler(int signum) {
    printf("\nCatched signum %d\n", signum);
    /*if (ftp_server.server_fd != -1) {
        if (socket_close(&g_server_socket))
            printf("Server socket closed\n");
    }*/
    exit(0);
}

char usage() {
    printf("Usage: ./server port path\n");
    return 1;
}

void                release_ftp_clients(t_ftp_client **clients) {
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
            printf("client pid=%d exited : %s\n", client->pid, strerror(errno));
            if (client->socket_fd != -1)
                socket_close(&client->socket_fd);
            if (prev)
                prev->next = next;
            else
                *clients = NULL;
            free(client);
        }
        else
            printf("client pid=%d connected\n", client->pid);
        prev = client;
        client = next;
    }
}

t_ftp_client        *new_ftp_client(t_ftp_client **clients,
                                    pid_t *pid,
                                    int *client_socket_fd)
{
    t_ftp_client    *ftp_client;
    t_ftp_client    *cur_client;

    ftp_client = NULL;
    if (NULL == (ftp_client = malloc(sizeof(t_ftp_client)))) {
        printf("malloc error\n");
        return 0;
    }
    ftp_client->pid = *pid;
    ftp_client->socket_fd = *client_socket_fd;
    ftp_client->is_logged = 0;
    ftp_client->cwd = NULL;
    ftp_client->next = NULL;
    if (!socket_infos(&ftp_client->socket_fd, &ftp_client->socket_infos))
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

char                ftp_server_accept_loop(t_ftp_server *ftp_server)
{
    pid_t           client_pid;
    int             status;
    pid_t           client_fd;
    t_ftp_client    *ftp_client;

    while (1) {
        release_ftp_clients(&ftp_server->clients);
        if (!socket_accept(&ftp_server->server_fd, &client_fd))
            return 0;
        ftp_client = new_ftp_client(&ftp_server->clients,
                                    &client_pid,
                                    &client_fd);
        if ((client_pid = fork()) == 0) {
            ftp_client_loop(ftp_server, ftp_client);
        } else if (client_pid == -1) {
            printf("Failed to fork !\n");
            usleep(5000); // To prevent flood
        }
        else {
            printf("New Client %s:%d\n",
                   ftp_client->socket_infos.ipv4_ip,
                   ftp_client->socket_infos.port);
        }
    }
}

char        start_ftp_server(t_ftp_server *ftp_server)
{
    char    returnv;

    if (!socket_init(&ftp_server->server_fd))
        return 0;
    if (!socket_listen(&ftp_server->server_fd,
                       ftp_server->listen_address,
                       &ftp_server->listen_port))
        return 0;
    ftp_server->running = 1;
    printf("Listen on 0.0.0.0:8889\n");
    returnv = ftp_server_accept_loop(ftp_server);
    if (!socket_close(&ftp_server->server_fd))
        return 0;
    ftp_server->server_fd = -1;
    return returnv;
}

int main(int ac, char **av) {
    if (ac != 3)
        return usage();
    ftp_server.server_fd = -1;
    ftp_server.listen_port = atoi(av[1]);
    ftp_server.listen_address = NET_SERVER_ADDRESS;
    ftp_server.anon_directory_path = av[2];
    ftp_server.running = 0;
    ftp_server.clients = NULL;
    if (SIG_ERR == signal(SIGINT, sig_handler))
        return exit_error(1);
    if (!start_ftp_server(&ftp_server))
        return 1;
    return 0;
}