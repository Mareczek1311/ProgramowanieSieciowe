#include <stdio.h>
#include <dirent.h>
     #include <fcntl.h>
     #include <sys/types.h>
     #include <sys/uio.h>
     #include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    
    if(argc != 2){
        printf("-- ./a.out [sciezka_do_pliku] --\n");
    }
    
    int n;
    int fd;
    char buf[1024];
    
    if((fd = open(argv[1], O_RDONLY)) == -1){
        perror("open");
    }

    while((n = read(fd, buf, 1024)) > 0){
        write(fileno(stdout), buf, n); 
    }
    
    close(fd);

	return 0;
}

