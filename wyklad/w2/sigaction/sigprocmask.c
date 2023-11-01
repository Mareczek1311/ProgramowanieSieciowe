#include <stdio.h>
#include <signal.h>
#include <unistd.h>


int main(){
    
    sigset_t iset;

    sigemptyset(&iset);
    sigaddset(&iset, SIGALRM);
    sigaddset(&iset, SIGINT);
    sigprocmask(SIG_BLOCK, &iset, NULL);

    alarm(5);

    while(1){
        sleep(1);
    }

    return 0;
}
