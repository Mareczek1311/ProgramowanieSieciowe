#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>


int main() {
    int pid, status;
	printf("Uruchamiam ls -l za pomoca exec\n");

    if((pid = fork()) == 0){
        execlp("ls","ls","-l", (char *)0);
    }
    else{
	    wait(&status);
        printf("Wazny komunikat na koniec\n");
    }

    return 0;

}

