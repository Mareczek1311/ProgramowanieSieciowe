#include<stdio.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

/* A. Mroz - zad. na SK, do modyfikacji */
/* brak pelnej obslugi bledow! */

int main(int argc, char * argv[]) {
	
	struct hostent *host;
	char   **bufs;
	struct in_addr *addr;
	struct in_addr arg_addr;

	//host = gethostbyname(argv[1]);
	inet_aton(argv[1], &arg_addr);

	host = gethostbyaddr(&arg_addr, sizeof arg_addr, AF_INET);
	
	if(host == NULL) {
		herror("Blad gethostbyname");
		return -1;
	}

	printf("Pola struktury hostent:\n");
	printf("h_name: %s\n", host->h_name);
	printf("h_aliases: ");
	for(bufs = host->h_aliases; *bufs != NULL; ++bufs)
		printf("%s; ", *bufs);
	
	printf("\nh_addrtype: %d(%s)\n", host->h_addrtype, 
			(host->h_addrtype == AF_INET)?"AF_INET":"nieznany typ adresu");
	
	printf("h_length: %d\n", host->h_length);
	printf("h_addr_list: ");
	for(bufs = host->h_addr_list; *bufs != NULL; ++bufs) {
		addr = (struct in_addr *) *bufs;
		printf("%s; ", inet_ntoa(*addr));
	}

	printf("\n");
	return 0;

}


