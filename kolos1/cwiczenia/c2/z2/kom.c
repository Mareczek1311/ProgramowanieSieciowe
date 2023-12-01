#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){

    int pid, status;
    int n;
    if((pid = fork()) == 0){
        printf("Potomek (%d): Wpisz n: \n", getpid());
        scanf("%d", &n);
        exit(n);
    }
    else{
        wait(&status);
        if(WIFEXITED(status)){
            printf("Rodzic(%d) Potomek przeslal mi: %d \n", getpid(), WEXITSTATUS(status));
        }
    }

    return 0;
}
