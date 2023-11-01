#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main(){

   pid_t pid;
   int fd[2];

   if((pipe(fd) == -1) || ((pid = fork()) == -1)){
       perror("pipe & fork");
       exit(0);
   }

   if(pid == 0){
       if(dup2(fd[1], STDOUT_FILENO) == -1){
           perror("Nie udalo sie przekierowac wyjscia ls");

       }
       else if((close(fd[0]) == -1) || (close(fd[1]) == -1)){
           perror("Nie udalo sie zamknac lacza");
       }
       else{
           execlp("ls", "ls", "-l", NULL);
           perror("Uruchamianie ls nie powiodlo sie");
       }
   }else{
       if(dup2(fd[0], STDIN_FILENO) == -1){
           perror("nie udalo sie przekierowac wejscia sort");
       }
       else if((close(fd[0]) == -1) || (close(fd[1]) == -1)){
           perror("nie udalo sie zamknac lacza");
       }
       else{
           execlp("sort", "sort", "-f", NULL);
           perror("Uruchamianie sort nie powiodlo sie");
       }
   }

    return 0;
}
