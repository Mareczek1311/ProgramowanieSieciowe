#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char* argv[]){

    if(argc > 2){
        printf("Zle argumenty! ./a.out [sciezka] \n");
        exit(0);
    }

    int status, pid;

    if((pid = fork()) == 0){
        execl("/bin/ls", "/bin/ls", argv[1], NULL);
        perror("execl");
    }
    else{
        wait(&status);
        if(WIFEXITED(status)){
            printf("ls wykonany \n");
        }
    }

    return 0;
}

