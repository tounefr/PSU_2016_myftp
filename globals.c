/*
** globals.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by Thomas HENON
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:29:43 2017 Thomas HENON
** Last update Thu May 18 10:29:44 2017 Thomas HENON
*/

#include "myftp.h"

t_command_func g_cs_cmd[CS_CMD_N] = {
    {"USER",    on_ftp_user_cmd,    0},
    {"PASS",    on_ftp_pass_cmd,    0},
    {"CWD",     on_ftp_cwd_cmd,     PRECHECK_LOGGED},
    {"CDUP",    on_ftp_cdup_cmd,    PRECHECK_LOGGED},
    {"QUIT",    on_ftp_quit_cmd,    0},
    {"DELE",    on_ftp_dele_cmd,    PRECHECK_LOGGED},
    {"PWD",     on_ftp_pwd_cmd,     PRECHECK_LOGGED},
    {"PASV",    on_ftp_pasv_cmd,    PRECHECK_LOGGED},
    {"PORT",    on_ftp_port_cmd,    PRECHECK_LOGGED},
    {"HELP",    on_ftp_help_cmd,    0},
    {"NOOP",    on_ftp_noo_cmd,     0},
    {"RETR",    on_ftp_retr_cmd,    PRECHECK_LOGGED},
    {"STOR",    on_ftp_stor_cmd,    PRECHECK_LOGGED},
    {"LIST",    on_ftp_list_cmd,    PRECHECK_LOGGED},
    {"TYPE",    on_type_list_cmd,   PRECHECK_LOGGED}
};

t_ftp_server *g_ftp_server = NULL;
t_ftp_client *g_cur_client = NULL;