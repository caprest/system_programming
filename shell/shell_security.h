#ifndef __SHELL_SECURITY_H__
#define __SHELL_SECURITY_H__
#include <seccomp.h>

typedef struct  {
    int network;
    int w_stdout;
    int w_redirect;
} secoption;

secoption init_secoption();
void set_secoption(secoption *, int ,int , int );
int set_seccomp(secoption *);
int seccomp_forbid_network(scmp_filter_ctx *);
int seccomp_forbid_write(scmp_filter_ctx*,secoption * );
#endif