/*
** ftp_socket2.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by Thomas HENON
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:29:27 2017 Thomas HENON
** Last update Thu May 18 10:29:28 2017 Thomas HENON
*/

#include "myftp.h"

static char *allocate_new_command_packet(char *buffer,
                                         unsigned int buffer_size,
                                         unsigned int limit)
{
    char    *packet;

    if (NULL == (packet = malloc(limit + 1)))
        malloc_error();
    return memcpy(packet, buffer, limit);
}

static int     index_delimiter_in_buffer(char *buffer) {
    int i;

    i = -1;
    while (buffer[++i]) {
        if (buffer[i] == '\r' && buffer[i + 1] == '\n')
            return i;
    }
    return -1;
}

static char extend_buffer(char **buffer, int *buffer_size) {
    *buffer_size += NET_BUFFER_SIZE;
    if (!(*buffer = realloc(*buffer, *buffer_size)))
        malloc_error();
    memset(&(*buffer[*buffer_size - NET_BUFFER_SIZE]), 0, NET_BUFFER_SIZE);
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
    static int  buffer_size = NET_BUFFER_SIZE;
    int         readv;
    int         index_delimiter;

    if (!buffer)
        buffer = my_malloc(NET_BUFFER_SIZE);
    read(*fd, &buffer[buffer_size - NET_BUFFER_SIZE],
         NET_BUFFER_SIZE - 1);
    if (strlen(buffer) == 0)
        return NULL;
    if ((index_delimiter = index_delimiter_in_buffer(buffer)) == -1) {
        extend_buffer(&buffer, &buffer_size);
        return ftp_recv_packet_command(fd);
    }
    packet = extract_packet_and_save(&buffer, &index_delimiter);
    if (strlen(buffer) == 0) {
        free(buffer);
        buffer_size = NET_BUFFER_SIZE;
        buffer = NULL;
    }
    return packet;
}
