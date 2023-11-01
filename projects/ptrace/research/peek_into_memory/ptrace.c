#include <stdio.h>

#include <sys/ptrace.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>



int main(int argc, char** argv){

    int pid, status;

    if((pid = fork()) == 0){
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        
        execl("./child4", "child4", 0);

        printf("exec failed... \n");

    }else{
         wait(&status);
         if(WIFSTOPPED(status)){
             printf("Child stopped! \n");
         }
        
        ptrace(PTRACE_CONT, pid, 0, 0);

        sleep(1);

        kill(pid, SIGINT);


         wait(&status);
         if(WIFSTOPPED(status)){
             printf("Child stopped! \n");
         }

        unsigned long adr = 0x601038;
        
        long data = ptrace(PTRACE_PEEKDATA, pid, adr, 0);

        printf("DATA: %d \n", data);
        data = 245;
        printf("DATA: %d \n", data);

        ptrace(PTRACE_POKEDATA, pid, adr, data);

        ptrace(PTRACE_CONT, pid, 0, 0);

        wait(&status);
        


    }

    return 0;
}
