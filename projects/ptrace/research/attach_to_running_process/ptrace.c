#include <stdio.h>

#include <sys/ptrace.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>


int main(){
    
    int status, pid;
    unsigned long adr;
    long data;

    printf("Podaj adres zmiennej: \n");
    scanf("%x", &adr);
    printf("\n Podaj pid procesu: \n");
    scanf("%d", &pid);

    ptrace(PTRACE_ATTACH, pid, 0, 0);
    wait(&status);

    ptrace(PTRACE_PEEKDATA, pid, adr, data);
    
    data = 100;

    ptrace(PTRACE_POKEDATA, pid, adr, data);

    ptrace(PTRACE_CONT, pid, 0, 0);

    wait(&status);

    if(WIFEXITED(status)){
        printf("child is dead :( \n )");
    }

    return 0;
}
