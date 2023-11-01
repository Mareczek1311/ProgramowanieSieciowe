#include<stdio.h>
#include <stdio.h>
#include <signal.h>
#include<sys/types.h>

#include <unistd.h>

void print_signal(int signal){
	
	printf("Serwer: dostalem od klienta wiadomosc typu: %d \n", signal); 

}	

int main(){
	
	signal( SIGUSR1, print_signal);
	signal(SIGUSR2, print_signal);
	
	printf("%d \n", getpid());
	for(int i=0; i<60; i++){
		sleep(1);
	}


	return 0;
}
