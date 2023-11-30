     #include <sys/types.h>
     #include <sys/stat.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
void hand(int stat){
    printf("przechwycony SIGCHLD %d \n", stat);
}

int main(){
    
    int fd1[2], fd2[2];  
    int chars;
    int pid, status;
    char* buf = NULL;
    size_t size = 30; 
    signal(SIGCHLD, hand);

    pipe(fd1);
    pipe(fd2);

    if((pid = fork()) == 0){
        char msg[30];
        char s[30];
        close(fd1[1]);
        close(fd2[1]);

        chars = read(fd1[0], msg, 30);
        if(chars < 0){
            perror("read");
            exit(1);
        }
        int currInx = 0;
        for(int i=0; i<chars-1; i++){
            s[currInx] = msg[i];
            currInx++;
        }

        chars = read(fd2[0], msg, 30);
        if(chars < 0){
            perror("read");
            exit(1);
        }


        for(int i=0; i<chars; i++){
            s[currInx] = msg[i];
            currInx++;
        }
        s[chars*2] = '\0'; 
        printf("Potomek: dostalem %d : %s \n", chars*2-2, s);
    }
    else{
    
        close(fd1[0]);
        close(fd2[0]);

        printf("Przodek: podaj napis: ");
        chars = getline(&buf, &size, stdin);

        if(write(fd1[1], buf, chars) == -1){
            perror("write");
            exit(1);
        }

        if(write(fd2[1], buf, chars) == -1){
            perror("write");
            exit(1);
        }

        wait(&status);
    }
        
    return 0;

}
