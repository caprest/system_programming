#include <stdio.h>
#include <fcntl.h>
#include <seccomp.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>     /* Defines DT_* constants */
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <errno.h>

#define _GNU_SOURCE


void seccomp_allow_least(scmp_filter_ctx *ctx){
    int rc = -1;
    rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit_group), 0);
    rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(fstat), 0);
    rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(stat), 0);
    rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mmap),0);
    rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect),0);
    rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(brk),0);
    rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(access),0);
    rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(fadvise64),0);
    rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mknod),0);
    rc = seccomp_rule_add(ctx,SCMP_ACT_ALLOW, SCMP_SYS(munmap),0);
    rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(arch_prctl),0);
}

void seccomp_allow_stdout(scmp_filter_ctx *ctx){
    int rc = -1;
    //rc = seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(write),0);
    //rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 1,SCMP_CMP(0, SCMP_CMP_EQ, 1));
    rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write),0);
    rc = seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(write), 1,SCMP_A0(SCMP_CMP_NE, 1));
}
void seccomp_allow_reado(scmp_filter_ctx *ctx){
    int rc = -1;    
    rc = seccomp_rule_add(ctx,SCMP_ACT_ALLOW,SCMP_SYS(open),1,SCMP_A1(SCMP_CMP_EQ,O_RDONLY,O_RDONLY));
    printf("%d\n",rc);
    rc = seccomp_rule_add(ctx,SCMP_ACT_ALLOW,SCMP_SYS(read),0);
    rc = seccomp_rule_add(ctx,SCMP_ACT_ALLOW,SCMP_SYS(close),0);

}

void seccomp_allow_execve(scmp_filter_ctx *ctx){
    int rc = -1;
    rc = seccomp_rule_add(ctx,SCMP_ACT_ALLOW,SCMP_SYS(fork),0);
    rc = seccomp_rule_add(ctx,SCMP_ACT_ALLOW,SCMP_SYS(execve),0);
    //rc = seccomp_rule_add(ctx,SCMP_ACT_ALLOW,SCMP_SYS(exec),0);
    rc = seccomp_rule_add(ctx,SCMP_ACT_ALLOW,SCMP_SYS(prctl),0);
    rc = seccomp_rule_add(ctx,SCMP_ACT_ALLOW,SCMP_SYS(getpid),0);
    rc = seccomp_rule_add(ctx,SCMP_ACT_ALLOW,SCMP_SYS(waitpid),0);
    rc = seccomp_rule_add(ctx,SCMP_ACT_ALLOW,SCMP_SYS(wait4),0);
    rc = seccomp_rule_add(ctx,SCMP_ACT_ALLOW,SCMP_SYS(clone),0);
}   

void seccomp_forbid_network(scmp_filter_ctx *ctx){
    int rc = -1;
    //ctx = seccomp_init(SCMP_ACT_ALLOW);
    rc = seccomp_rule_add(ctx,SCMP_ACT_KILL,SCMP_SYS(socket),0);
}


/*
特定ディレクトリの読み書き？→chmodを使うのが早いのでは
特定ファイルのみexecv→丸そう(dinamic-linkは？)


*/




#define BUF_SIZE    256



int main(int argc, char *argv[],char *envp[])
{
    int rc = -1;
    
    //struct scmp_arg_cmp arg_cmp[] = { SCMP_A0(SCMP_CMP_EQ, 2) };

    /*
    seccomp_allow_stdout(ctx);
    seccomp_allow_least(ctx);
    seccomp_allow_reado(ctx);
    seccomp_allow_execve(ctx);
    */
    //seccomp_forbid_network(ctx);
    int pid;
    char *cat_arg[] ={"/bin/ping","/proc/meminfo",0};
    int status;
    //write(fd,"hear",5);

    if ((pid = fork())==0){
        scmp_filter_ctx ctx;
            ctx = seccomp_init(SCMP_ACT_ALLOW);
        int fd = open("hoge",O_RDWR);
        ctx = seccomp_init(SCMP_ACT_ALLOW);
        struct scmp_arg_cmp arg_cmp[] = { SCMP_A0(SCMP_CMP_NE, 1),SCMP_A0(SCMP_CMP_NE, fd) };
        rc  = seccomp_rule_add(ctx, SCMP_ACT_KILL,SCMP_SYS(write),0);
        printf("rc = %d\n",rc);
        rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 1,arg_cmp);
        //rc = seccomp_rule_add(ctx, SCMP_ACT_KILLL, SCMP_SYS(write), 1,SCMP_CMP(0, SCMP_CMP_EQ, 1));
        printf("rc = %d\n",rc);
        //rc = seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 1,SCMP_CMP(0, SCMP_CMP_EQ, fd));
        printf("rc2 = %d\n",rc);
        //seccomp_forbid_network(ctx);
        seccomp_load(ctx);
        write(fd,"hoge",5);
        close(fd);
        printf("kinshisareteruyatu");
        //write(fd,"hoge",5)q;
        execve("/bin/ping",cat_arg,envp);
        //exit(0);
    }
    
    else waitpid(-1,&status,WUNTRACED);
    printf("status = %d\n",status);
    printf("errorno = %d\n",errno);

    //seccomp_release(ctx);
    return -rc;
}