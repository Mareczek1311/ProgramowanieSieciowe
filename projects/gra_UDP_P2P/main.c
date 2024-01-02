#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 1024

int main(int argc, char* argv[]){
   
    if(argc != 3){
        fprintf(stderr, "usage: ip port\n");
        exit(1);
    } 

    char opcja[BUFLEN];
    int sockfd;
    struct sockaddr_in addr;

    struct sockaddr_storage their_addr;
    size_t addr_len = sizeof(struct sockaddr_storage);

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(atoi(argv[2]));

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("socket");
        exit(1);
    }

    if(bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) == 0){
        printf("Port dostepny\n");

        if ((numbytes = recvfrom(sockfd, data, MAXBUFLEN - 1, 0,
                        (struct sockaddr *)&their_addr,
                        (socklen_t *)&addr_len)) == -1)
    }
    else{
        printf("port zajety \n");
    }

    
    printf("Podaj opcje: \n");
    scanf("%s", opcja);
    
    while(strcmp(opcja, "koniec")){
        printf("Podaj opcje: \n");
        scanf("%s", opcja);
    }

    
    printf("---KONIEC---\n");
    close(sockfd);

    return 0;
}
