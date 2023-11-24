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
	size_t bufsize = 64;
	size_t chars;
    pid_t pid;
    pid = getpid();
    buf = (char*) malloc (bufsize);
	
	printf("Klient: napisz komunikat ktory chcesz przeslac do serwera:\n");
	chars = getline(&buf, &bufsize, stdin);
	printf("Wiadomosc jaka wysle: %s \n", buf);

	fifodesc = open("mojafifo", O_WRONLY);


	if(fifodesc != -1) {

		write(fifodesc, &pid, sizeof(pid_t));
		write(fifodesc, buf, chars);
		close(fifodesc);

	}
    free(buf);
	return 0;

}
