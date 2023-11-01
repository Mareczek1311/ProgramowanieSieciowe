#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void signalHandler(int signal_num){
    printf("Sygnal: %d \n", signal_num);
    switch (signal_num) {
        case SIGINT:
            printf("Otrzymano SIGINT - Sygnał przerwania (Ctrl+C)\n");
            break;
        case SIGTERM:
            printf("Otrzymano SIGTERM - Sygnał zakończenia\n");
            break;
        case SIGUSR1:
            printf("Otrzymano SIGUSR1 - Sygnał użytkownika 1\n");
            // Dodaj niestandardową akcję dla SIGUSR1 tutaj
            break;
        case SIGUSR2:
            printf("Otrzymano SIGUSR2 - Sygnał użytkownika 2\n");
            // Dodaj niestandardową akcję dla SIGUSR2 tutaj
            break;
        case SIGALRM:
            printf("Otrzymano SIGALRM - Sygnał alarmu\n");
            // Dodaj niestandardową akcję dla SIGALRM tutaj
            break;
        default:
            printf("Otrzymano nieznany sygnał o numerze %d\n", signal_num);
            break;
    }
}

int main(){

    pid_t pid;

    int sig, r;

    
    signal(SIGUSR1, signalHandler);
    signal(SIGUSR2, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGALRM, signalHandler);
    signal(SIGINT, signalHandler);
  
    raise(SIGUSR1);
    
    while(1){
        sleep(1);
    }

	return 0;
}
