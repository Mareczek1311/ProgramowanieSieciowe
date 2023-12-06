#include <stdio.h>
#include <stdlib.h>
     #include <unistd.h>
     #include <signal.h>

void mysig(int signal){
    printf("Ojciec dostal sygnal %d \n", signal);
}

int main(){
    
    int pid, status;
    
            

    if((pid = fork()) == 0){
        int i = 0;
        while(i<10){
            if(rand() % 2 + 1 == 1){
                kill(getppid(), SIGUSR1);
            }
            else{
                kill(getppid(), SIGUSR2);
            }
            sleep(1);
            i++;
        }
    }else{
                
    signal(SIGUSR1, mysig);
    signal(SIGUSR2, mysig);
        int i = 0;
        while(i<11){
            sleep(1);
            i++;
        }

        wait(&status);
    }

    return 0;
}
