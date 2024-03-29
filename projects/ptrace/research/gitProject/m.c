#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <signal.h>

int main() {
    int pid, status;
    struct user_regs_struct regs;

    if ((pid = fork()) == 0) {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execl("./child", "child", 0);
    } else {
        while (1) {
            wait(&status);
            ptrace(PTRACE_CONT, pid, 0, 0);
            sleep(1);
            kill(pid, SIGINT);
            wait(&status);
            ptrace(PTRACE_GETREGS, pid, 0, &regs);


            printf("Value in RBX: %lld\n", regs.rbx);

            regs.rbx = 245;

            printf("New value in RBX: %lld\n", regs.rbx);
            // Set the new value of RBX in the child's register
            ptrace(PTRACE_SETREGS, pid, 0, &regs);

            ptrace(PTRACE_CONT, pid, 0, 0);
            wait(&status);

            if (WIFEXITED(status)) {
                break;
            }
        }
    }

    return 0;
}
