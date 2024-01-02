#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include <strings.h>
#define MAXBUFFER 1024

void print_client_info(struct sockaddr* their_addr){
    char p_addr[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &(((struct sockaddr_in *) their_addr))->sin_addr, p_addr, sizeof(p_addr));
    printf("[Server]: got connection from: %s\n", p_addr);
}

void get_request_and_reply(int sockfd){
    char buffer[MAXBUFFER];
    int numbytes;
    struct sockaddr_storage their_addr;
    size_t addrlen = sizeof(struct sockaddr_storage);

    if((numbytes = recvfrom(sockfd, buffer, MAXBUFFER-1, 0, (struct sockaddr*) &their_addr, 
                    (socklen_t*) &addrlen)) == -1){
        perror("recvfrom");
        exit(1);
    }
    
    print_client_info((struct sockaddr*) &their_addr);

    printf("[Server]: klient pisze: \n");

    if(write(1, buffer, numbytes) < 0){
        perror("write");
        exit(1);
    }

    if((numbytes = sendto(sockfd, buffer, numbytes, 0, (struct sockaddr*) &their_addr, addrlen)) == -1){
        perror("sendto");
        exit(1);
    }


}

int setup_server(char *addr, char *port){
    int sockfd, sockopt, yes;

    struct in_addr naddr;
    struct sockaddr_in server_addr;

    if(inet_pton(AF_INET, addr, &naddr) < 1){
        perror("pton");
        exit(1);
    }

    bzero(&server_addr, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(port));
    server_addr.sin_addr = naddr;

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("socket");
        exit(1);
    }

    sockopt = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    if (sockopt == -1){
        perror("sockopt");
        exit(1);
    }
    
    if(bind(sockfd, (struct sockaddr* )&server_addr, sizeof(struct sockaddr)) == -1){
        perror("bind");
        exit(1);
    }

    return sockfd;

}

int main(int argc, char* argv[]){
    
    int sockfd;

    if(argc != 3){
        fprintf(stderr, "usage: server_ip port");
        exit(0);
    }
    
    sockfd = setup_server(argv[1], argv[2]);
    
    printf("[Server]: waiting for connection... \n");

    while(1){
        get_request_and_reply(sockfd);
    }

    return 0;
}
