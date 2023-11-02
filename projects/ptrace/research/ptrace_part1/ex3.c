#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/syscall.h>
#include <string.h>
#include <stdlib.h>
const int long_size = sizeof(long);
void getdata(pid_t child, long addr,
        char *str, int len){
    char* laddr;
    int i, j;
    union u{
        long val;
        char chars[long_size];

    }data;
    i=0;
    j= len/long_size;
    laddr = str;

    while(i<j){
        data.val = ptrace(PTRACE_PEEKDATA, child, addr + i*8, 0);
        memcpy(laddr, data.chars, long_size);
        i++;
        laddr += long_size;

    }
    

    j = len%long_size;
    if(j!=0){
        data.val = ptrace(PTRACE_PEEKDATA, child, addr +i*8, 0);
        memcpy(laddr, data.chars, long_size);
    }

    str[len] = '\0';
    printf("%s \n", str);
}

int main() {
    pid_t pid;
    long orig_rax, rax;
    long params[3];
    int toggle = 0;
    int status;
    char *str, *adr;
    if ((pid = fork()) == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls", "ls", NULL);
    } else {
        while(1){

            wait(&status);

            if(WIFEXITED(status)){
                break;
            }

            orig_rax = ptrace(PTRACE_PEEKUSER, pid, 8*ORIG_RAX, 0);

            if(orig_rax == SYS_write){
                if(toggle == 0){
                    toggle = 1;
               params[0] = ptrace(PTRACE_PEEKUSER,
                                  pid, 8 * RBX,
                                  NULL);
               params[1] = ptrace(PTRACE_PEEKUSER,
                                  pid, 8 * RCX,
                                  NULL);
               params[2] = ptrace(PTRACE_PEEKUSER,
                                  pid, 8 * RDX,
                                  NULL);



               str = (char*) calloc((params[2]+1), sizeof(char));
               getdata(pid, params[1], str, params[2]);
               //reverse();
               //putdata();

                }
                else{
                    toggle = 0;
                }
            }
        ptrace(PTRACE_SYSCALL, pid, 0, 0);
        }
    }


    return 0;
}

