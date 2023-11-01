#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h> 

int main(int argc, char* argv[]){

    if(argc != 3){
        printf("Za malo danych! \n");
    }

    int sig = atoi(argv[1]);
    int pid = atoi(argv[2]);
    
    kill(pid, sig);


    return 0;
}
