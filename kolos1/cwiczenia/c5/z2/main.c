     #include <unistd.h>

#include <stdio.h>
#include <stdlib.h>


int main(){

    int fd1[2], fd2[2];
    int pid, status;
    int n;

    pipe(fd1);
    pipe(fd2);
    
    if((pid = fork()) == 0){
        close(fd1[1]);
        close(fd2[0]);

        read(fd1[0], &n,  sizeof(int));
        n = 2 * n;

        write(fd2[1], &n, sizeof(int));

    }else{
        close(fd1[0]);
        close(fd2[1]);

        printf("Rodzic: Podaj n >>> ");
        scanf("%d", &n);
        write(fd1[1], &n, sizeof(int));

        read(fd2[0], &n, sizeof(int));

        printf("Rodzic: Dostalem od potomka: %d \n", n);
    }
    
    return 0;
}
