
#include "myftp.h"

void on_type_list_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 200, "TYPE is now 8-bit binary");
}

void on_ftp_user_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 331, "Please specify the password.");
}

void on_ftp_pass_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 230, "Login successful.");
}

void on_ftp_cwd_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {

    /*
    char *tmp;
    char *dir;
    char *abs_path;
    struct stat stat_buf;
    char *token;
    int i;

    dir = NULL;
    i = 0;
    while ((token = strtok(buffer, " "))) {
        if (buffer)
            buffer = NULL;
        if (i++ == 1) {
            dir = strdup(token);
            //TODO: strdup returnv
        }
    }
    if (!dir) {
        //TODO:
        return;
    }
    tmp = ftp_client->cwd;
    if (!(abs_path = join_path(ftp_client->cwd, dir))) {
        //TODO:
        return;
    }
    if (!(abs_path = join_path(ftp_client->home_path, abs_path))) {
        //TODO:
        return;
    }
    if (stat(abs_path, &stat_buf) == -1 || !(stat_buf.st_mode & S_IFDIR)) {
        //TODO:
        return;
    }
    if (!(ftp_client->cwd = join_path(tmp, dir))) {
        //TODO:
        return;
    }
    */

    char *dir;
    char *abs_path;

    while (buffer && *buffer != '\0' && *buffer != ' ')
        buffer++;
    buffer++;

//    dir = normalize_path(strdup(buffer));
    dir = strdup(buffer);

    //TODO: fuite de mémoire
    if (*buffer == '/')
        ftp_client->cwd = dir;
    else
        ftp_client->cwd = join_path(ftp_client->cwd, dir);

    if (!(abs_path = join_path(ftp_client->home_path, ftp_client->cwd))) {
        //TODO:
        return;
    }
    printf("CWD : %s\n", abs_path);
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 250, "Directory successfully changed.");
}

void on_ftp_cdup_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {
    char *tmp;

    tmp = ftp_client->cwd;
    ftp_client->cwd = path_parent_directory(ftp_client->cwd);
    free(tmp);
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 250, "Directory successfully changed.");
}

void on_ftp_quit_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {
    if (ftp_client->conn_cmd.socket_fd != -1) {
        send_cmd_response(&ftp_client->conn_cmd.socket_fd, 221, "Service closing control connection.");
        socket_close(&ftp_client->conn_cmd.socket_fd);
    }
}

void            on_ftp_dele_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {

    char        *path;
    struct stat stat_buf;
    char        *abs_path;

    while (buffer && *buffer != '\0' && *buffer != ' ')
        buffer++;
    buffer++;

    //TODO: fuite mémoire
    path = strdup(buffer); //TODO:
    if (*path != '/') {
        path = join_path(ftp_client->cwd, path);
    }
    path = join_path(ftp_client->home_path, path);
    if (-1 != stat(path, &stat_buf) && !S_ISDIR(stat_buf.st_mode)) {
        printf("DELETE %s\n", path);
        //TODO: activate
        //unlink(path);
        send_cmd_response(&ftp_client->conn_cmd.socket_fd, 250, "Requested file action okay, completed.");
    }
    else {
        //TODO:
    }
    free(path);
}

void        on_ftp_pwd_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {
    char    *cwd;
    char    cwd_buffer[1000];
    int     buffer_len;

    snprintf(cwd_buffer, sizeof(cwd_buffer) - 1, "\"%s\", is your current location", ftp_client->cwd);
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 257, cwd_buffer);
}

void                on_ftp_pasv_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {
    char            *listen_address;
    char            *pasv_buffer;
    unsigned short  listen_port;

    if (ftp_client->is_logged) {
        //TODO: already logged
        send_cmd_response(&ftp_client->conn_cmd.socket_fd, 500, NULL);
        return;
    }

    if (!(pasv_buffer = calloc(1, 1000)))
        return;
    listen_port = rand_port();
//    listen_address = format_pasv_ipv4_address(ftp_client->conn_cmd.socket_infos.server_ipv4);
    listen_address = "127,0,0,1"; //TODO:
    if (!listen_address) {
        free(pasv_buffer);
        return;
    }
    snprintf(pasv_buffer, 999, "Entering Passive Mode (%s,%d,%d)",
             listen_address,
             (listen_port / 256),
             (listen_port % 256));
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 227, pasv_buffer);
    free(pasv_buffer);

    if (!listen_data_conn(ftp_client, &listen_port)) {
        // TODO:
    }
}

void on_ftp_port_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {

}

void on_ftp_help_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 214, "RTFM");
}

void on_ftp_noo_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 200, "Ok");
}

void        on_ftp_list_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {
    FILE    *ls_f;
    char    ls_buf[256];
    char    *abs_path;
    char    ls_cmd[1000];
    int     i;

    memset(ls_cmd, 0, sizeof(ls_cmd));
    if (!(abs_path = join_path(ftp_client->home_path, ftp_client->cwd))) {
        //TODO:
        return;
    }
    snprintf(ls_cmd, sizeof(ls_cmd) - 1, "ls %s -l", abs_path);
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 150, "Here comes the directory listing.");
    if (!(ls_f = popen(ls_cmd, "r"))) {
        //TODO:
        return;
    }
    memset(ls_buf, 0, sizeof(ls_buf));
    i = 0;
    while (fgets(ls_buf, sizeof(ls_buf) - 1, ls_f)) {
        if (i++ == 0)
            continue;
        printf("%s", ls_buf);
        socket_send(&ftp_client->conn_data.socket_fd, ls_buf);
        memset(ls_buf, 0, sizeof(ls_buf));
    }
    pclose(ls_f);
    socket_close(&ftp_client->conn_data.socket_fd);
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 226, "Directory send OK.");
}