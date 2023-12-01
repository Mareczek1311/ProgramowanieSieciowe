#include<stdio.h>
#include<stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

#include <ctype.h>
#include <string.h>

void my_handler(int signal){

    printf("Proces potomny zakonczyl dzialanie\n");

}

int main(){

    if(signal(SIGCHLD, my_handler) == SIG_ERR){
        perror("signal");
    }

    int fd[2];
    int pid, status;
    size_t chars;
    size_t bufsize = 500;
    char* buf = NULL;
    char* buf2 = (char*) malloc (500*sizeof(char));
    

    if(pipe(fd) == -1){
        perror("pipe");
        exit(1);
    }

    if((pid = fork()) < 0){
        perror("fork");
        exit(1);
    }
    else if(pid == 0){
        if(close(fd[1]) == -1){
            perror("close");
            exit(1);
        }

        char msg[500];
        char s[500];

        chars = read(fd[0], msg, 500);
        
        if(chars < 0){
            perror("read");
            exit(1);
        }

        if(chars > 500){
            chars = 500;
        }

        for(int i=0; i<chars; i++){
            s[i] = toupper(msg[i]);
        }
        
        s[chars] = '\0';

        printf("[Potomek] Proces macierzysty przeslal: %s \n", s);
        printf("Wiadomosc miala dlugosc %ld znak(i/ow) \n", chars);
        
        if(close(fd[0]) == -1){
            perror("close");
            exit(1);
        }
    }
    else{
        if(close(fd[0]) == -1){
            perror("close");
            exit(1);
        }
        
        printf("[Rodzic] Podaj napis: \n");

        chars = getline(&buf, &bufsize, stdin);
        
        if(chars < 0){
            perror("getline");
            exit(1);
        }

        if(chars > 500){
            chars = 499;
        }
        else{
            chars = chars-1;
        }

        strncpy(buf2, buf, chars);
        
        if(write(fd[1], buf2, chars) == -1){
            perror("write");
            exit(1);
        }
        
        if(wait(&status) == -1){
            perror("wait");
            exit(1);
        }

        if(close(fd[1]) == -1){
            perror("close");
            exit(1);
        }
    }


    free(buf);
    free(buf2);

    return 0;
}
