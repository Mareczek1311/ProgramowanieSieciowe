#include <sys/ptrace.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    
    pid_t pid; 
    int status;
   
    if((pid = fork()) == 0){
        execl("./child_process", "child_process", 0);
        printf("exec failed... \n");
    }
    else{
        sleep(1);
        kill(pid, SIGINT);
        printf("Signal sended... \n");
        wait(&status);

        if(WIFSTOPPED(status)){
            printf("Child stopped... \n");
        }

        sleep(3);
        ptrace(PTRACE_CONT, pid, 0, 0);

        //wait(&status);
        sleep(3);
        kill(pid, SIGKILL);
    }
    
    return 0;
    
}
