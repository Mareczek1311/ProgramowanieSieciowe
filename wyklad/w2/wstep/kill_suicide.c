#include <stdio.h>
#include <signal.h>
#include <unistd.h>


int main(){
    
    printf("Proces o pid: %d zostal utworzony \n");

    kill(getpid(), SIGKILL);
    
    printf("Wiadomosc nie zostanie wyswietlona, proces popelnil samobojstwo :) \n");

    return 0;
}
