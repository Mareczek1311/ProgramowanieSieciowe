
#define _WITH_GETLINE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

/* Brak obslugi bledow! */

int main() {

	int    fifodesc;
	char   *buf = NULL;
	size_t bufsize = 80;
	size_t chars;

	printf("Klient: napisz komunikat ktory chcesz przeslac do serwera:\n");
	chars = getline(&buf, &bufsize, stdin);

	fifodesc = open("mojafifo", O_WRONLY);
	if(fifodesc != -1) {
	
		write(fifodesc, buf, chars);
		close(fifodesc);

	}

	return 0;

}
