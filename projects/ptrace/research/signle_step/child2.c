#include <stdio.h>

#include <sys/ptrace.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>

int main(){
    
    printf("child: -1 \n");

    for(int i=0; i<10; i++){
        printf("child: i=%d \n", i);
    }

    return 0;
}
