#include<unistd.h>
#include<stdio.h>

int main(){
    
    printf("Child starts process... \n");
    
    while(1){
        sleep(1);
        printf("Hello \n");
    }

    return 0;
}
