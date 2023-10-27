#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char* argv[]){

    if(argc < 2){
        printf("\n");
        exit(0);
    }

    int status;
    pid_t pid;
    char path[16] = "/bin/";
    char** arg = (char**) malloc (sizeof(char*) * (argc - 1)); 
    for(int i = 1; i < argc; i++){
        arg[i-1] = argv[i];
    }

    strcat(path, argv[1]);
    
    if((pid = fork()) < 0){
        perror("fork ");
    }
    else if(pid == 0){
        if(execv(path, arg) < 0){
            perror("execl");
        }
    }
    else{
        if(wait(&status) != pid){
            perror("wait");
        }
    }

    return 0;
}