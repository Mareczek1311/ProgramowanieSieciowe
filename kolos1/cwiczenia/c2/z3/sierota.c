     #include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main(){
    int pid, status;

    if((pid = fork()) == 0){
        sleep(10);
        printf("Kutas nie poczekal \n");
    }else{
        printf("Pierole nie czekam \n");
    }

    return 0;
}
