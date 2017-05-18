/*
** myftp.h for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by toune
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:06:39 2017 toune
** Last update Thu May 18 10:06:39 2017 toune
*/

#ifndef PROJETS_MYFTP_H
#define PROJETS_MYFTP_H

#include "socket.h"

#define EXIT_ERROR(returnv, ...) {fprintf(stderr, __VA_ARGS__); return returnv;}

char exit_error(int returnv);

#endif //PROJETS_MYFTP_H
