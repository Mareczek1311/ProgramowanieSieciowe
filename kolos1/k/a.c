#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

void obsluga(int singal){
    printf("Proces potomny zakonczyl dzialanie \n");
}

int main()
{
    signal(20, obsluga);

    int fd[2], fd2[2];
    pid_t child;
    size_t buf_size = 30;
    char *buf = NULL;
    int len;
    int i = 0;
    int chars;    

    if(pipe(fd) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
     if(pipe(fd2) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    child = fork();
    if(child == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }else if(child == 0){
        char msg[30];
        char res[30];
        close(fd[1]);
        close(fd2[1]);
        if((i = read(fd[0], msg,30)) < 0){
                perror("read");
                exit(1);
        }
        int currInx = 0;
        for(int j=0; j<i-1; j++){
            res[currInx] = msg[j];
            currInx++;
        }

        if((i = read(fd2[0], msg, 30)) < 0){
            perror("read");
            exit(1);
        }
        
        for(int j=0; j<i-1; j++){
            res[currInx] = msg[j];
            currInx++;
            
        }
        
        res[i*2-1] = '\0';

        printf("Potomek: dostalem %d : %s \n", i*2-2, res);
        
    }else{
        close(fd[0]);
        close(fd2[0]);
        printf("Przodek: podaj napis: \n");
        chars = getline(&buf, &buf_size, stdin);
        if(write(fd[1], buf, chars) == -1){
            perror("write");
            exit(1);
        }
        
        if(write(fd2[1], buf, chars) == -1){
            perror("write 2");
            exit(1);
        }

    }
    return 0;
}
