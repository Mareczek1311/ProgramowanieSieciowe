#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define MAXLINE 1024


int main(){
    
    int n, fd[2];
    char line[MAXLINE];
    pid_t pid;

    
    if(pipe(fd) < 0){
        perror("pipe error");
    }

    if((pid = fork()) < 0){
        perror("fork ");
    }
    else if(pid == 0){
        close(fd[1]);
        while((n = read(fd[0], line, MAXLINE)) > 0){
            if(write(STDOUT_FILENO, line, n) == -1){
                perror("write potomek");
            }
        }

        close(fd[0]);
    }
    else{
        close(fd[0]);
        
        char input[MAXLINE];

        while(fgets(input, MAXLINE, stdin) != NULL){
            if(write(fd[1], input, strlen(input)) == -1){
                perror("write rodzic");
            }
        }
        close(fd[1]);
        
    }

    return 0;
}
