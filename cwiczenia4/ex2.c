#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define MAXLINE 1024

int main(){
    
    int fd[2], status, fd2[2];
    pid_t pid;
    long n;
    pipe(fd);
    pipe(fd2);
    char line[MAXLINE];
    int i;
    if((pid = fork()) == 0){
        
        close(fd[1]);
        close(fd2[0]);
        
        i = read(fd[0], &n,sizeof(long));
        if(i != sizeof(long)){
            perror("read");
        }
        n = n*2;
        write(fd2[1], &n, sizeof(long));

    }else{
        scanf("%d", &n);
        close(fd[0]); 
        close(fd2[1]);
        write(fd[1], &n, sizeof(long));

        i = read(fd2[0], &n,sizeof(long));
        
        printf("N*2: %d \n", n);
        
    }
    
    return 0;
}
