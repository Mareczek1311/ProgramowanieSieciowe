     #include <sys/types.h>
     #include <sys/stat.h>
     #include <fcntl.h>
     #include <unistd.h>
     #include <sys/uio.h>
    #include <stdio.h> 
     #include <stdlib.h>


int main(){
    

    int fd;
    char buf[1024];
    int chars;
    mkfifo("mojaFifo", 0777);
    printf("Komunukaty... \n");
    fd = open("mojaFifo", O_RDONLY);
   if(fd != 1)
   {
    while(1){
        chars = read(fd, &buf, sizeof(buf));
        if(chars > 0){
            buf[chars] = '\0';
            printf("Serwer: dostaje info od potomka o dl %d: %s \n", chars, buf);
        }
    
    close(fd);
    }
   }
    return 0;
}
