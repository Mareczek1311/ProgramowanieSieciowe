#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <string.h>
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

        printf("len: %d \n", regs.rdx);

        int len = regs.rdx;
        long addr = regs.rsi;
        union u {
                long val;
                char chars[regs.rdx];
            }data;

            char *laddr = (char*) malloc(20*sizeof(char));
            int i,j;
            i = 0;
            j = len/sizeof(long);

            while(i<j){
                data.val = ptrace(PTRACE_PEEKDATA, pid, addr+i*8, 0);
                ++i;
                memcpy(laddr, data.chars, sizeof(long));
            }

            j = len % sizeof(long);

if(j!=0){
    data.val = ptrace(PTRACE_PEEKDATA, pid, addr +i*8, 0);
    memcpy(laddr, data.chars, j);
}
laddr[len]='\0';

printf("Wiadomosc: %s \n", laddr);

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
