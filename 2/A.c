#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
typedef struct timeval timeval_t;

int main(int argc,char *argv[]){
    int i = 0;
    timeval_t start,end;
    char *buffer;
    buffer = malloc(sizeof(char) * BUFSIZE);
    gettimeofday(&start,NULL);
    FILE *fpr,*fpw;;
    fpr = fopen(argv[1],"rb");
    fpw = fopen(argv[2],"wb");

    if(buffer ==NULL){
        printf("Memory allocation failed");
        exit(0);
    }
    while(1){
        fread(buffer,sizeof(char),BUFSIZE,fpr);
        fwrite(buffer,sizeof(char),BUFSIZE,fpr);
    }
    gettimeofday(&end,NULL);
    printf("%ld.%06lu\n",start.tv_sec-end.tv_sec,start.tv_usec-end.tv_usec);
    free(buffer)
    fclose(fpr);
    fclose(fpw);
    gettimeofday(&start,NULL);
    mycp()
    gettimeofday(&end,NULL);
    printf("%ld.%06lu\n",start.tv_sec-end.tv_sec,start.tv_usec-end.tv_usec);
    //fread experiment
    return 0;
}