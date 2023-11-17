#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
/* Brak obslugi bledow! */
#include <string.h>
int main() {

	int    fifodesc;
	char   buf[1024];
	int    chars;
    pid_t pid;
	mkfifo("mojafifo", 0777);
	printf("Serwer: czekam na komunikaty klientow...\n");

	fifodesc = open("mojafifo", O_RDONLY);

	if(fifodesc != -1) {

		while(1) {
			chars = read(fifodesc, &buf, sizeof(buf));
			if(chars > 0) {
                memcpy(&pid, buf, sizeof(pid_t));

                char* msg = buf + sizeof(pid_t);

				buf[chars] = '\0';
				printf("Klient (%d) przyslal %2d bajtow: %s", pid, chars - sizeof(pid)-1, msg);
			}
		}

		close(fifodesc);

	}

	return 0;

}
