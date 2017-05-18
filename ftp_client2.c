/*
** ftp_client2.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by Thomas HENON
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:31:23 2017 Thomas HENON
** Last update Thu May 18 10:31:23 2017 Thomas HENON
*/

#include "myftp.h"

char        ftp_send_list_file(char *abs_path, t_ftp_client *ftp_client)
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
        if (!(socket_send(&ftp_client->conn_data.socket_fd, ls_buf))) {
            pclose(ls_f);
            return 0;
        }
        memset(ls_buf, 0, sizeof(ls_buf));
    }
    pclose(ls_f);
    return 1;
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
