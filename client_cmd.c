
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
        {"NOO", on_ftp_noo_cmd},
        {"RETR", on_ftp_retr_cmd},
        {"STOR", on_ftp_stor_cmd},
        {"LIST", on_ftp_list_cmd}
};

void on_ftp_user_cmd(t_ftp_client *ftp_client, char *buffer) {

}

void on_ftp_pass_cmd(t_ftp_client *ftp_client, char *buffer) {

}

void on_ftp_cwd_cmd(t_ftp_client *ftp_client, char *buffer) {

}

void on_ftp_cdup_cmd(t_ftp_client *ftp_client, char *buffer) {

}

void on_ftp_quit_cmd(t_ftp_client *ftp_client, char *buffer) {

}

void on_ftp_dele_cmd(t_ftp_client *ftp_client, char *buffer) {

}

void on_ftp_pwd_cmd(t_ftp_client *ftp_client, char *buffer) {

}

void on_ftp_pasv_cmd(t_ftp_client *ftp_client, char *buffer) {

}

void on_ftp_port_cmd(t_ftp_client *ftp_client, char *buffer) {

}

void on_ftp_help_cmd(t_ftp_client *ftp_client, char *buffer) {

}

void on_ftp_noo_cmd(t_ftp_client *ftp_client, char *buffer) {

}

void on_ftp_retr_cmd(t_ftp_client *ftp_client, char *buffer) {

}

void on_ftp_stor_cmd(t_ftp_client *ftp_client, char *buffer) {

}

void on_ftp_list_cmd(t_ftp_client *ftp_client, char *buffer) {

}
