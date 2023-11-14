#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>

int main(){

    int pid, status;
    struct user_regs_struct regs;
    int option;
    if((pid = fork()) == 0){
        ptrace(PTRACE_TRACEME, 0,0,0);
        execl("./func", "func", 0);
    }else{

        while(1){

        wait(&status);

        ptrace(PTRACE_GETREGS, 0, &regs);


        char cmdbuf[10];
        int rsp_val = ptrace(PTRACE_PEEKDATA, pid, regs.rbx, NULL);

        printf("Warotsc RAX: %d \n", rsp_val);
        ptrace(PTRACE_SINGLESTEP, pid, 0, 0);
        scanf("%d", &option);
        printf("\n");
        ptrace(PTRACE_CONT, pid, 0, 0);

    }
    }
    return 0;
}
