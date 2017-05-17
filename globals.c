
#include "myftp.h"

t_command_func g_cs_cmd[CS_CMD_N] = {
    {"USER",    on_ftp_user_cmd,    0},
    {"PASS",    on_ftp_pass_cmd,    0},
    {"CWD",     on_ftp_cwd_cmd,     PRECHECK_LOGGED},
    {"CDUP",    on_ftp_cdup_cmd,    PRECHECK_LOGGED},
    {"QUIT",    on_ftp_quit_cmd,    PRECHECK_LOGGED},
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

t_ftp_client *cur_client = NULL;
t_ftp_server *g_ftp_server = NULL;
