#include <stdio.h>

#include <sys/ptrace.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>


int i;

int main(){
    i = 10;

    printf("child: i=%d \n", i);

    
    return 0;
}
