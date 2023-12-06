#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

/* Brak obslugi bledow! */

void obsluga_klienta(int fifodesc) {
    char buf[1024];
    int chars;

    while (1) {
        chars = read(fifodesc, &buf, sizeof(buf));
        if (chars > 0) {
            buf[chars] = '\0';
            printf("Klient przyslal %2d bajtow: %s", chars - 1, buf);
        }
    }

    close(fifodesc);
}

int main() {
    int fifodesc;
    pid_t child_pid;

    mkfifo("mojafifo", 0777);
    printf("Serwer: czekam na komunikaty klientow...\n");

    while (1) {
        fifodesc = open("mojafifo", O_RDONLY);

        if (fifodesc != -1) {

            child_pid = fork();

            if (child_pid == 0) {
                close(fifodesc);
                obsluga_klienta(open("mojafifo", O_RDONLY));
                exit(0);
            } else {
                close(fifodesc);
                waitpid(child_pid, NULL, WNOHANG);

            }
        }
    }

    return 0;
}
