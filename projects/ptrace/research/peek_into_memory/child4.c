#include <stdio.h>
#include <unistd.h>

int i=0;
int main(){
    printf("Child process starts... \n"); 
    i = 7;    
    while(i<145){
        sleep(1);
        printf("Waiting...");
    }
    
    printf("Child outside loop.. /n");

    return 0;
}
