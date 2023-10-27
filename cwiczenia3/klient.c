#include <signal.h>
#include <sys/types.h>
#include<stdio.h>
#include<stdlib.h>

//int kill(pid_t pid, int sig);

int main(){

	int sig;
	int pid;
	
	printf("Podaj pid serwera: \n");
	scanf("%d", &pid);


	while(1){

		printf("Podaj sygnal: \n");
		scanf("%d", &sig);
		switch(sig){
			case 1:
				kill(pid, SIGUSR1);
				break;
			case 2:
				kill(pid, SIGUSR2);
				break;
			case 0:
				exit(0);
				break;
			default:
				printf("Zla akcja \n");

		}
	
	}
		

	return 0;
}


