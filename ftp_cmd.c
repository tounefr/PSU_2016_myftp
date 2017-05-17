
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
    if (ftp_client->user)
        free(ftp_client->user);
    printf("Login : %s\n", user);
    ftp_client->user = user;
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 331,
                      "Please specify the password.");
}

void on_ftp_pass_cmd(t_ftp_server *ftp_server,
                     t_ftp_client *ftp_client,
                     char *buffer)
{
    if (ftp_client->user == NULL) {
        send_cmd_response(&ftp_client->conn_cmd.socket_fd, 332,
                          "Need login first.");
        return;
    }
    if (!strcasecmp(ftp_client->user, "anonymous")) {
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
    char    *buffer_path;
    char    *abs_path;

    if (!(buffer_path = get_path_from_buffer(buffer)))
        fatal_error(ftp_client);
    file_path = buffer_path;
    if (!is_absolute_path(buffer_path)) {
        if (!(file_path = join_path(ftp_client->cwd, file_path)))
            fatal_error(ftp_client);
    }
    if (!(abs_path = join_path(ftp_client->home_path, file_path)))
        fatal_error(ftp_client);
    if (!check_directory_exists(abs_path)) {
        send_cmd_response(&ftp_client->conn_cmd.socket_fd, 550,
                          "File not found.");
        return;
    }
    if (!(ftp_client->cwd = strdup(file_path)))
        fatal_error(ftp_client);
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

    pasv_buffer = my_malloc(1000);
    listen_port = rand_port();
    listen_address = format_pasv_ipv4_address(ftp_client->conn_cmd.socket_infos.server_ipv4);
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

char    ftp_connect(t_ftp_client *ftp_client,
                 char *ip,
                 unsigned short *port)
{
    int fd;

    if (!socket_init(&fd))
        return 0;
    if (!socket_connect(&fd, ip, port))
        return 0;
    ftp_client->conn_data.socket_fd = fd;
    if (!socket_infos(&fd, &ftp_client->conn_data.socket_infos))
        return 0;
    return 1;
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

void on_ftp_help_cmd(t_ftp_server *ftp_server,
                     t_ftp_client *ftp_client,
                     char *buffer)
{
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 214,
                      "RTFM");
}

void on_ftp_noo_cmd(t_ftp_server *ftp_server,
                    t_ftp_client *ftp_client,
                    char *buffer)
{
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 200,
                      "Ok");
}


static char ftp_send_list_file(char *abs_path,
                               t_ftp_client *ftp_client)
{
    int     i;
    char    ls_buf[256];
    char    ls_cmd[1000];
    FILE    *ls_f;

    memset(ls_cmd, 0, sizeof(ls_cmd));
    memset(ls_buf, 0, sizeof(ls_buf));
    snprintf(ls_cmd, sizeof(ls_cmd) - 1, "ls %s -l", abs_path);
    if (!(ls_f = popen(ls_cmd, "r")))
        EXIT_ERROR(0, "popen error : %s\n", strerror(errno))
    i = 0;
    while (fgets(ls_buf, sizeof(ls_buf) - 1, ls_f)) {
        if (i++ == 0)
            continue;
        printf("%s", ls_buf);
        if (!(socket_send(&ftp_client->conn_data.socket_fd, ls_buf))) {
            pclose(ls_f);
            return 0;
        }
        memset(ls_buf, 0, sizeof(ls_buf));
    }
    pclose(ls_f);
    return 1;
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
