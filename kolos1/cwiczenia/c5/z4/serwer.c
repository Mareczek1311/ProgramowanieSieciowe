     #include <sys/types.h>
     #include <sys/stat.h>
     #include <unistd.h>
     #include <fcntl.h>

int main(){
    
    int fd1, fd2;
    mkfifo("fifo1", 0777);
    mkfifo("fifo2", 0777);
    
    int n;
    

    while(1){
    
        fd1 = open("fifo1", O_RDONLY);
        read(fd1, &n, sizeof(int));
        close(fd1);

        n = n*2;
        
        fd2 = open("fifo2", O_WRONLY);
        write(fd2, &n, sizeof(int));
        close(fd2);
    }
    

    return 0;
}
