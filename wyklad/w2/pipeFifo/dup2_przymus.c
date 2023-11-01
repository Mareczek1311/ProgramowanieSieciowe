#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    pid_t childpid;
    int fd[2];

    if ((pipe(fd) == -1) || ((childpid = fork()) == -1)) {
        perror("Nie udalo sie zestawienie potoku");
        return 1;
    }

    if (childpid == 0) { /* dziecko uruchomi ls */
        if (dup2(fd[1], STDOUT_FILENO) == -1)
            perror("Nie udalo sie przekierowanie wyjscia ls");
        else if ((close(fd[0]) == -1) || (close(fd[1]) == -1))
            perror("Nie udalo sie zamkniecie lacza");
        else {
            execlp("ls", "ls", "-l", NULL);
            perror("Nie udalo sie uruchomienie ls"); // exec zwraca wartosc tylko jesli sie nie powiodl
        }
    } else {
        if (dup2(fd[0], STDIN_FILENO) == -1) /* rodzic uruchomi sort */
            perror("Nie udalo sie przekierowanie wejscia sort");
        else if ((close(fd[0]) == -1) || (close(fd[1]) == -1))
            perror("Nie udalo sie zamkniecie lacza");
        else {
            execlp("sort", "sort", "-f", NULL);
            perror("Nie udalo sie uruchomienie sort");
        }
    }
}
