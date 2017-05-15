
#ifndef PROJETS_SOCKET_H
#define PROJETS_SOCKET_H

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

# define NET_SERVER_ADDRESS "0.0.0.0"
# define NET_BUFFER_SIZE 4096
# define NET_BACKLOG 2000

# define FTP_HELLO_MESSAGE "ftpserver"
# define FTP_ANONYMOUS_LOGIN "Anonymous"

# define PATH_CHARS_TO_NORMALIZE " \""

typedef struct s_socket_infos
{
    char            *client_ipv4;
    unsigned short  client_port;
    char            *server_ipv4;
    unsigned short  server_port;
} t_socket_infos;

typedef struct s_ftp_client_conn
{
    int                 socket_fd;
    t_socket_infos      socket_infos;
} t_ftp_client_conn;

typedef struct s_ftp_client
{
    pid_t               pid;

    t_ftp_client_conn   conn_cmd;
    t_ftp_client_conn   conn_data;

    char                *home_path;
    char                *cwd;
    char                *user;
    char                *pass;
    char                is_logged;
    struct s_ftp_client *next;
} t_ftp_client;

typedef struct s_ftp_server
{
    char *anon_directory_path;
    char *listen_address;
    unsigned short listen_port;
    int server_fd;
    char running;
    pid_t server_pid;
    t_ftp_client *clients;
} t_ftp_server;

typedef struct s_command_func
{
    char *cmd;
    void (*callback)(t_ftp_server*, t_ftp_client*, char*);
} t_command_func;

extern t_ftp_server ftp_server;
extern t_ftp_client *cur_client;
extern t_ftp_server *g_ftp_server;


// socket.c
char socket_init(int *fd);
char socket_accept(int *server_socket, int *client_socket);
char                    socket_listen(int *server_fd,
                                      char *listen_address,
                                      unsigned short *listen_port);
char socket_close(int *fd);
char socket_infos(int *socket_fd, t_socket_infos *socket_infos);
char socket_send(int *fd, char *buffer);
char socket_port_used(unsigned short port);

// protocol.c
char ftp_client_loop(t_ftp_server *ftp_server, t_ftp_client *ftp_client);

// ftp_socket.c

char *ftp_recv_packet_command(int *fd);
char send_cmd_response(int *fd, int code, char *content);
unsigned short rand_port();
char listen_data_conn(t_ftp_client *ftp_client, unsigned short *listen_port);
char send_data_file(t_ftp_client *ftp_client, char *abs_path);

// client_cmd.c

# define CS_CMD_N 15
extern t_command_func g_cs_cmd[];

void on_ftp_user_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer);
void on_ftp_pass_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer);
void on_ftp_cwd_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer);
void on_ftp_cdup_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer);
void on_ftp_quit_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer);
void on_ftp_dele_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer);
void on_ftp_pwd_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer);
void on_ftp_pasv_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer);
void on_ftp_port_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer);
void on_ftp_help_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer);
void on_ftp_noo_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer);
void on_ftp_retr_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer);
void on_ftp_stor_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer);
void on_ftp_list_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer);
void on_type_list_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer);

// util.c
char exit_error(int returnv);
char *format_pasv_ipv4_address(char *ipv4);
char *join_path(char *s1, char *s2);
char *path_parent_directory(char *path);
char in_str(char c, char *tokens);
char *normalize_path(char *path);
void fatal_error(t_ftp_client *ftp_client);
char  extract_ip_port(char *str);
char check_directory_exists(char *abs_path);
char        check_file_exists(char *abs_path);
char *get_path_from_buffer(char *buffer);

// ftp.c
void free_ftp_client(t_ftp_client *ftp_client);
void release_ftp_client(t_ftp_client *client);
void release_ftp_clients(t_ftp_client **clients);
t_ftp_client        *init_ftp_client(
        t_ftp_server *ftp_server,
        pid_t pid, int client_socket_fd);
t_ftp_client        *new_ftp_client(
        t_ftp_server *ftp_server,
        t_ftp_client **clients,
        pid_t pid,
        int client_socket_fd);


// ftp_server.c
char                ftp_server_accept_loop(t_ftp_server *ftp_server);
char        start_ftp_server(t_ftp_server *ftp_server);
void init_ftp_server(t_ftp_server *ftp_server, char **av);

// path.c
char is_absolute_path(char *path);

char *get_path(char *file, t_ftp_client *ftp_client);


#endif //PROJETS_SOCKET_H
