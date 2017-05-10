
#include <errno.h>
#include <string.h>
#include <stdio.h>

char exit_error(int returnv) {
    printf("%s\n", strerror(errno));
    return returnv;
}
