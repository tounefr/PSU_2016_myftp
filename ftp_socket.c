
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "myftp.h"

static char *allocate_new_command_packet(char *buffer,
                                  unsigned int buffer_size,
                                  unsigned int limit) {
    char *packet;

    if (NULL == (packet = malloc(limit + 1))) {
        fprintf(stderr, "malloc error\n");
        return NULL;
    }
    return memcpy(packet, buffer, limit);
}

char                *ftp_recv_packet_command(int *fd)
{
    static char     *buffer = NULL;
    unsigned int    buffer_size = NET_BUFFER_SIZE;
    int             readed;
    unsigned int    i;

    if (buffer == NULL && (NULL == (buffer = malloc(NET_BUFFER_SIZE)))) {
        fprintf(stderr, "Malloc error\n");
        return 0;
    }
    if ((readed = read(*fd, buffer, NET_BUFFER_SIZE)) == -1) {
        fprintf(stderr, "read error : %s\n", strerror(errno));
        return NULL;
    }
    /*
    i = 0;
    while (i < readed) {
        printf("%d ", buffer[i++]);
    }
    printf("\n");
     */
    i = 0;
    while (i + 1 < readed) {
        if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
            buffer[i] = '\0';
            return buffer;
        }
        i += 2;
    }
    return NULL;
}