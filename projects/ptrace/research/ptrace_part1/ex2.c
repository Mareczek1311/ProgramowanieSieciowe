#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/syscall.h>

int main() {
    pid_t pid;
    long orig_rax, rax;
    long params[3];
    int insyscall = 0;
    int status;

    if ((pid = fork()) == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls", "ls", NULL);
    } else {
        while(1){
            wait(&status);
            if(WIFEXITED(status)){
                break;
            }
            orig_rax = ptrace(PTRACE_PEEKUSER, pid, 8 * ORIG_RAX, NULL); // Użyj konkretnej wartości 39 jako numeru wywołania systemowego (SYS_getpid)
            
            if(orig_rax == SYS_write){
                if(insyscall == 0){
                    insyscall = 1;
                    params[0] = ptrace(PTRACE_PEEKUSER, pid, 8 * RBX, 0);
                    params[1] = ptrace(PTRACE_PEEKUSER, pid, 8 * RCX, 0);
                    params[2] = ptrace(PTRACE_PEEKUSER, pid, 8 * RDX, 0);

                    printf("Write called with: %ld %ld %ld \n", params[0], params[1], params[2]);
                }
                else{
                    rax = ptrace(PTRACE_PEEKUSER, pid, 8 * RAX, NULL);
                    printf("Write returned: %ld \n", rax);
                    insyscall = 0;
                }
                ptrace(PTRACE_SYSCALL, pid, 0, 0);
            }

        }
    }

    return 0;
}

