
#include "myftp.h"

static char *allocate_new_command_packet(char *buffer,
                                  unsigned int buffer_size,
                                  unsigned int limit)
{
    char    *packet;

    if (NULL == (packet = malloc(limit + 1)))
        EXIT_ERROR(NULL, "malloc error\n")
    return memcpy(packet, buffer, limit);
}

/*
char        *ftp_recv_packet_command(int *fd)
{
    char    *buffer;
    int     i;

    if (!(buffer = calloc(1, NET_BUFFER_SIZE)))
        EXIT_ERROR(NULL, "malloc error\n")
    if (-1 == read(*fd, buffer, NET_BUFFER_SIZE - 1))
        EXIT_ERROR(NULL, "read error %s\n", strerror(errno))
    i = -1;

    for (int i2 = 0; i2 < NET_BUFFER_SIZE; i2++) {
        printf("%d ", buffer[i2]);
    }
    printf("\n");

    while (++i < strlen(buffer)) {
        if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
            buffer[i] = '\0';
            return buffer;
        }
    }
    return NULL;
}
 */

int     index_delimiter_in_buffer(char *buffer) {
    int i;

    i = -1;
    while (buffer[++i]) {
        if (buffer[i] == '\r' && buffer[i + 1] == '\n')
            return i;
    }
    return -1;
}

char extend_buffer(char *buffer, int *buffer_size) {
    *buffer_size += NET_BUFFER_SIZE;
    if (!realloc(buffer, *buffer_size))
        return 0;
    memset(&buffer[*buffer_size - NET_BUFFER_SIZE], 0, NET_BUFFER_SIZE);
    return 1;
}

char        *extract_packet_and_save(char **buffer, int *index_delimiter) {
    char    *packet;
    char    *buffer_rest;
    int     buffer_rest_size;

    packet = my_malloc(*index_delimiter + 1);
    memcpy(packet, *buffer, *index_delimiter);
    buffer_rest_size = strlen(&(*buffer)[*index_delimiter + 2]);
    buffer_rest = my_malloc(buffer_rest_size + 1);
    memcpy(buffer_rest, &(*buffer)[*index_delimiter + 2], buffer_rest_size);
    free(*buffer);
    *buffer = buffer_rest;
    return packet;
}

char            *ftp_recv_packet_command(int *fd) {
    static char *buffer = NULL;
    char        *packet;
    static int  buffer_size = NET_BUFFER_SIZE;;
    int         readv;
    int         index_delimiter;

    if (!buffer)
        buffer = my_malloc(NET_BUFFER_SIZE);
    read(*fd, &buffer[buffer_size - NET_BUFFER_SIZE],
                      NET_BUFFER_SIZE - 1);
    if (strlen(buffer) == 0)
        return NULL;
    if ((index_delimiter = index_delimiter_in_buffer(buffer)) == -1) {
        if (!extend_buffer(buffer, &buffer_size))
            EXIT_ERROR(0, "malloc error\n")
        return ftp_recv_packet_command(fd);
    }
    if (!(packet = extract_packet_and_save(&buffer, &index_delimiter)))
        EXIT_ERROR(0, "malloc error\n")
    if (strlen(buffer) == 0) {
        free(buffer);
        buffer_size = NET_BUFFER_SIZE;
        buffer = NULL;
    }
    return packet;
}

char    socket_data_conn(t_ftp_client *ftp_client)
{
    int fd;

    if (!socket_init(&fd))
        return 0;
    if (!socket_accept(&fd, &ftp_client->conn_data.socket_fd))
        return 0;
    socket_close(&fd);
    return 1;
}

char                listen_data_conn(t_ftp_client *ftp_client, unsigned short *listen_port)
{
    char            *pasv_buffer;
    pid_t           child_pid;
    int             server_fd;

    if (ftp_client->conn_data.socket_fd != -1)
        socket_close(&ftp_client->conn_data.socket_fd);
    *listen_port = rand_port();
    if (!socket_init(&server_fd))
        EXIT_ERROR(0, "%s\n", strerror(errno))
    if (!socket_listen(&server_fd, NET_SERVER_ADDRESS, listen_port))
        EXIT_ERROR(0, "%s\n", strerror(errno))
    if (!socket_accept(&server_fd, &ftp_client->conn_data.socket_fd))
        EXIT_ERROR(0, "%s\n", strerror(errno))
    printf("New data conn\n");
    socket_close(&server_fd);
    return 1;
}

unsigned short      rand_port()
{
    static char     init = 0;
    int             min;
    int             max;
    unsigned short  num;

    if (!init)
        srand(time(NULL));
    min = 1025;
    max = 65535;
    num = (rand() % max) + min + 1;
    if (socket_port_used(num))
        return rand_port();
    return num;
}