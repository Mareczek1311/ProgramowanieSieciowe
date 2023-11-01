#include <sys/ptrace.h>
#include <signal.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#include <unistd.h>

int main(){

    int status = 0, pid, r;
    struct user_regs_struct uregs;

    if( (pid = fork()) == 0 ){
        printf("Child process: pid=%d, parent_pid=%d \n", getpid(), getppid());
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        kill(getpid(), SIGINT);
        r = getpid();
        printf("Child process: pid=%d \n", r);
    }else{
        wait(&status);
        ptrace(PTRACE_SYSCALL, pid, 0, 0);

        wait(&status);
        ptrace(PTRACE_GETREGS, pid, 0, &uregs);

        uregs.orig_rax = 110;

        ptrace(PTRACE_SETREGS, pid, 0, &uregs);
        ptrace(PTRACE_CONT, pid, 0, 0);

        wait(&status);

        if(WIFEXITED(status)){
            printf("Child process ended \n");
        }

    }


    return 0;
}
