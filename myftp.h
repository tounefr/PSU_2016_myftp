#ifndef PROJETS_MYFTP_H
#define PROJETS_MYFTP_H

#include "socket.h"

#define EXIT_ERROR(returnv, ...) {fprintf(stderr, __VA_ARGS__); return returnv;}

char exit_error(int returnv);

#endif //PROJETS_MYFTP_H
