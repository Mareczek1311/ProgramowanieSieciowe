     #include <sys/types.h>
     #include <sys/stat.h>
     #include <unistd.h>
     #include <fcntl.h>
    #include<stdio.h>

int main(){

    
    int fd1, fd2;
    int n;
    printf("Podaj liczbe: ");
    scanf("%d", &n);

    fd1 = open("fifo1", O_WRONLY);
    write(fd1, &n, sizeof(int));
    close(fd1);

    fd2 = open("fifo2", O_RDONLY);
    read(fd2, &n, sizeof(int));
    close(fd2);

    printf("Liczba: %d", n);

    return 0;
}
