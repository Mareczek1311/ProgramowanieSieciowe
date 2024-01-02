#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

/* A. Mroz - zad. na SK, do modyfikacji */
/* bez pelnej obslugi bledow! */

struct my_msg{
    char name[16];
    char text[255];
};

int main() {

	int sockfd;
	u_short my_port;
	struct sockaddr_in server_addr;
	struct my_msg msg;
    struct sockaddr_in client_addr;
    char ip[INET_ADDRSTRLEN];
    socklen_t addr_len = sizeof(client_addr);

	/* wybieram "unikalny" numer portu dla serwera */
	my_port = 5000 + (getpid() % 10000);

	/* tworze gniazdo - na razie tylko czesc "protokol" */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	/* podpinam gniazdo pod  konkretny "adres-lokalny" 
	   i "proces-lokalny" (= port) */
	server_addr.sin_family	    = AF_INET;           /* IPv4 */
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* dowolny (moj) interfejs */
	server_addr.sin_port        = htons(my_port);    /* moj port */
	bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	printf("[Serwer]: Utworzone gniazdo, slucham na porcie: %d\n", my_port); 

	while(1) {
		recvfrom(sockfd, &msg, sizeof(msg), 0,(struct sockaddr *) &client_addr, &addr_len);
		inet_ntop(AF_INET, &client_addr.sin_addr, ip , INET_ADDRSTRLEN);
        printf("Wiadomosc od %s (IP: %s, port %d): %s\n", msg.name, ip, ntohs(client_addr.sin_port), msg.text);
	}


	return 0;

}

