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
    signal(SIGCHLD, obsluga);

    pid_t child;
    int chars;
    char *buf = NULL;
    size_t buf_size = 100;
    int fifodesc;

    mkfifo("mojafifo", 0777);

    child = fork();
    if(child < 0){
        perror("fork");
        exit(EXIT_FAILURE);
    }else if(child == 0){
        if((fifodesc = open("mojafifo", O_WRONLY)) != -1){
            printf("Potomek: podaj napis: \n");
            chars = getline(&buf, &buf_size, stdin);
            int idx = chars - 1;
            char tmp;
            for(int i = 0; i < chars/2; i++){
                tmp = buf[i];
                buf[i] = buf[idx];
                buf[idx] = tmp;
                idx--;
            }
            write(fifodesc, buf, chars);
            close(fifodesc);
            exit(0);
        }
    }else{
        if((fifodesc = open("mojafifo", O_RDONLY)) != -1){
        char msg[30];
        if((chars = read(fifodesc, msg, 30)) < 0){
            perror("read");
            exit(EXIT_FAILURE);
        }
        msg[chars] = '\0';
        printf("Przodek: potomek przeslal: %s \n", msg);
        
        close(fifodesc);
        }
        unlink("mojafifo");
    }
    return 0;
}
