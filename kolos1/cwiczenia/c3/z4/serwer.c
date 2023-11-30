     #include <unistd.h>
#include<stdlib.h>
#include<stdio.h>

void sigi(int s){
    printf("Serwer konczy prace \n");
    exit(0);
}

void mysig(int s){
    printf("Sygnal %d \n", s);
}

int main(){

    int pid, status;

    signal(SIGUSR1, mysig);
    signal(SIGUSR2, mysig);
    signal(SIGINT, sigi);
    printf("Serwer dziala %d \n", getpid());
    while(1){
        sleep(1);

    }

    return 0;
}
