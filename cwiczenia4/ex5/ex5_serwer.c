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

    mkfifo("fifo1", 0777);
    mkfifo("fifo2", 0777);

    while(1){
        fifodesc1 = open("fifo1", O_RDONLY);

        if(fifodesc1 != -1){
            read(fifodesc1, &n, sizeof(int));
            close(fifodesc1);
        }

        printf("SERWER: od klienta: %d \n", n);
        n = n*2;
        fifodesc2 = open("fifo2", O_WRONLY);

        if(fifodesc2 != -1){
            write(fifodesc2, &n, sizeof(int));
            close(fifodesc2);
        }
    }

    return 0;
}