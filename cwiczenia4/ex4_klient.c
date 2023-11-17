#define _WITH_GETLINE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
/* Brak obslugi bledow! */

int main() {

	int    fifodesc;
	char   *buf = NULL;
	size_t bufsize = 80;
	size_t chars;
    pid_t pid;
    pid = getpid();
    buf = (char*) malloc ((bufsize + sizeof(pid_t)));
	printf("Klient: napisz komunikat ktory chcesz przeslac do serwera:\n");
	chars = getline(&buf + sizeof(pid_t), &bufsize, stdin);
    memcpy(&buf, &pid, sizeof(pid_t));
	fifodesc = open("mojafifo", O_WRONLY);
	if(fifodesc != -1) {

		write(fifodesc, buf, chars);
		close(fifodesc);

	}
    free(buf);
	return 0;

}
