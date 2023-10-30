#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>


void print_signal(int signal){
	
	switch(signal){
		case 2:
			printf("Przechwycono sigInt \n");
			break;
		case 3:
			printf("Przechwycono sigQUIT \n");
			break;

		case 18:
			printf("Przechwycono sigTSTP \n");
			break;

		case 9:
			printf("Przechwycono sigKILL \n");
			break;

		case 30:
			printf("Przechwycono sigUSR1 \n");
			break;
		default:
			printf("Brak informacji \n");
	}

}
int main(){

	
	signal(2, print_signal);
	signal(3, print_signal);
	signal(18, print_signal);
	signal(9, print_signal);
	signal(30, print_signal);

	for(int i=1; i<=10; i++){	
	
		sleep(1);
		printf("dzialam od %d sek \n", i);	
	}

	return 0;
}

