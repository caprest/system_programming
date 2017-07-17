#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <seccomp.h>
#include <signal.h>
#include "parse.h"
#include "shell_security.h"
#include "shell.h"
#define _GNU_SOURCE


void shell_loop(char *envp[]){
    char *command;
    job *jobs;
    int status;
    secoption option = init_secoption();
    do {
        printf(PROMPT);
        command = shell_read_line();
        jobs = parse_line(command);
        status = shell_execute(jobs,&option,envp);
        
        free(command);
        free_job(jobs);

    }while(status);
}


char *shell_read_line(){
    int bufsize = SHELL_BUF_SIZE;
    char *buffer = malloc(sizeof(char) * bufsize);
    if (!buffer) {
    fprintf(stderr, "malloc: allocation error\n");
    exit(EXIT_FAILURE);
  }
  int c;
  int position = 0;
  while(1)
  {
        c = getchar();
        if (c == EOF || c == '\n'){
            buffer[position] = '\n';
            return buffer;
        }
        else {
            buffer[position] = c;
        }

        position++;

        if (position >=bufsize) {
            bufsize += SHELL_BUF_SIZE;
            buffer = realloc(buffer,bufsize);
            if(!buffer){
                fprintf(stderr,"memory allocation error");
                exit(EXIT_FAILURE);
            }
        }
  }
}



int shell_execute(job *jobs,secoption *option,char *envp[]){
    job *cur_job = jobs;
    job_mode mode;
    process *cur_process;
    while(cur_job){
        mode = cur_job->mode;
        cur_process = cur_job->process_list;
        if (mode == FOREGROUND){
                execute_process(cur_process,option,envp);
        }
        cur_job = cur_job->next;
    }
    return 1;
}

void execute_process(process *p,secoption *option,char *envp[]){
    int n_process = 0;
    process *cp_p = p;
    while(cp_p){
        n_process++;
        cp_p = cp_p->next;
    }
    int *pid;
    int status;
    int i;
    int **fd_array;
    pid = malloc(sizeof(int) * n_process);
    mem_check(pid);
    if(n_process > 1){
        fd_array = malloc(sizeof(int*) * (n_process - 1 ));
        mem_check(fd_array);
        for (i =0 ;i < n_process-1; i++){
            fd_array[i] = malloc(sizeof(int) * 2);
            mem_check(fd_array[i]);
            }
    }
    int ret_pipe;
    if (n_process >1){
        for(i =0;i < n_process;i++){
            if (strcmp(p->program_name,"exit") == 0){
                exit_shell(0);
            }
            if (i == 0){
                ret_pipe = pipe(fd_array[i]);
                if (ret_pipe != 0) exit(0);
                fork_process(p,pid,fd_array,0,1,i,envp,option);
                close(fd_array[i][1]);
                p = p->next;
            }
            else if(0<i && i< n_process-1){
                ret_pipe = pipe(fd_array[i]);
                if (ret_pipe != 0) exit(0);
                fork_process(p,pid,fd_array,1,1,i,envp,option);
                close(fd_array[i-1][0]);
                close(fd_array[i][1]);
                p = p->next;
            }
            else{
                fork_process(p,pid,fd_array,1,0,i,envp,option);
                if (ret_pipe != 0) exit(0);
                close(fd_array[i-1][0]);
                p = p->next;
            }
        }
    }
    else{
        if (strcmp(p->program_name,"exit") == 0){
            exit_shell(0);
        }
        else if (strcmp(p->program_name, "setsec") == 0){
            char **p_arg = p->argument_list;
            int f_network = 0;
            int f_redirect = 0;
            int f_stdout = 0;
            int f_reset = 0;
            while(p_arg[0]){
                if (strcmp(p_arg[0],"no-network" ) == 0){
                 f_network = 1;
                }    
                else if (strcmp(p_arg[0],"only-redirect") ==0) {
                    f_redirect =1;
                }
                else if (strcmp(p_arg[0],"only-stdout") == 0) {
                    f_stdout = 1;
                }
                else if (strcmp(p_arg[0],"reset") == 0){

                 f_reset =1; break;
                }
                p_arg++;
            }
            set_secoption(option,f_network,f_stdout,f_redirect);
            if (f_reset == 0){
                printf("Seccomp activated. no-network = %d,stdout = %d,redirect = %d\n",
                        option->network,option->w_stdout,option->w_redirect);
            }
            else{
                printf("Seccomp Resetted\n");
            }
        }
        else fork_process(p,pid,fd_array,0,0,0,envp,option);
    }
    int flag_sigint = 0;
    for(i = 0; i < n_process;i++){
        waitpid(-1,&status,WUNTRACED);
        if (status == EXIT_SECCOMP * 256){
            printf("Process exited with seccomp\n");
            printf("Seccomp Setting. no-network = %d,stdout = %d,redirect = %d\n",
                        option->network,option->w_stdout,option->w_redirect);
            }
        if (status == EXIT_SIGINT *256 && flag_sigint == 0){
            printf("Keyboard Interruption\n");
        }
    }
    for (i = 0;i <n_process-1;i++){
        close(fd_array[i][0]);
    }
    if (n_process > 1){
        for (i = 0;i< n_process-1 ;i++){
        free(fd_array[i]);
        }
    if (fd_array){
        free(fd_array);
        }
    }
    
    free(pid);
}

void exit_shell(int status){
    exit(status);
}

void mem_check(void * mem){
    if(!mem){
        fprintf(stderr,"malloc :allocation error\n");
        exit(EXIT_FAILURE);
    }
}

void fork_process(process * p,int *pid,int **fd_array,int flag_in,int flag_out, int i,char *envp[],secoption *option){
    if((pid[i] = fork()) == 0){
            int fd1 = -1 ;
            int fd2 = -1;
            /*
            struct sigaction sa;
            sa.sa_sigaction = child_handler;
            sa.sa_flags = SA_RESTART; 
            sigaction(SIGSYS, &sa, 0);
            sigaction(SIGINT, &sa,0 );
            */
            if (p->output_redirection && option->w_stdout){
                printf("Redirect is forbidden\n");
                exit(0);
            }
            if (p->output_redirection){
                fd1 = open(p->output_redirection,O_WRONLY|O_CREAT|O_TRUNC,S_IRWXU);
                dup2(fd1, 1);
            }
            if(p->input_redirection){
                fd2 = open(p->input_redirection,O_RDONLY);
                dup2 (fd2,0);
            }
            if (flag_in){
                int ret_f = dup2(fd_array[i-1][0],0);
                if (ret_f == -1) exit(0);
                close(fd_array[i-1][0]);
            }
            if (flag_out){
                int ret_f = dup2(fd_array[i][1],1);
                if (ret_f == -1) exit(0);
                close(fd_array[i][0]);
                close(fd_array[i][1]);
            }
            if (access(p->program_name, X_OK) != 0){
                printf("file not exist %s\n",p->program_name);
                exit(0);
            }
            int rsec = set_seccomp(option);
            if (rsec <0) {
                printf("failed seccomp \n");
                exit(1);
            }
            execve(p->program_name,p->argument_list,envp);
        }
}
void child_handler(int num,siginfo_t *info, void *hoge) {
        if (info->si_signo == SIGSYS){
            _exit(EXIT_SECCOMP);
        }
        if (info->si_signo == SIGINT){
            _exit(EXIT_SIGINT);
        }
}

void parent_handler(int num,siginfo_t *info, void *hoge){

}

int main ( int argc, char *argv[], char *envp[] ){   
    struct sigaction sa;
    sa.sa_sigaction = parent_handler;
    sa.sa_flags = SA_RESTART; 
    sigaction(SIGINT, &sa, 0);
    
    shell_loop(envp);
    return 0;
}
