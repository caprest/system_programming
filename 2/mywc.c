#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "systest.h"

typedef struct {
    int lines;
    int words;
    int bytes;
} file_description_t;

file_description_t *count(int fd){
    int flag = 0;
    int read_size;
    int count_bytes = 0;
    int count_lines = 0;
    int count_words = 0;
    char buf[BUFSIZE];
    int i;
    while(1){
        read_size = read(fd,buf,BUFSIZE);
        count_bytes += read_size;
        if(read_size <0){
            perror("Read Error");

        }
        for (i=0;i<read_size;i++){
            if (buf[i] == *"\n") count_lines +=1;
            if(is_space(buf[i])){
                while(++i<read_size){
                    if (is_space(buf[i]) == 0) break;
                }
                count_words +=1;
            }
        }

        if(read_size <BUFSIZE){
            break;
        }
    }
    file_description_t *ret_value;
    ret_value =(file_description_t*) malloc(sizeof(file_description_t));
    ret_value->lines = count_lines;
    ret_value->words = count_bytes;
    ret_value->bytes = count_words;
    return ret_value;
}



int main(int argc, char* argv[]){
    int fd;
    if (argc <2){
        printf("Too few arguments");
        exit(0);
    }
    int i;
    int status;
    char buf[BUFSIZE];
    int break_flag = 0;
    size_t size_read = 0;
    size_t size_write = 0;
    fd = open(argv[argc-1], O_RDONLY);
    fd_check(fd,argv[argc-1]);
    int flag_l=0;
    int flag_w=0;
    int flag_c=0;
    if (argc>2){
        for (i =1 ; i< argc-1;i++){
            
            if (strcmp(argv[i],"-l") == 0){
                flag_l = 1;
            }
            if (strcmp(argv[i],"-w") == 0){
                flag_w = 1;
            }
            if (strcmp(argv[i],"-c") == 0){
                flag_c = 1;
            }    
        }
    } 
    file_description_t *result = count(fd);
    if (flag_l){
        printf("%d line\n" ,result->lines);
    }
    if (flag_w){
        printf("%d words\n",result->words);
    }
    if(flag_c) {
        printf("%d bytes\n",result->bytes);
    }
    free(result);
    close(fd);
    return 0;

}
