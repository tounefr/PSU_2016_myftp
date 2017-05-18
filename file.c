/*
** file.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by toune
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:07:11 2017 toune
** Last update Thu May 18 10:07:11 2017 toune
*/

#include "myftp.h"

char            check_directory_exists(char *abs_path) {
    struct stat stat_buf;

    return (stat(abs_path, &stat_buf) != -1 && S_ISDIR(stat_buf.st_mode));
}

char            check_file_exists(char *abs_path) {
    struct stat stat_buf;

    return (stat(abs_path, &stat_buf) != -1 && !S_ISDIR(stat_buf.st_mode));
}
