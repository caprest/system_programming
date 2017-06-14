#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "systest.h"


void count_line(int fd){
    int i;
    int lines;
    int read_size;
    int count_line=1;
    while(flag = 0){
        read_size = read(fd,buf,BUFSIZE)
        if(read_size <0){
            perror("Read Error");

        }
        for (i=0;i<read_size;i++){
            if (buf[i] == "\n") count_line +=1;
        }
        if(read_size <BUFSIZE){
            break;
        } 
    }
}

void count_words(int fd){
        int i;
    int lines;
    int read_size;
    int count_word=1;
    while(flag = 0){
        read_size = read(fd,buf,BUFSIZE)
        if(read_size <0){
            perror("Read Error");

        }
        for (i=0;i<read_size;i++){
            if (is_space(buf[i]){
                while(++i<read_size){
                    if is_space(buf[i]) == 0 break;
                }
                count_word +=1;
            }
        }
        if(read_size <BUFSIZE){
            break;
        } 
    }

}

void count_bytes(int fd){
    int flag = 0;
    int read_size;
    int count_size = 0;
    char buf[BUFSIZE];
    while(1){
        read_size = read(fd,buf);
        count_size += read_size;
        if(read_size <0){
            perror("Read Error");

        }
        else if(read_size <BUFSIZE){
            break;
        } 
    }
}
;


int main(int argc, char* argv[]){
    int fd;
    if (argc <2){
        printf("Too few arguments");
    }
    else if {
        open(argv[0])
    }
    int i;
    int status;
    char buf[BUFSIZE];
    int break_flag = 0;
    size_t size_read = 0;
    size_t size_write = 0;
    fd = open(in_filename, O_RDONLY);
    fd_check(fd,argv[argc-1]);
    if (argc >2 && argv[2]== "-l"){
        count_line(fd);
    }
    else if (argc >2 && argv[2] == "-w"){
        count_words(fd);
    }
    else{
        count_bytes(fd);
    }



    close(fd_in);

}
