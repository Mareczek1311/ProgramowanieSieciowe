#include <sys/ptrace.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
    
    int pid, status;

    if((pid = fork()) == 0){
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execl("./child2", "child2", 0);
        printf("exec failed... \n");
    }else{
        wait(&status);
        if(WIFSTOPPED(status)){
            printf("child has stopped... \n");

        }
        sleep(4);
        ptrace(PTRACE_CONT, pid, 0, 0);
        wait(&status);
    }
        

    return 0;
}
