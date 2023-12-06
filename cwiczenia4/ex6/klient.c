#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong arguments! (./k username message)\n");
        return 1;
    }

    char fifoname[64] = "";
    char prefix[64] = "./tmp/fifo_";
    int fifodesc;

    char buff1[64] = "";
    char buff2[64] = "";

    strcat(fifoname, prefix);
    strcat(fifoname, argv[1]);

    // Otwórz plik do zapisu (lub utwórz, jeśli nie istnieje)

    strcat(buff1, argv[1]);
    strcat(buff2, argv[2]);
    printf("Klient [%s] wysyla komunikat: %s \n", buff1, buff2);
    printf("Klient: Otwieram FIFO o nazwie %s \n", fifoname);
    fifodesc = open(fifoname, O_WRONLY);


    if(fifodesc != -1){
        printf("Klient: klient (%s) wysyla komunikat: %s \n", argv[1], argv[2]);
     //   write(fifodesc, buff1, sizeof(buff1));
     //   write(fifodesc, buff2, sizeof(buff2));
        write(fifodesc, argv[1], sizeof(argv[1]));
        write(fifodesc, argv[2], sizeof(argv[2]));
        close(fifodesc);
    }
    else{
        perror("Potok o takiej nazwie nie istnieje");
    }

    return 0;
}
