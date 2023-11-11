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

#define printreg(REGNAME) printf("%3s: 0x%016lx\n", #REGNAME, regs.REGNAME)
int main(){

    int pid, status;
    struct user_regs_struct regs;
    int option;
    if((pid = fork()) == 0){
        ptrace(PTRACE_TRACEME, 0,0,0);
        execl("./ex", "ex", 0);
    }else{
            
        while(1){
        
        wait(&status);
    
        ptrace(PTRACE_GETREGS, 0, &regs);


        printreg(rax);
        printreg(rbx);
        printreg(rcx);
        printreg(rdx);
        printreg(rsi);
        printreg(rdi);
        printreg(rsp);
        printreg(rbp);
        printreg(r8);
        printreg(r9);
        printreg(r10);
        printreg(r11);
        printreg(r12);
        printreg(r13);
        printreg(r14);
        printreg(r15);
        char cmdbuf[10];
        for(int i = 0; i < 10; i++) {
          void* rsp_addr = (void*)((unsigned long int)(regs.rsp + 8*i));
          long rsp_val = ptrace(PTRACE_PEEKDATA, pid, rsp_addr, NULL);
          if(rsp_val != -1) {
            printf("(rsp + %d): 0x%016lx\n", i, rsp_val);
          }
        }
        
        printf("Warotsc RAX: %d \n", regs.eax);
        ptrace(PTRACE_SINGLESTEP, pid, 0, 0);        
        scanf("%d", &option);
        printf("\n");
        ptrace(PTRACE_CONT, pid, 0, 0);
            if(WIFEXITED(status)){
                break;
            }
        }

    }

    return 0;
}
