#include<unistd.h>

#include<stdio.h>


int main(){
    
    int pid;

    if((pid = fork()) == 0){
        sleep(1);
        printf("%d\n", getppid());
    }
    else{
        
    }
    
    return 0;
}
