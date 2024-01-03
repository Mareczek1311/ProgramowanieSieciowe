#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 1024

int setup_server(char *address, char *port, int *who){
    
    int sockfd;
    struct sockaddr_in addr;

    struct sockaddr_storage their_addr;
    size_t addr_len = sizeof(struct sockaddr_storage);
    
    char data[BUFLEN];
    int numbytes;
    int num;

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(atoi(port));

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("socket");
        exit(1);
    }

    if(bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) == 0){
        printf("Port dostepny\n");
        if ((numbytes = recvfrom(sockfd, data, BUFLEN - 1, 0,
                        (struct sockaddr *)&their_addr,
                        (socklen_t *)&addr_len)) == -1){
            perror("rcv");
            exit(1);
        }

        if((connect(sockfd, (struct sockaddr*) &their_addr, addr_len)) < 0){
            perror("connect");
            exit(1);
        }
        *who = 0;
        printf("Nawiazalem polaczenie \n");
    }
    else{
        printf("port zajety \n");
        if ((numbytes = sendto(sockfd, "", 0, 0,
                        (struct sockaddr *)&addr,
                        addr_len)) == -1){
            perror("sendto");
            exit(1);
        }

        
        if((connect(sockfd, (struct sockaddr*) &addr, addr_len)) < 0){
            perror("connect");
            exit(1);
        }

        *who = 1;
    }

    return sockfd;
}


int main(int argc, char* argv[]){
   
    if(argc != 3){
        fprintf(stderr, "usage: ip port\n");
        exit(1);
    } 
    
    char opcja[BUFLEN];
    
    int sockfd;
    int who;

    
    int my_score = 0;
    int enemy_score = 0;
    char data[BUFLEN];
    int numbytes;
    int num;
    int score = 0;
    int new_score = 0;

    sockfd = setup_server(argv[1], argv[2], &who);
    
    memcpy(opcja, "", 0);
    
    printf("WHO: %d\n", who);

    while(strcmp(opcja, "koniec")){
        
        if(who == 0){
            printf("Podaj opcje: \n");
            scanf("%s", opcja);


            //Nie robie tego w while wiec problem bedzie gdy opcja bedzie niepoprawna
            //to do:
            //while na podawanie liczby oraz opcji 
            if(sscanf(opcja, "%d", &num) == 1){
                //liczba
                
                //to do:
                //sprawdzenie czy z dobrego przedzialu
                score = score + num;
                numbytes = snprintf(data, BUFLEN, "%d", score);

                if(numbytes < 0 || numbytes >= BUFLEN){
                    fprintf(stderr, "snprintf");
                    exit(1);
                }
                
                data[numbytes] = '\0';
                
                //dane jakie wysylam
                printf("data: %s\n", data);
                printf("numbyttes: %d\n", numbytes);
                printf("num: %d\n", score);

                if(write(sockfd, data, numbytes) < 0){
                    perror("write");
                    exit(1);
                }

                who = 1;

            }
            else{
                //string
                //do zastapienia nickiem enemy
                if(!strcmp(opcja, "wynik")){    
                    printf("Ty %d : %d Enemy\n", my_score, enemy_score);
                }
                else if(!strcmp(opcja, "koniec")){
                    //chyba wyslemy -1 jako liczbe, ze konczymy gre
                }
                else{
                    printf("Nie ma takiej opcji!\n");
                }
            }
        }

        else{
            printf("oczekiwanie...\n");
            if((numbytes = read(sockfd, data, BUFLEN)) < 0){
                perror("read");
                exit(1);
            }
            
            data[numbytes] = '\0';

            score = atoi(data);

            printf("score: %d\n", score);
            printf("wynik: %s \n", data);

            who = 0;
        }
    }

    
    printf("---KONIEC---\n");
    close(sockfd);

    return 0;
}
