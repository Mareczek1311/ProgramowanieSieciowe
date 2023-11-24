#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>


int main(){

    int fifodesc1;
    int fifodesc2;
    int n;

    printf("Podaj liczbe: ");
    scanf("%d", &n);

    fifodesc1 = open("fifo1", O_WRONLY);

    if(fifodesc1 != -1){
        write(fifodesc1, &n, sizeof(int));
        close(fifodesc1);
    }
    fifodesc2 = open("fifo2", O_RDONLY);

    if(fifodesc2 != -1){
        read(fifodesc2, &n, sizeof(int));
        close(fifodesc2);
    }

    printf("Klient: od serwera %d \n", n);

    return 0;
}