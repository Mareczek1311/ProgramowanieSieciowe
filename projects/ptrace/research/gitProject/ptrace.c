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

void *mapfile(const char *path, long *filesize){
    FILE* file = fopen(path, "rb");
    int fd;
    void *content;
    fseek(file, 0, SEEK_END);
    fd = fileno(file);

    *filesize = ftell(file);
    rewind(file);

    content = nmap(NULL, *filesize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    
    fclose(file);

    return content;
}

int main(){

    int pid, status;
    struct user_regs_struct regs;
    const char *path = "./ex";
    char* content;
    long filesize;

    if((pid = fork()) == 0){
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execl("./ex", "ex", 0);
    }else{
        wait(&status);
        ptrace(PTRACE_GETREGS, pid , 0, &regs);
        
        content = mapFile(path, &filesize)
        
    
    }
    
        
    return 0;
}


