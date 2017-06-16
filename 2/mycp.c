#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define BUFSIZE 256

void recur_write(int fd,char* buf,int size_read,int wrote_size);

void deal_write_error(int fd){
    if(errno == EDQUOT){
        perror("Insufficient Diskspace");
        
    } 
    else{
        perror("WRITE ERROR OCCURRED");
    }
 
    close(fd);
    exit(0);
}

void recur_write(int fd,char* buf,int size_read,int wrote_size){
    if(size_read == wrote_size) return;
    else{
        int size_write;
        size_write = write(fd,buf,size_read-wrote_size);
        if (size_write == -1) deal_write_error(fd);
        else{
        wrote_size +=size_write;
        buf=&buf[size_write];
        recur_write(fd,buf,size_read,wrote_size);
        }
    }
}

int main(int argc, char *argv[]){
    char *in_filename = argv[1];
    char *out_filename = argv[2];
    if (argc!=3){
        printf("Too many argument");
        printf("%d",argc);
        exit(0);
    }
    printf("%s",in_filename);
    int fd_in,fd_out;
    int i;
    int status;
    char buf[BUFSIZE];
    int break_flag = 0;
    size_t size_read = 0;
    size_t size_write = 0;
    struct stat *buf_stat;
    //scanf("%s",namebuf);
    fd_in = open(in_filename, O_RDONLY);
    fstat(fd_in,buf_stat);
    mode_t permission = buf_stat->st_mode;
    fd_out = open(out_filename,O_RDWR|O_CREAT,permission);
    fd_check(fd_in,in_filename);
    fd_check(fd_out,out_filename);
    while(1){
        size_read =(int) read(fd_in,buf,BUFSIZE);
        if (size_read<BUFSIZE) break_flag = 1;
        if (size_read>0) {
                recur_write(fd_out,buf,size_read,0);            
            }
        if (break_flag==1) break;
    }


    close(fd_in);
    close(fd_out);
}