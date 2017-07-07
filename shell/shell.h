
#ifndef __SHELL_H__
#define __SHELL_H__
#include "parse.h"
#include "shell_security.h"
#define SHELL_BUF_SIZE 2000
#define TOK_DELIM " \t\r\n\a"


void exit_shell();
void shell_loop(char **);
char * shell_read_line();
int shell_execute(job * ,secoption * ,char**);
void execute_process(process *,secoption *,char**);
void fork_process(process *,int *,int **,int, int, int,char **,secoption * );
void mem_check(void*);

#endif