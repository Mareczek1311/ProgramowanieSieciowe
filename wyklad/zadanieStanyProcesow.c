#include<stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

void print_status(int status) {
    if (WIFEXITED(status))
        printf("normal exit, status = %d\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf(" exit, status = %d\n", WTERMSIG(status));
    else if (WIFSTOPPED(status))
        printf("exit, status = %d\n", WIFSTOPPED(status));
}

int main(){

    int status;
    pid_t child1, child2, child3;

    if((child1 = fork()) < 0) { perror("fork 1 "); }
    else if(child1 == 0){
        sleep(3);

        exit(1);
    }
    else{
        if(waitpid(-1, &status, 0) != child1)
        {
            perror("waitpid 1");
        }
        print_status(status);
    }
    if((child2 = fork()) < 0) { perror("fork 2 "); }
        else if(child2 == 0){

        exit(2);
    }
    else{
        if(waitpid(child2, &status, 0) != child2)
        {
            perror("waitpid 2");
        }
        print_status(status);
    }
    
    waitpid(-1, &status, 0);

    return 0;
}