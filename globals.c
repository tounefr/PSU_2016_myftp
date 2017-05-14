
#include "myftp.h"

t_command_func g_cs_cmd[CS_CMD_N] = {
    {"USER", on_ftp_user_cmd},
    {"PASS", on_ftp_pass_cmd},
    {"CWD", on_ftp_cwd_cmd},
    {"CDUP", on_ftp_cdup_cmd},
    {"QUIT", on_ftp_quit_cmd},
    {"DELE", on_ftp_dele_cmd},
    {"PWD", on_ftp_pwd_cmd},
    {"PASV", on_ftp_pasv_cmd},
    {"PORT", on_ftp_port_cmd},
    {"HELP", on_ftp_help_cmd},
    {"NOOP", on_ftp_noo_cmd},
    {"RETR", on_ftp_retr_cmd},
    {"STOR", on_ftp_stor_cmd},
    {"LIST", on_ftp_list_cmd},
    {"TYPE", on_type_list_cmd}
};

t_ftp_client *cur_client = NULL;
t_ftp_server *g_ftp_server = NULL;
