#include <sys/ptrace.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    
    ptrace(PTRACE_TRACEME, 0, 0, 0);

    while(1){
        sleep(1);
        printf("Process is working... \n");
    }

    return 0;
}
