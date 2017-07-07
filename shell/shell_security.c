#include <seccomp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "shell_security.h"

secoption init_secoption(){
    secoption option;
    option.network = 0;
    option.w_stdout = 0;
    option.w_redirect =0;
    return option;
}

void set_secoption(secoption *option, int network ,int stdout, int redirect){
    option->network =  network?1:0;
    option->w_stdout =  stdout?1:0;
    option->w_redirect = redirect?1:0;
}

int set_seccomp(secoption *option, int fd){
    scmp_filter_ctx ctx;
    ctx = seccomp_init(SCMP_ACT_ALLOW);
    if (option->network) seccomp_forbid_network(ctx);
    if (option->w_stdout || option->w_redirect) seccomp_forbid_write(ctx,option,fd);
    int ret = seccomp_load(ctx);
    return ret;
}

int  seccomp_forbid_network(scmp_filter_ctx *ctx){
    int rc = -1;
    rc = seccomp_rule_add(ctx,SCMP_ACT_KILL,SCMP_SYS(socket),0);
    return rc;
}

int seccomp_forbid_write(scmp_filter_ctx *ctx, secoption *option, int fd){
    int rc = 1;
    if (option->w_stdout){
        rc = seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(write), 1,SCMP_CMP(0, SCMP_CMP_NE, 1));
        //rc = seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(open), 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_WRONLY, O_WRONLY));
    }
    else if (option->w_redirect){
        rc = seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(write), 1,SCMP_CMP(0, SCMP_CMP_NE, fd));
    }
    return rc;
}
