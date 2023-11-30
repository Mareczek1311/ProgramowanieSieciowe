#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

int main(int argc, char* argv[]){
    
    int n;
    int pid;
    
    printf("Podaj pid: ");
    scanf("%d", &pid);

    printf("Sygnal: ");
    scanf("%d", &n);

    if(n == 1) {
        kill(pid, SIGUSR1); 
    }else{
        kill(pid, SIGUSR2);
    }
    return 0;
}
