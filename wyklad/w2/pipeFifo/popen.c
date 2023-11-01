#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#define MAXLINE 1024
#define PAGER "/usr/bin/more"



int main(int argc, char** argv){

    
    char line[MAXLINE];
    FILE *fpin, *fpout;

    if(argc != 2){
        printf("Uzycie: ./a.out sciezka_do_pliku \n");
        exit(0);
    }

    if((fpin = fopen(argv[1], "r")) == NULL){
        perror("open");
        exit(0);
    }
    if((fpout = popen(PAGER, "w"))== NULL){
        perror("blad");
        exit(0);
    }
    while(fgets(line, MAXLINE, fpin) != NULL){
        if(fputs(line, fpout) == EOF){
            perror("blad");
            exit(0);
        }

    }

    if(ferror(fpin)){
        perror("blad");
    }
    if(pclose(fpout)){
        perror("blad");
    }

    return 0;
}
