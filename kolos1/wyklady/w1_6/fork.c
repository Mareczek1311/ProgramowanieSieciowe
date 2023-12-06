#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int glob = 2;
int main() {
    int var = 0, status;
    pid_t pid;

    if ((pid = fork()) < 0) { // Pojawił się błąd, tylko proces rodzica
        perror("Blad:\n");
    } else if (pid == 0) { // proces potomny
        glob++;
        var++;
    } else { // proces rodzica
        wait(&status);
    }

    printf("proces = %s, pid = %d  ppid = %d  uid = %d euid = %d  gid = %d "
           "egid = %d\n var = %d, glob = %d\n ",
           pid == 0 ? "potomek" : "rodzic", getpid(), getppid(), getuid(),
           geteuid(), getgid(), getegid(), var, glob);
    return 1;
}