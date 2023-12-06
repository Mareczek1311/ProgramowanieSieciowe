#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFS 1024

int main(int argc, char* argv[]){
    int fd, n;
    char buf[BUFS];

    for(int i=1; i<argc; i++){
        if((fd = open(argv[i], O_RDONLY)) == -1){
            perror("open");
            exit(1);
        }
        while((n = read(fd, buf, BUFS)) > 0){
            write(fileno(stdout), buf, n);
        }
    }
    close(fd);

    return 0;
}
