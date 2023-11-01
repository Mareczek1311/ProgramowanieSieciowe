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
        printf("--Proces potomny wykonuje sie...-- \n");
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }
    else{
        printf("--Proces rodzica wykonuje sie...-- \n");
        sleep(5);
        printf("--Proces rodzica wybudzil sie--\n");
        close(fd[0]);
        char* text = "potoki are mid :/ \n";
        write(fd[1], text, strlen(text));
    }

    return 0;
}
