#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>


#define MAXLINE 1024
#define FIFO "fifo"



int main(){
    int n, fd;
    char line[MAXLINE];
    pid_t pid;

    if( (mkfifo(FIFO, 0666) < 0) && (errno != EEXIST) ){
        perror("pipe error");
    }

    if((pid = fork()) < 0){
        perror("fork");
    }
    else if(pid == 0){
   
        if( (fd = open(FIFO, O_WRONLY)) < 0 ){
            perror("open rodzic");
            exit(0);
        }

        char* text = "siema to ja :) \n ";
        write(fd, text, strlen(text));
    
    }
    else{
    
        if( (fd = open(FIFO, O_RDONLY)) < 0 ){
            perror("open potomen");
            exit(0);
        }
        n = read(fd, line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }



    return 0;
}
