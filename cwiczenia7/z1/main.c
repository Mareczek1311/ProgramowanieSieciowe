#include <arpa/inet.h>
#include <unistd.h>
     #include <stdio.h>

#include <stdlib.h>
int main(int argc, char* argv[]){
    uint32_t naddr;
    
    if(inet_pton(AF_INET, argv[1], &naddr) == -1){
        perror("inet");
        exit(1);
    }
    
    printf("%s\n", argv[1]);
    printf("%d\n", naddr);
    
    naddr = htonl(naddr);

    printf("%d\n", naddr);

    return 0;
}
