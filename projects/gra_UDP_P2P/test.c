#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

//DELETE COMMS: "temp"


//TO DO
//
///Sprawdzenie czy policzylismy do 50
///obsluga komendy "koniec"
///
///no i dla wersji "A" asynchronicznosc
///

#define BUFLEN 1024

int connected = 0;
int got_move = 0;
int score = 0;

int my_score = 0;
int enemy_score = 0;

void print_global(){
    printf("==============\n");
    printf("connected: %d \n", connected);
    printf("got_move: %d \n", got_move);
    printf("score: %d \n", score);
    printf("my_score: %d \n", my_score);
    printf("enemy_score: %d \n", enemy_score);
    printf("==============\n");
}

void send_to_player(int sockfd){
    
    char data[BUFLEN];
    int numbytes;

    numbytes = snprintf(data, BUFLEN, "%d", score);

    if(numbytes < 0 || numbytes >= BUFLEN){
        fprintf(stderr, "snprintf");
        exit(1);
    }

    data[numbytes] = '\0';


    if(write(sockfd, data, numbytes) < 0){
        perror("write");
        exit(1);
    }

}

void receive_from_player(int sockfd){
 
    char data[BUFLEN];
    int numbytes;

    if((numbytes = read(sockfd, data, BUFLEN)) < 0){
        perror("read");
        exit(1);
    }
    
    data[numbytes] = '\0';

    score = atoi(data);

}   


void make_move(int sockfd){
    
    char option[BUFLEN];
    int num;

    while(1){
        scanf("%s", option);
        
        if(sscanf(option, "%d", &num) != 1){
            
            if(!strcmp(option, "wynik")){
                printf("Ty %d : %d enemy \n", my_score, enemy_score);
            }
            else if(!strcmp(option, "koniec")){
                //TODO
            }
            else{
                printf("Brak takiej opcji \n");
            }
        }
        else{
            if((num - score) > 10 || (num - score) <= 0){
                printf("Takiej wartosci nie mozesz wybrac! \n");
            }
            else{
                score = num;
                break;
            }
        }

    }


}

void get_request_and_respond(int sockfd, int who){
    char data[BUFLEN];
    int numbytes;
    struct sockaddr_storage their_addr;
    size_t addr_len = sizeof(struct sockaddr_storage);
    int curr_score;
    
    //Start gry
    //PROBLEM !!!!!!!!!!!!!!!!!!!!!!!! --- DZIALA TO TYLKO NA POCZATKU
    if(who == 0 && connected == 0){
        if((numbytes = recvfrom(sockfd, data, BUFLEN - 1, 0, 
                        (struct sockaddr*)&their_addr,
                        (socklen_t *)&addr_len)) == -1){
            perror("recvfrom");
            exit(1);
        }

        ///////////-------------SPRAWDZIC CZY IP SIE ZGADZAJA!!!!!!!
        
        if((connect(sockfd, (struct sockaddr*) &their_addr, addr_len)) < 0){
            perror("connect");
            exit(1);
        }
    
        printf("XXX dolaczyl do gry\n");

        connected = 1;
        
        srand(time(NULL));
        score = (rand() % 10) + 1;
            
        printf("Losowa wartosc poczatkowa: %d, podaj kolejna wartosc.\n", score); 

        make_move(sockfd);
        send_to_player(sockfd);
        got_move = 0;
    }
    else{

        if(got_move == 1){
            
            make_move(sockfd);
            send_to_player(sockfd);
            got_move = 0;
        }
        else{
            receive_from_player(sockfd);
            got_move = 1;
            printf("XXX podal wartoscc %d, podaj kolejna wartosc.\n", score);
        }        
    }

}

int setup_server(char *address, char *port, int *who){
    
    int sockfd;
    struct sockaddr_in addr;
    struct sockaddr_in addr_out;
    struct in_addr naddr;

    char data[BUFLEN];
    int numbytes;
    int num;


    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    //addr.sin_addr = naddr;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(port));

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("socket");
        exit(1);
    }
 
    printf("Rozpoczynam gre z XXX. Napisz ""koniec"" by zakonczyc lub ""wynik"" by wyswietlic aktualny wynik.\n");

    if(bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) == -1){
        perror("bind"); // serwer juz zostal utworzony
    }
    
    *who = 0;

    printf("Propozycja gry wyslana.\n");

   /* 
    */


    if (inet_pton(AF_INET, address, &naddr) < 1){
        perror("inet_pton");
        exit(1);
    }

    bzero(&addr, sizeof(addr));
    addr_out.sin_family = AF_INET;
    addr_out.sin_addr = naddr;
    addr_out.sin_port = htons(atoi(port));

    if(sendto(sockfd, "", 0, 0, (struct sockaddr*)&addr, sizeof(addr)) == -1){
        perror("sendto");
    }
    else{
        
        if((connect(sockfd, (struct sockaddr*) &addr, sizeof(addr))) < 0){
            perror("connect"); //BRAK HOSTA!!!!!!!
            exit(1);
        }

        connected = 1;
        write(sockfd, "0", 1);

        printf("Propozycja gry wyslana.\n");

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

    //0 server || 1 klient
    int who;

    
    sockfd = setup_server(argv[1], argv[2], &who);
    
    
    while(1){
        get_request_and_respond(sockfd, who);        
    }


    close(sockfd);

    return 0;
}
