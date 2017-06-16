#ifndef _SYSTEST_H
#define _SYSTEST_H
#define BUFSIZE 1024
#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)
void fd_check(int ,char *name);
int  is_space(char);
#endif