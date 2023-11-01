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
        execl("./child", "child", 0);
        printf("exec failed... \n");
    }else{
        wait(&status);

        while(1){
            ptrace(PTRACE_SINGLESTEP, pid, 0, 0);

            wait(&status);

            if(WIFEXITED(status)){
                break;
            }
            
            unsigned long adr = 0x601028;
            long data = ptrace(PTRACE_PEEKDATA, pid, adr, 0);

            if(data == 10){
                ptrace(PTRACE_POKEDATA, pid, adr, 2024);
                ptrace(PTRACE_CONT, pid, 0, 0);
                break;
            }

        
        }
    }

    wait(&status);

    if(WIFEXITED(status)){
        printf("child finished work \n");
    }

    return 0;
}
