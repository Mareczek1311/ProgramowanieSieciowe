#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/user.h>
#include <sys/reg.h>

int main() {
    pid_t pid;
    long orig_rax;
    int status;

    if ((pid = fork()) == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls", "ls", NULL);
    } else {
        wait(&status);

        if (WIFSTOPPED(status)) {
            orig_rax = ptrace(PTRACE_PEEKUSER, pid, 8 * ORIG_RAX, NULL); // Użyj konkretnej wartości 39 jako numeru wywołania systemowego (SYS_getpid)
            printf("Child made a system call number: %ld\n", orig_rax);
            ptrace(PTRACE_CONT, pid, NULL, NULL);
        }
    }

    return 0;
}

