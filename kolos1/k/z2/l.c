#include<unistd.h>
#include<stdio.h>
#include <signal.h>
int main(){
   int i = 0; 
    while(1){
        sleep(1);
        printf("...\n");
        i++;
        if(i == 3){
           kill(getpid(), SIGUSR1);
        
    }
    }

    return 0;   
}
