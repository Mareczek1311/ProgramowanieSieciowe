#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>

int main(){
    int pid, status;
    long orig_rax;

    if(pid == 0){
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execl("./reg", "reg", 0);
    }else{

        wait(&status);
        ptrace(PTRACE_SINGLESTEP, pid, 0, 0);

    }


    return 0;
}
