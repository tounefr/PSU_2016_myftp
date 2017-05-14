
#include "myftp.h"

char        *join_path(char *s1, char *s2)
{
    char    *s3;
    int     s1_len;

    if (!(s3 = calloc(1, strlen(s1) + strlen(s2) + 1)))
        EXIT_ERROR(NULL, "malloc error\n");
    if (s1[strlen(s1)-1] == '/' && *s2 == '/')
        s1[strlen(s1)-1] = '\0';
    if (s1[strlen(s1)-1] != '/' && *s2 != '/') {
        s1_len = strlen(s1);
        if (!realloc(s1, s1_len + 2))
            EXIT_ERROR(NULL, "malloc error\n")
        s1[s1_len] = '/';
        s1[s1_len + 1] = '\0';
    }
    strcpy(s3, s1);
    strcpy(&s3[strlen(s1)], s2);
    return s3;
}

static unsigned int     path_count_chars_to_normalize(char *path)
{
    unsigned int        i;
    unsigned int        i2;
    unsigned int        c;

    c = 0;
    i = -1;
    while (++i < strlen(path)) {
        i2 = -1;
        while (++i2 < strlen(PATH_CHARS_TO_NORMALIZE)) {
            if (path[i] == PATH_CHARS_TO_NORMALIZE[i2])
                c++;
        }
    }
    return c;
}

char                *normalize_path(char *path)
{
    unsigned int    count_chars_to_normalize;
    char            *path_normalized;
    unsigned int    i;
    unsigned int    i2;

    count_chars_to_normalize = path_count_chars_to_normalize(path);
    if (!(path_normalized = calloc(1, strlen(path) + count_chars_to_normalize + 1)))
        EXIT_ERROR(NULL, "malloc error\n")
    i = 0;
    i2 = 0;
    while (i2 < strlen(path)) {
        if (in_str(path[i2], PATH_CHARS_TO_NORMALIZE))
            path_normalized[i++] = '\\';
        path_normalized[i++] = path[i2++];
    }
    return path_normalized;
}

char    *path_parent_directory(char *path)
{
    int i;

    i = strlen(path) - 1;
    while (i > 0 && path[i] != '/')
        i--;
    if (strlen(path) > 1 && path[i] == '/')
        path[i] = '\0';
    return strdup(path);
}

char is_absolute_path(char *path) {
    return (path && *path == '/');
}

char        *get_path(char *file, t_ftp_client *ftp_client) {
    char    *tmp;
    char    *orig_file;

    orig_file = file;
    while (file && *file != '\0' && *file != ' ')
        file++;
    file++;
    tmp = file;
    /*
    if (!(file = normalize_path(file)))
        return NULL;
        */
    //free(orig_file);
    if (*file != '/') {
        tmp = file;
        if (!(file = join_path(ftp_client->cwd, file)))
            return NULL;
        //free(tmp);
    }
    printf("test : %s\n", file);
    printf("%s %s\n", ftp_client->home_path, file);
    if (!(file = join_path(ftp_client->home_path, file)))
        return NULL;
    printf("test2 : %s\n", file);
    return file;
}