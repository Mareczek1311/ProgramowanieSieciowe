#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int plik, n;
    char bufor[100];

    if((plik  = open(argv[1], O_RDONLY)) == -1){
        perror("Error file");
        exit(EXIT_FAILURE);
    }

    while ((n = read(plik, bufor, sizeof(bufor))) > 0){
        write(fileno(stdout), bufor, n);
    }

    close(plik);

}

