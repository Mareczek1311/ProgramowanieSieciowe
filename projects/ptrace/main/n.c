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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>

#include <wait.h>
#include <sys/ptrace.h>
int terminate = 0;
static void SignalHandler (int sig)
{
    terminate = 1;
}
int main() {
    int pid, status;
    struct user_regs_struct regs;
    if ((pid = fork()) == 0) {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execl("./child3", "child3", 0);
    } else {
        while (1) {
        int len = 0;
    int waitStat = 0;
    int waitRes = waitpid(pid, &waitStat, WUNTRACED);
    if (waitRes != pid || !WIFSTOPPED(waitStat))
    {
        printf("unexpected waitpid result!\n");
        exit(1);
    }
    printf("waitpid result: pid=%d, stat=%d\n", waitRes, waitStat);

            while(1)
            {
            sleep(1);
            int sigNo = 0;
            const int pRes = ptrace(PTRACE_SINGLESTEP, pid, 0, 0);
            if (pRes < 0)
            {
                perror("singlestep error");
                exit(1);
            }
            ptrace(PTRACE_GETREGS, pid, 0, &regs);


            printf("leni: %d \n", regs.rdx);
            len = regs.rdx;
            }
        long addr = regs.rsi;
        printf("len: %d \n", len);
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

printf("$%s \n", laddr);

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
