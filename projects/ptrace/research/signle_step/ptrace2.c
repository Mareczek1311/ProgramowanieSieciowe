#include <stdio.h>

#include <sys/ptrace.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>

int main(){

    int pid, status;


    if((pid = fork()) == 0){
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execl("./child2", "child2", 0);
        printf("exec failed! \n");
    }else {
        wait(&status);

            ptrace(PTRACE_SINGLESTEP, pid, 0, 0);
            wait(&status);
            ptrace(PTRACE_CONT, pid, 0, 0);
        printf("outside of loop \n");
        ptrace(PTRACE_CONT, pid, 0, 0);
    
        wait(&status);
        if(WIFEXITED(status)){
            printf("child process ended \n");
        }
    }

    return 0;
}
