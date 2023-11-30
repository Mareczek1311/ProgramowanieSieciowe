#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
     #include <sys/types.h>
     #include <sys/stat.h>
#include<string.h>

#include <sys/errno.h>
void my_hand(int signal){
    printf("Proces potomny zakonczyl dzialanie \n");
}
void revstr(char *str1, int chars)  
{  
    int i, len, temp;  
    len = chars;  
      
    for (i = 0; i < len/2; i++)  
    {  
        temp = str1[i];  
        str1[i] = str1[len - i - 1];  
        str1[len - i - 1] = temp;  
    }  
}  

int main(){
    
    int fd;
    int pid, status;
    size_t chars;
    char* buf = NULL;
    char buf2[1024];
    size_t bufsize = 80;
    char tmp;
    if((mkfifo("fifo", 0777) < 0) && (errno != EEXIST)){
        perror("mkfifo");
    }


    if(signal(SIGCHLD, my_hand) == SIG_ERR){
        perror("signal");
    }

    if((pid = fork()) < 0){
        perror("fork");
        exit(1);
    }else if(pid == 0){
        
        if((fd = open("fifo", O_WRONLY)) == -1){
            perror("open chld");
            exit(1);
        }
        
        printf("Potomek: podaj napis: \n");
        chars = getline(&buf, &bufsize, stdin);
        
        
        revstr(buf, chars);
        
        if(write(fd, buf, chars) == -1){
            perror("write");
            exit(1);
        }
    
        if(close(fd) == -1){
            perror("close chld");
            exit(1);
        }
        
    }else{
        
        if((fd = open("fifo", O_RDONLY)) == -1){
            perror("open parent");
            exit(1);
        }

        chars = read(fd, &buf2, sizeof(buf));
        if(chars == -1){
            perror("read");
            exit(1);
        } 

        printf("Przodek: potomek przeslal: %s \n", buf2);

        if(wait(&status) == -1){
            perror("wait");
            exit(1);
        }

        if(close(fd) == -1){
            perror("close parent");
            exit(1);
        }
    }

    return 0;
}
