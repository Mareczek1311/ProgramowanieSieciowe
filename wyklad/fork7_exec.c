#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){

    int status; pid_t pid;

    if ((pid = fork()) < 0){
        perror("fork ");
    }
    else if(pid == 0){
        if(execl("./hw", "hw", NULL) < 0){
            perror("execl ");
        }
    }
    else{
        if(wait(&status) != pid){
            perror("wait ");
        }
    }

    return 0;
}