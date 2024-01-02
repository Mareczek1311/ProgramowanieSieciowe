#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXBUFLEN 1024

int exit_with_perror(char *msg) {
    perror(msg);
    exit(0);
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void print_client_info(struct sockaddr *their_addr) {
    char presentation_addr[INET6_ADDRSTRLEN];
    inet_ntop(their_addr->sa_family,
              get_in_addr(their_addr), presentation_addr,
              sizeof(presentation_addr));
    printf("got from %s\n", presentation_addr);
}


void communicate_with_server(char *address, char *port) {
    int sockfd; // deskryptor socket
    struct in_addr naddr;
    struct sockaddr_in server_addr;

    int numbytes;
    char data[MAXBUFLEN];

    struct sockaddr_storage their_addr;
    size_t addr_len = sizeof(struct sockaddr_storage);

    // Translacja prezentacja na sieciowy
    if (inet_pton(AF_INET, address, &naddr) < 1)
        exit_with_perror("Błąd pton");

    // Adresacja
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(port));
    server_addr.sin_addr = naddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        exit_with_perror("Socket");

    printf("client: podaj dane\n");
    if ((numbytes = read(0, data,  MAXBUFLEN)) == -1)
        exit_with_perror("Błąd czytania");

    if ((numbytes = sendto(sockfd, data, numbytes, 0,
                             (struct sockaddr *)&server_addr,
                             addr_len)) == -1)
        exit_with_perror("sendto");

    if ((numbytes = recvfrom(sockfd, data, MAXBUFLEN - 1, 0,
                             (struct sockaddr *)&their_addr,
                             (socklen_t *)&addr_len)) == -1)
        exit_with_perror("recvfrom");

    print_client_info((struct sockaddr *)&their_addr);
    printf("Serwer pisze: \n");
    if (write(1, data, numbytes) < 0)
        exit_with_perror("write");


    close(sockfd);
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "usage: client ip port\n");
        exit(0);
    }

    communicate_with_server(argv[1], argv[2]);

    return 0;
}
