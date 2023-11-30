#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void my_hand(int signal){
    printf("Proces potomny zakonczyl dzialanie \n");
}

int main(){
    
    int fd[2];
    int pid, status;
    size_t chars;
    char* buf = NULL;
    char buf2[1024];
    size_t bufsize = 80;
    char tmp;

    if(pipe(fd) == -1){
        perror("pipe");
        exit(1);
    }

    signal(SIGCHLD, my_hand);

    if((pid = fork()) < 0){
        perror("fork");
        exit(1);
    }else if(pid == 0){
        
        close(fd[0]);

        printf("Potomek: podaj napis: \n");
        chars = getline(&buf, &bufsize, stdin);
        
        for(int i=0; i<chars; i++){
            tmp = buf[i];
            buf[i] = buf[chars-1-i];
            buf[chars-1-i] = tmp;
        }

        write(fd[1], buf, chars);
        
    }else{
        
        close(fd[1]);

        chars = read(fd[0], &buf2, sizeof(buf));
        
        buf[chars] = '\0';

        printf("Przodek: potomek przeslal: %s", buf2);

        wait(&status);
    }

    return 0;
}
