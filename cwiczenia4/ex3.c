#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

/* Brak obslugi bledow! */

int main() {

	int    fifodesc;
	char   buf[1024];
	int    chars;
    int pid, status;


   	mkfifo("mojafifo", 0777);
    printf("Serwer_rodzic: kolelejka fifo zostala utworzona  \n");


    fifodesc = open("mojafifo", O_RDONLY);

    if(fifodesc != -1) {
        while(1) {
		    chars = read(fifodesc, &buf, sizeof(buf));
		    if(chars > 0) {
                buf[chars] = '\0';
			    printf("Potomek: Klient przyslal %2d bajtow: %s", chars-1, buf);
            }
	    }

    }


	return 0;

}
