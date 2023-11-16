#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <string.h>


void title(){


 printf(" ▐██▌  ███▄ ▄███▓▓█████  ███▄ ▄███▓ ▒█████   ██▀███ ▓██   ██▓    ▄████▄   ▒█████   ███▄    █ ▄▄▄█████▓ ██▀███   ▒█████   ██▓     ▐██▌ \n");
 printf(" ▐██▌ ▓██▒▀█▀ ██▒▓█   ▀ ▓██▒▀█▀ ██▒▒██▒  ██▒▓██ ▒ ██▒▒██  ██▒   ▒██▀ ▀█  ▒██▒  ██▒ ██ ▀█   █ ▓  ██▒ ▓▒▓██ ▒ ██▒▒██▒  ██▒▓██▒     ▐██▌ \n");
 printf(" ▐██▌ ▓██    ▓██░▒███   ▓██    ▓██░▒██░  ██▒▓██ ░▄█ ▒ ▒██ ██░   ▒▓█    ▄ ▒██░  ██▒▓██  ▀█ ██▒▒ ▓██░ ▒░▓██ ░▄█ ▒▒██░  ██▒▒██░     ▐██▌ \n");
 printf(" ▓██▒ ▒██    ▒██ ▒▓█  ▄ ▒██    ▒██ ▒██   ██░▒██▀▀█▄   ░ ▐██▓░   ▒▓▓▄ ▄██▒▒██   ██░▓██▒  ▐▌██▒░ ▓██▓ ░ ▒██▀▀█▄  ▒██   ██░▒██░     ▓██▒ \n");
 printf(" ▒▄▄  ▒██▒   ░██▒░▒████▒▒██▒   ░██▒░ ████▓▒░░██▓ ▒██▒ ░ ██▒▓░   ▒ ▓███▀ ░░ ████▓▒░▒██░   ▓██░  ▒██▒ ░ ░██▓ ▒██▒░ ████▓▒░░██████▒ ▒▄▄ \n");
 printf(" ░▀▀▒ ░ ▒░   ░  ░░░ ▒░ ░░ ▒░   ░  ░░ ▒░▒░▒░ ░ ▒▓ ░▒▓░  ██▒▒▒    ░ ░▒ ▒  ░░ ▒░▒░▒░ ░ ▒░   ▒ ▒   ▒ ░░   ░ ▒▓ ░▒▓░░ ▒░▒░▒░ ░ ▒░▓  ░ ░▀▀▒ \n");
 printf(" ░  ░ ░  ░      ░ ░ ░  ░░  ░      ░  ░ ▒ ▒░   ░▒ ░ ▒░▓██ ░▒░      ░  ▒     ░ ▒ ▒░ ░ ░░   ░ ▒░    ░      ░▒ ░ ▒░  ░ ▒ ▒░ ░ ░ ▒  ░ ░  ░ \n");
 printf("    ░ ░      ░      ░   ░      ░   ░ ░ ░ ▒    ░░   ░ ▒ ▒ ░░     ░        ░ ░ ░ ▒     ░   ░ ░   ░        ░░   ░ ░ ░ ░ ▒    ░ ░       ░ \n");
 printf(" ░           ░      ░  ░       ░       ░ ░     ░     ░ ░        ░ ░          ░ ░           ░             ░         ░ ░      ░  ░ ░ \n");
 printf("                                                     ░ ░        ░ \n");



}

void options(){


    printf("PICK SCENARIO \n");
    printf("[1] READ FILE \n");
    printf("[2] CHANGE GLOBAL VARIABLE SCENARIO \n");
    printf("[3] OPTION3 \n");
    printf("[4] OPTION4 \n");
    printf("[5] OPTION5 \n");
}

int main() {
    int pid, status;
    int fd[2], fd2[2];
    int opt;
    char filename[64];
    unsigned long adr;
    long data;

    pipe(fd);
    pipe(fd2);

    pid = fork();

    if (pid < 0) {
        perror("Fork error!");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        close(fd[1]);
        close(fd2[1]);

        read(fd[0], &opt, sizeof(int));

        switch (opt) {
            case 1:
                read(fd2[0], filename, sizeof(char) * 64);
                execl("/bin/cat", "cat", filename, NULL);
                perror("execl");
                exit(EXIT_FAILURE);
            case 2:
                ptrace(PTRACE_TRACEME, 0, 0, 0);
                execl("./child3", "child3", 0);
                perror("execl");
                exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        close(fd[0]);
        close(fd2[0]);

        title();
        options();

        printf(">> ");
        scanf("%d", &opt);

        switch (opt) {
            case 1:
                printf("FILE DIR >>> ");
                scanf("%s", filename);
                write(fd[1], &opt, sizeof(int));
                write(fd2[1], filename, sizeof(char) * 64);
                break;

            case 2:

                write(fd[1], &opt, sizeof(int));
                printf("VARIABLE ADDRES >>> ");
                scanf("%lu", &adr);
                while(1){
                    ptrace(PTRACE_SINGLESTEP, pid, 0, 0);
                    wait(&status);
                    if(WIFEXITED(status)){break;}
                    data = ptrace(PTRACE_PEEKDATA, pid, adr, 0);
                    printf("%d \n", data);
                    if(data == 0){
                        printf("CHANGE VARIABLE %ld TO >>> ", data);
                        scanf("%ld", &data);
                        ptrace(PTRACE_POKEDATA, pid, adr, data);
                        ptrace(PTRACE_CONT, pid ,0, 0);
                        break;
                    }
                }

        }

        // Wait for the child process to finish
        wait(NULL);
    }

    return 0;
}
