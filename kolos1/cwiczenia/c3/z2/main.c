     #include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>

void my_signal(int signal){
    printf("Sygnal: %d \n", signal);
}

int main(){
    
    
    signal(SIGINT, my_signal);
    signal(SIGQUIT, my_signal);
    signal(SIGTSTP, my_signal);
    
    int i = 0;
    while(i<15){
        printf("jestem dolo i jestem zjebany \n");
        i++;
        sleep(1);
    }
}
