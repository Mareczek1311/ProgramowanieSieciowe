#include<stdio.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>


/* Na SK - do modyfikacji
   Brak pelnej obslugi bledow! */

int main(int argc, char * argv[]) {

	struct addrinfo *res;
	struct sockaddr_in *sain;
	int co;
	struct addrinfo hints;

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = 0;
	hints.ai_protocol = 0;
	hints.ai_flags = AI_CANONNAME;

	//co = getaddrinfo(argv[1], NULL, NULL, &res);
	co = getaddrinfo(argv[1], NULL, &hints, &res);
	if (co != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(co));
		return -1;
	}
	
	int num = 0;
	for(; res; res=res->ai_next)
	{
		num++;
		printf("\nPola (%d na liscie) struktury addrinfo:\n", num);

		printf("ai_flags: %d\n", res->ai_flags);

		printf("ai_family: %d (", res->ai_family);
		if(res->ai_family == AF_INET) printf("AF_INET)\n");
		else if(res->ai_family == AF_INET6) printf("AF_INET6)\n");
		else printf("nieznany typ adresu)\n");

		printf("ai_socktype: %d\n", res->ai_socktype);

		printf("SOCK TYPE: %d ", res->ai_socktype);
		switch(res->ai_socktype){
			case SOCK_DGRAM:
				printf("SOCKET DGRAM\n");
				break;
			case SOCK_STREAM:
				printf("SOCK_STREAM\n");
				break;
			case SOCK_RAW:
				printf("Sock RAW \n");
				break;
			case SOCK_SEQPACKET:
				printf("SOCK SEQPACKET \n");
				break;
		}

		printf("ai_protocol: %d\n", res->ai_protocol);
		printf("ai_addrlen: %d\n", res->ai_addrlen);

		printf("ai_addr: ");
		if(res->ai_family == AF_INET) {
			sain = (struct sockaddr_in*)(res->ai_addr);
			printf("%s\n", inet_ntoa(sain->sin_addr));
		}

		printf("ai_canonname: %s\n", res->ai_canonname);
	}
	return 0;

}

