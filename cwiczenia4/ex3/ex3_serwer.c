#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

/* Brak obslugi bledow! */
int liczbaPot = 0;

void signal_handler(int signal){
	int status;

	waitpid(-1, &status, WNOHANG);
	if(WIFEXITED(status)){
		printf("Potomek zakonczyl prace \n");
		liczbaPot--;
		printf("Aktualna liczba potomkow: %d \n", liczbaPot);
	}
}	

int main() {

	int    fifodesc;
	char   buf[1024];
	int    chars;
	int pid, status;
	mkfifo("mojafifo", 0777);
	printf("Serwer: czekam na komunikaty klientow...\n");
	signal(SIGCHLD, signal_handler);
	while(1){
		if((pid = fork()) == 0){
			fifodesc = open("mojafifo", O_RDONLY);
			if(fifodesc != -1) {

				chars = read(fifodesc, &buf, sizeof(buf));
				if(chars > 0) {
					buf[chars] = '\0';
					printf("POTOMEK: Klient przyslal %2d bajtow: %s", chars-1, buf);	
				}
							
				close(fifodesc);

			}
			exit(0);
		}
		else{
			liczbaPot ++;
			printf("Utworzono nowego potomka (%d) \n", liczbaPot);
 
			while(liczbaPot >= 1){
				waitpid(-1, &status, WNOHANG);
				printf("Praca serwera...\n");
				sleep(1);
				//praca serwera
			}

		}
	}


	return 0;

}
