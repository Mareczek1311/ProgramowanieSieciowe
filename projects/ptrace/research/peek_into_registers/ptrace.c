#include <sys/ptrace.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>

int main(){
    
    int i=0, status = 0, pid;
    struct user_regs_struct uregs;

    if((pid = fork()) == 0){
        
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execl("./child3", "child3", 0);
        printf("exec failed... \n");

    }else{
        wait(&signal);
        ptrace(PTRACE_CONT, pid, 0, 0);

        //wait for child to get to the loop
        sleep(1);

        kill(pid, SIGINT);
        
        wait(&status);

        ptrace(PTRACE_GETREGS, pid, 0, &uregs);

        printf("exb = %d \n", uregs.ebx);

        uregs.ebx = 245;

        ptrace(PTRACE_SETREGS, pid, 0, &uregs);
        ptrace(PTRACE_CONT, pid, 0, 0);

        wait(&status);

        printf("Parent: out of wait: %d ... \n", WIFSTOPPED(status));

        sleep(5);
        

    }
    

    return 0;
}
