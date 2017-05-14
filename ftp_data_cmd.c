
#include "myftp.h"

char            ftp_send_file(char *file, int *socket_fd) {
    char        *abs_path;
    struct stat stat_buff;
    char        file_buffer[1024];
    int         readv;
    int         file_fd;

    if (-1 == (file_fd = open(file, O_RDONLY)))
        EXIT_ERROR(0, "%s\n", strerror(errno))
    while ((readv = read(file_fd, file_buffer, sizeof(file_buffer))) > 0) {
        if (-1 == write(*socket_fd, file_buffer, readv)) {
            //TODO:
            return 0;
        }
    }
    if (readv == -1)
        EXIT_ERROR(0, "%s\n", strerror(errno))
    close(file_fd);
    return 1;
}

void        on_ftp_retr_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {
    char    *abs_path;

    if (!(abs_path = get_path(buffer, ftp_client)))
        fatal_error(ftp_client);
    if (!ftp_send_file(abs_path, &ftp_client->conn_data.socket_fd))
        fatal_error(ftp_client);
    send_cmd_response(&ftp_client->conn_cmd.socket_fd, 226, "226 Transfer complete.");
    socket_close(&ftp_client->conn_data.socket_fd);
}

void on_ftp_stor_cmd(t_ftp_server *ftp_server, t_ftp_client *ftp_client, char *buffer) {

}
