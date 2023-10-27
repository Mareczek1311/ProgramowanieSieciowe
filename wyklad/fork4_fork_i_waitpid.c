#include<stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int glob = 2;

void print_status(int status) {
    if (WIFEXITED(status))
        printf("normal exit, status = %d\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf(" exit, status = %d\n", WTERMSIG(status));
    else if (WIFSTOPPED(status))
        printf("exit, status = %d\n", WIFSTOPPED(status));
}

int main(){

    int var = 0, status;
    pid_t pid;


    if((pid = fork()) < 0){
        perror("fork ");
    }
    else if(pid == 0){
        glob++;
        var++;
        exit(32);
    }
    else{
        if(waitpid(pid, &status, 0) != pid){
            perror("waitpid ");
        }

        print_status(status);

    }


    return 0;
}