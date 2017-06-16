#define _GNU_SOURCE
#include <dirent.h>     /* Defines DT_* constants */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include "systest.h"


typedef struct  {
    unsigned long  d_ino;     /* Inode number */
    unsigned long  d_off;     /* Offset to next linux_dirent */
    unsigned short d_reclen;  /* Length of this linux_dirent */
    char           d_name[];  /* Filename (null-terminated) */
                        /* length is actually (d_reclen - 2 -
                        offsetof(struct linux_dirent, d_name)) */
    /*
    char           pad;       // Zero padding byte
    char           d_type;    // File type (only since Linux
                                // 2.6.4); offset is (d_reclen - 1)
    */
} linux_dirent; 

int main(int argc,char *argv[]){
    int fd,nread;
    char buf[BUFSIZE];
    linux_dirent *d;
    int bpos;
    char d_type;
    char *filename = argc>1 ? argv[1] : ".";
    fd = open(filename, O_RDONLY | O_DIRECTORY);
    fd_check(fd,filename);
    for ( ; ; ) {
               nread = syscall(SYS_getdents, fd, buf, BUFSIZE);
               if (nread == -1)
                   handle_error("getdents");

               if (nread == 0)
                   break;

               printf("--------------- nread=%d ---------------\n", nread);
               printf("i-node#  file type  d_reclen  d_off   d_name\n");
               //bpos = offset, 
               for (bpos = 0; bpos < nread;) {
                   d = (linux_dirent *) (buf + bpos);
                   printf("%8ld  ", d->d_ino);
                   d_type = *(buf + bpos + d->d_reclen - 1);
                   printf("%-10s ", (d_type == DT_REG) ?  "regular" :
                                    (d_type == DT_DIR) ?  "directory" :
                                    (d_type == DT_FIFO) ? "FIFO" :
                                    (d_type == DT_SOCK) ? "socket" :
                                    (d_type == DT_LNK) ?  "symlink" :
                                    (d_type == DT_BLK) ?  "block dev" :
                                    (d_type == DT_CHR) ?  "char dev" : "???");
                   printf("%4d %10lld  %s\n", d->d_reclen,
                           (long long) d->d_off, d->d_name);
                   bpos += d->d_reclen;
               }
    }
    exit(EXIT_SUCCESS);
}
