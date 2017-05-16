
#include "myftp.h"

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
        ftp_client = new_ftp_client(ftp_server,
                                    &ftp_server->clients,
                                    client_pid,
                                    client_fd);
        if ((client_pid = fork()) == 0) {
            ftp_client_loop(ftp_server, ftp_client);
        } else if (client_pid == -1)
            EXIT_ERROR(0, "fork error : %s\n", strerror(errno))
        else {
            ftp_client->pid = client_pid;
            printf("New Client %s:%d\n",
                   ftp_client->conn_cmd.socket_infos.client_ipv4,
                   ftp_client->conn_cmd.socket_infos.client_port);
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
    printf("Listen on %s:%d\n", ftp_server->listen_address, ftp_server->listen_port);
    returnv = ftp_server_accept_loop(ftp_server);
    if (!socket_close(&ftp_server->server_fd))
        return 0;
    ftp_server->server_fd = -1;
    return returnv;
}

void init_ftp_server(t_ftp_server *ftp_server, char **av) {
    ftp_server->server_fd = -1;
    ftp_server->listen_port = atoi(av[1]);
    ftp_server->listen_address = NET_SERVER_ADDRESS;
    ftp_server->anon_directory_path = av[2];
    ftp_server->running = 0;
    ftp_server->clients = NULL;
    ftp_server->server_pid = getpid();
}