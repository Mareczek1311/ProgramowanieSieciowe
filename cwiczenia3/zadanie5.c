#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>

void print_status(int status){
	printf("Dostalem od potomka wiadomosc: %d \n", status);
}

int main(){
	srand(time(NULL));
	pid_t pid, parent_id;
	int status, randNum;
	

	if((pid = fork()) < 0){
		perror("fork");
	}
	else if(pid == 0){
		parent_id = getppid();
		randNum = rand()%2;
		for(int i=0; i<10; i++){
			if(randNum == 1){
				kill(parent_id, SIGUSR1);
			}else{
				kill(parent_id, SIGUSR2);
			}
			sleep(1);
		}

	}
	else{
		while(1){
			signal(SIGUSR1, print_status);	
			signal(SIGUSR2, print_status);
			
			sleep(1);
		}	
	}

	return 0;
}
