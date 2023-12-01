#define _WITH_GETLINE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

FILE *file;
char fifoname[64] = "";

void signal_handler(int signal){
    fclose(file);
    unlink(fifoname);
    printf("Serwer: serwer zakończył działanie!\n");
    exit(EXIT_SUCCESS);
}

int main(){
    char filename[64] = "";
    char line[64] = "";
    char prefix[64] =  "./tmp/fifo_";
    char user[64];
    int fifodesc;
    char buf1[64] = "";
    char buf2[64] = "";
    int userLen;
    int msgLen;
    printf("Serwer: Podaj login >> ");
    scanf("%s", user);

    strcat(fifoname, prefix);
    strcat(fifoname, user);
    strcat(filename, fifoname);
    strcat(filename, ".txt");

    printf("FIFO o nazwie: %s zostaje utworzone \n", fifoname);

    mkfifo(fifoname, 0777);

    printf("Serwer: Otwieram PLIK o nazwie %s \n", filename);
    file = fopen(filename, "a"); // "a" oznacza tryb dopisywania do pliku
    fifodesc = open(fifoname, O_RDONLY);
    if (file == NULL) {
        perror("Błąd podczas otwierania pliku");
        return 1; // Zakończ program z kodem błędu
    }

    signal(SIGINT, signal_handler);

    if(fifodesc != -1){
        while(1){
            userLen = read(fifodesc, buf1, sizeof(buf1) - 1);
            msgLen = read(fifodesc, buf2, sizeof(buf2) - 1);
            if (userLen > 0) {

                strcat(line, "[");
                strcat(line, buf1);
                strcat(line,  "] ");
                strcat(line, buf2);
                fprintf(file, "%s\n", line);
                memset(line, 0, sizeof(line));
                printf("Serwer: Klient (%s) przeslal dane %s \n", buf1, buf2);    
            }
        }
        close(fifodesc);
    }
    else{
        perror("fifodesc");
    }

    return 0;
}
