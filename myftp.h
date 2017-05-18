/*
** myftp.h for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
** 
** Made by Thomas HENON
** Login   <thomas.henon@epitech.eu>
** 
** Started on  Thu May 18 10:32:47 2017 Thomas HENON
** Last update Thu May 18 10:32:47 2017 Thomas HENON
*/

#ifndef PROJETS_MYFTP_H
#define PROJETS_MYFTP_H

#include "socket.h"

#define EXIT_ERROR(returnv, ...) {fprintf(stderr, __VA_ARGS__); return returnv;}

char exit_error(int returnv);

#endif //PROJETS_MYFTP_H
