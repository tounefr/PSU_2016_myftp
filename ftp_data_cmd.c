
#include "myftp.h"

char            ftp_send_file(char *file, int *socket_fd)
{
    char        *abs_path;
    struct stat stat_buff;
    char        file_buffer[1024];
    int         readv;
    int         file_fd;

    if (-1 == (file_fd = open(file, O_RDONLY)))
        EXIT_ERROR(0, "open error %s : %s\n", file, strerror(errno))
    while ((readv = read(file_fd, file_buffer, sizeof(file_buffer))) > 0) {
        if (-1 == write(*socket_fd, file_buffer, readv)) {
            if (-1 == close(file_fd))
                EXIT_ERROR(0, "close error : %s\n", strerror(errno))
            EXIT_ERROR(0, "write error : %s\n", strerror(errno))
        }
    }
    if (readv == -1)
        EXIT_ERROR(0, "read error : %s\n", strerror(errno))
    if (-1 == close(file_fd))
        EXIT_ERROR(0, "close error : %s\n", strerror(errno))
    return 1;
}

char            ftp_recv_file(char *file, int *socket_fd)
{
    char        *abs_path;
    struct stat stat_buff;
    char        file_buffer[1024];
    int         readv;
    int         file_fd;

    if (-1 == (file_fd = open(file, O_RDWR|O_CREAT, 0666)))
        EXIT_ERROR(0, "open error : %s\n", strerror(errno))
    while ((readv = read(*socket_fd, file_buffer, sizeof(file_buffer))) > 0) {
        if (-1 == write(file_fd, file_buffer, readv))
            EXIT_ERROR(0, "write error : %s\n", strerror(errno))
    }
    if (readv == -1)
        EXIT_ERROR(0, "read error : %s\n", strerror(errno))
    if (-1 == close(file_fd))
        EXIT_ERROR(0, "close error : %s\n", strerror(errno));
    return 1;
}

void        on_ftp_retr_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer)
{
    char    *abs_path;

    if (!(abs_path = get_path(buffer, ftp_client)))
        fatal_error(ftp_client);
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 150, NULL);
    if (!ftp_send_file(abs_path, &ftp_client->conn_data.socket_fd))
        fatal_error(ftp_client);
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 226, "Transfer complete.");
    socket_close(&ftp_client->conn_data.socket_fd);
}

void        on_ftp_stor_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {
    char    *abs_path;

    if (!(abs_path = get_path(buffer, ftp_client)))
        fatal_error(ftp_client);
    /*if (check_file_exists(abs_path) || check_directory_exists(abs_path)) {
        //TODO: file already exist
        printf("file already exist\n");
        return;
    }*/
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 150, "Accepted data connection");
    if (!ftp_recv_file(abs_path, &ftp_client->conn_data.socket_fd))
        fatal_error(ftp_client);
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 226, "File successfully transferred");
    socket_close(&ftp_client->conn_data.socket_fd);
}
