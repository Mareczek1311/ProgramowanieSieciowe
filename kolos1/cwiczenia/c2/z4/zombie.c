#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>

int main(){
    
    int pid, status;

    if( (pid = fork()) == 0 ){
        printf("staje sie zombie... \n");
    }else{
        printf("Przez 10 sekund mam wyjebane w potomka \n");
        sleep(10);
        printf("Dobra obsluze smieci \n");
    }
    
    wait(&status);
    
    if(WIFEXITED(status)){
        printf("Oblsuzony i martwy B) \n");
    }

    return 0;
}
