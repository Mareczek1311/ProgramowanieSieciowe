#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFS 1024

int main(int argc, char* argv[]){

    int fd, fd2, n;
    char buf[BUFS];

    if((fd = open(argv[1], O_RDONLY)) != -1 && (fd2 = open("output.txt", O_WRONLY)) != -1){
        while((n = read(fd, buf, BUFS)) > 0){
            write(fd2, buf, n);
        }
    }
    else{
        perror("open");
    }

    close(fd);
    close(fd2);

    return 0;

}