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

int set_seccomp(secoption *option){
    scmp_filter_ctx ctx;
    ctx = seccomp_init(SCMP_ACT_ALLOW);
    int rc = 0;
    if (option->network)rc =  seccomp_forbid_network(ctx);
    if (rc < 0) return -1;
    if (option->w_stdout || option->w_redirect)rc = seccomp_forbid_write(ctx,option);
    if (rc < 0) return -1;
    int ret = seccomp_load(ctx);
        return ret;
}

int  seccomp_forbid_network(scmp_filter_ctx *ctx){
    int rc = -1;
    rc = seccomp_rule_add(ctx,SCMP_ACT_TRAP,SCMP_SYS(socket),0);
    return rc;
}

int seccomp_forbid_write(scmp_filter_ctx *ctx, secoption *option){
    int rc = 1;
    rc = seccomp_rule_add(ctx, SCMP_ACT_TRAP, SCMP_SYS(write), 1,SCMP_CMP(0, SCMP_CMP_NE, 1));
    if (option->w_stdout){
        rc = seccomp_rule_add(ctx, SCMP_ACT_TRAP,SCMP_SYS(dup2),0);
    }
    return rc;
}
