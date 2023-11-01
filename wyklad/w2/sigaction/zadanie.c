#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void obsluga(int sig){
    printf("Otrzymalem sygnal %d \n", sig);
}

int main(){
    
    struct sigaction act;
    sigset_t iset;
    int i=0;

    sigemptyset(&iset);
    sigaddset(&iset, SIGINT);

    act.sa_handler = &obsluga;
    act.sa_mask = iset;
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
    sigprocmask(SIG_BLOCK, &iset, NULL);
    
    while(1){
        sleep(1);
        i++;
        if(i==5){
            sigemptyset(&iset);
            sigaddset(&iset, SIGINT);
            sigprocmask(SIG_UNBLOCK, &iset, NULL);
            
            printf("Odblokowanie SIGINT \n");

        }
    }

    return 0;
}
