     #include <fcntl.h>
#include<stdio.h>
     #include <unistd.h>


#define _WITH_GETLINE

int main(){
    
    int fd;
    char *buf = NULL;
    size_t chars;
    size_t bufsize = 80;
        
    printf("Podaj komunikat: "); //BARDZO WAZNE WEJSCIE 
    chars = getline(&buf, &bufsize, stdin);


    fd = open("mojaFifo", O_WRONLY);
    if(fd != -1){
        write(fd, buf, chars);
        close(fd);
    }

    return 0;
}
