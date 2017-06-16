#include "systest.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void fd_check(int fd,char *name){
        if (fd == -1){
            printf("fd = %d, errno=%d,filename = %s\n", fd, errno,name);
            perror("ERROR in fd_check");
            exit(0);
        }
        
        else {
            printf("%s open succeeded\n",name);
        }
}

int is_space(char c){
    int ret = 0;

    if(c == *"\t" || c== *"\t" || c == *"\n" || c == *"\f" || c == *"\r") ret =1;
    return ret;
}