#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>


int main(){
    
    pid_t pid;
    long orig_eax;
    int signal;

    if( (pid = fork())  == 0){
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execl("/bin/ls", "ls", 0);
    }
    else{
        wait(&signal);

        orig_eax = ptrace(PTRACE_PEEKUSER, pid, 4 * ddorig_rax, 0);

        printf("Child made a system call number: %ld \n", orig_eax);

        ptrace(PTRACE_CONT, pid, 0, 0);
    }

    return 0;
}
