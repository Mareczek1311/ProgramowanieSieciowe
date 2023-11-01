#include <stdio.h>
#include <unistd.h>


int i = 15;
int main(){

    while(1){
        printf("Child: i=%d \n", i);
        sleep(1);
    }

    return 0;
}
