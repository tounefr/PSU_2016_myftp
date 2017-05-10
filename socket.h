
#ifndef PROJETS_SOCKET_H
#define PROJETS_SOCKET_H

# define NET_SERVER_ADDRESS "0.0.0.0"
# define NET_BUFFER_SIZE 4096
# define NET_BACKLOG 2000

# define FTP_HELLO_MESSAGE "ftpserver"
typedef struct s_socket_infos
{
    char *ipv4_ip;
    unsigned short port;
} t_socket_infos;

typedef struct s_ftp_client
{
    int pid;
    int socket_fd;
    t_socket_infos socket_infos;
    char *cwd;
    char is_logged;
    struct s_ftp_client *next;
} t_ftp_client;

typedef struct s_ftp_server
{
    char *anon_directory_path;
    char *listen_address;
    unsigned short listen_port;
    int server_fd;
    char running;
    t_ftp_client *clients;
} t_ftp_server;

typedef struct s_command_func
{
    char *cmd;
    void (*callback)(t_ftp_client*, char*);
} t_command_func;

extern t_ftp_server ftp_server;

// socket.c
char socket_init(int *fd);
char socket_accept(int *server_socket, int *client_socket);
char                    socket_listen(int *server_fd,
                                      char *listen_address,
                                      unsigned short *listen_port);
char socket_close(int *fd);
char socket_infos(int *socket_fd, t_socket_infos *socket_infos);
char socket_send(int *fd, char *buffer);

// protocol.c
char ftp_client_loop(t_ftp_server *ftp_server, t_ftp_client *ftp_client);

// ftp_socket.c

char *ftp_recv_packet_command(int *fd);
char send_cmd_response(int *fd, int code, char *content);

// client_cmd.c

# define CS_CMD_N 14
extern t_command_func g_cs_cmd[];

void on_ftp_user_cmd(t_ftp_client *ftp_client, char *buffer);
void on_ftp_pass_cmd(t_ftp_client *ftp_client, char *buffer);
void on_ftp_cwd_cmd(t_ftp_client *ftp_client, char *buffer);
void on_ftp_cdup_cmd(t_ftp_client *ftp_client, char *buffer);
void on_ftp_quit_cmd(t_ftp_client *ftp_client, char *buffer);
void on_ftp_dele_cmd(t_ftp_client *ftp_client, char *buffer);
void on_ftp_pwd_cmd(t_ftp_client *ftp_client, char *buffer);
void on_ftp_pasv_cmd(t_ftp_client *ftp_client, char *buffer);
void on_ftp_port_cmd(t_ftp_client *ftp_client, char *buffer);
void on_ftp_help_cmd(t_ftp_client *ftp_client, char *buffer);
void on_ftp_noo_cmd(t_ftp_client *ftp_client, char *buffer);
void on_ftp_retr_cmd(t_ftp_client *ftp_client, char *buffer);
void on_ftp_stor_cmd(t_ftp_client *ftp_client, char *buffer);
void on_ftp_list_cmd(t_ftp_client *ftp_client, char *buffer);

#endif //PROJETS_SOCKET_H
