#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void obsluga_int(int s){
    printf("Sygnal o numerze: %d \n", s);
    printf("I tak mnie nie przerwiesz kochany :) \n");
}

void ouch(int sig, siginfo_t *sip, void *uap){
    printf("Dostalem signal numer %d \n", sig);
    if(sip->si_code <= 0){
        printf("Od UID %d \n", sip->si_uid);
    }
}

int main(){

    sigset_t iset;
    struct sigaction act;

    sigemptyset(&iset);
    act.sa_handler = &obsluga_int;
    act.sa_mask = iset;
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);

    sigemptyset(&iset);
    act.sa_sigaction = &ouch;
    act.sa_mask = iset;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    while(1){
        sleep(1);
    }
    

    return 0;
}
