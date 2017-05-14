
#include "myftp.h"

static void sig_handler(int signum)
{
    printf("\nCatched signum %d\n", signum);
    if (g_ftp_server) {
        if (getpid() == g_ftp_server->server_pid) {
            t_ftp_client *client;
            client = g_ftp_server->clients;
            while (client) {
                socket_close(&client->conn_cmd.socket_fd);
                client = client->next;
            }
            if (g_ftp_server->server_fd != -1)
                socket_close(&g_ftp_server->server_fd);
        }
    }
    exit(0);
}

static char usage()
{
    printf("Usage: ./server port path\n");
    return 1;
}

int                 main(int ac, char **av)
{
    t_ftp_server    ftp_server;

    if (ac != 3)
        return usage();
    init_ftp_server(&ftp_server, av);
    g_ftp_server = &ftp_server;
    if (SIG_ERR == signal(SIGINT, sig_handler))
        return exit_error(1);
    if (!start_ftp_server(&ftp_server))
        return 1;
    return 0;
}