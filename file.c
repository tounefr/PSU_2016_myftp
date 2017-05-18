/*
** file.c for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by Thomas HENON
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:28:39 2017 Thomas HENON
** Last update Thu May 18 10:28:40 2017 Thomas HENON
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
