#include<stdio.h>
#include <signal.h>
#include <stdlib.h>

void byby(void) { printf("\t -> atexit - papa \n"); }

int main(int argc, char* argv[]){


    if(argc != 2){
        printf("A - abort; S - raise; E - exit; R - return \n");
        exit(0);
    }

    atexit(byby);
    
    switch(argv[1][0]){
        
        case 'a':
        case 'A':
            printf("Koniec - Abort \n");
            abort();
            break;
        
        case 's':
        case 'S':
            printf("Koniec - raise \n");
            raise(9);
            break;
        
        case 'e':
        case 'E':
            printf("Koniec - exit \n");
            exit(0);
            break;

        default:
            printf("Koniec - return");
            return 0;

    };
}