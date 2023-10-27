#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>

int main(){

	for(int i=1; i<=60; i++){	
		sleep(1);
		printf("dzialam od %d sek \n", i);	
	}

	return 0;
}

