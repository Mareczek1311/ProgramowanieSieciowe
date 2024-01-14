#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/un.h>
//DELETE COMMS: "temp"
       #include <signal.h>
#include <sys/wait.h>
//TO DO
//
///
///no i dla wersji "A" asynchronicznosc
///

#define BUFLEN 1024

int connected = 0;
int got_move = 0;
int score = 0;
int first_move = 1;

int my_score = 0;
int enemy_score = 0;

int is_started = 0;

int is_host;

int games = 0;

struct sockaddr_in other_addr;
size_t addr_len = sizeof(struct sockaddr_storage);

char enemy_nickname[BUFLEN];
char nickname_ip[INET_ADDRSTRLEN];
char enemy_address[INET_ADDRSTRLEN];

int num_of_childs;
int currFlag = 0;	

void print_global(){
    printf("==============\n");
    printf("connected: %d \n", connected);
    printf("got_move: %d \n", got_move);
    printf("score: %d \n", score);
    printf("my_score: %d \n", my_score);
    printf("enemy_score: %d \n", enemy_score);
    printf("enemy_nickname: %s", enemy_nickname);
    printf("==============\n");
}


void conn(int sockfd){
    int host = 0;
    struct in_addr naddr;

    char ip[INET_ADDRSTRLEN];

    char data[BUFLEN];
    int numbytes;


    if((numbytes = recvfrom(sockfd, data, BUFLEN, 0, (struct sockaddr *) &other_addr, (socklen_t *)&addr_len)) == -1){
        perror("recvfrom");
        exit(1);
    }

    if(!strcmp(data, "start")){
        if(sendto(sockfd, "klient", sizeof("klient"), 0, (struct sockaddr*) &other_addr, addr_len) == -1){
            perror("sendto");
            exit(1);
        }
                
        
        inet_ntop(AF_INET, &(other_addr.sin_addr), ip, INET_ADDRSTRLEN );
        memcpy(enemy_address, ip, INET_ADDRSTRLEN);

        printf("%s, dolaczyl do gry.\n", ip);
        host = 1;
    }

    if(!strcmp(data, "klient")){
        inet_ntop(AF_INET, &(other_addr.sin_addr), ip, INET_ADDRSTRLEN );
        memcpy(enemy_address, ip, INET_ADDRSTRLEN);
        
        host = 0;
    }
    
    is_host = host;

    connected  = 1;

}

void send_option(int sockfd, int option){

    char data[BUFLEN];
    int numbytes;

    sprintf(data, "%d", option);   
    numbytes = strlen(data);
    
    data[numbytes] = '\0';

    
    if(sendto(sockfd, data, numbytes, 0, (struct sockaddr*) &other_addr, addr_len) == -1){
        perror("write");
        exit(1);
    }
    
}

void send_points(int sockfd){
    
    char data[BUFLEN];
    int numbytes;

    numbytes = snprintf(data, BUFLEN, "%d", score);

    if(numbytes < 0 || numbytes >= BUFLEN){
        fprintf(stderr, "snprintf");
        exit(1);
    }

    data[numbytes] = '\0';


    if(sendto(sockfd, data, numbytes, 0, (struct sockaddr*) &other_addr, addr_len) == -1){
        perror("write");
        exit(1);
    }

}


void send_start(int sockfd, int whos_starting){
    
    char data[BUFLEN];
    int numbytes;

    numbytes = snprintf(data, BUFLEN, "%d", whos_starting);

    if(sendto(sockfd, data, numbytes, 0, (struct sockaddr*) &other_addr, addr_len) == -1){
        perror("write");
        exit(1);
    }

    if(whos_starting == is_host){
        got_move = 1;
    }
    else{
        got_move = 0;
    }

}

void send_nickname(int sockfd, char* nickname){
    char data[BUFLEN];
    int numbytes;
    
    numbytes = strlen(nickname);

    memcpy(data, nickname, numbytes);

    data[numbytes] = '\0';

    if(sendto(sockfd, data, numbytes, 0, (struct sockaddr*) &other_addr, addr_len) == -1){
        perror("write");
        exit(1);
    }

}

void receive_nickname(int sockfd){
    char data[BUFLEN];
    int numbytes;


    if((numbytes = recvfrom(sockfd, data, BUFLEN, 0, (struct sockaddr *) &other_addr, (socklen_t *)&addr_len)) == -1){
        perror("read");
        exit(1);
    }

    memcpy(enemy_nickname, data, numbytes);
    
    enemy_nickname[numbytes] = '\0';
    
}

void receive_start(int sockfd){
    
    char data[BUFLEN];
    int numbytes;

    if((numbytes = recvfrom(sockfd, data, BUFLEN, 0, (struct sockaddr *) &other_addr, (socklen_t *)&addr_len)) == -1){
        perror("read");
        exit(1);
    }
    
    data[numbytes] = '\0';

    first_move = atoi(data);

    if(first_move == is_host){
        got_move = 1;
    }
    else{
        got_move = 0;
    }

}

void receive_points(int sockfd){
 
    char data[BUFLEN];
    int numbytes;

    if((numbytes = recvfrom(sockfd, data, BUFLEN, 0, (struct sockaddr *) &other_addr, (socklen_t *)&addr_len)) == -1){
        perror("read");
        exit(1);
    }
    
    data[numbytes] = '\0';

    score = atoi(data);

}   

int receive_option(int sockfd){
    
    char data[BUFLEN];
    int numbytes;

    if((numbytes = recvfrom(sockfd, data, BUFLEN, 0, (struct sockaddr *) &other_addr, (socklen_t *)&addr_len)) == -1){
        perror("read");
        exit(1);
    }
    
    data[numbytes] = '\0';

    return atoi(data);
}

int make_move(int sockfd){
    
	char option[BUFLEN];
	int num;

	printf(">");
	scanf("%s", option);

	if(sscanf(option, "%d", &num) != 1){

		if(!strcmp(option, "wynik")){
			printf("Ty %d : %d %s \n", my_score, enemy_score, enemy_nickname);
		}
		else if(!strcmp(option, "koniec")){
			return 4;
		}
		else{
			printf("Brak takiej opcji \n");
		}
	}
	else{
		if((num - score) > 10 || (num - score) <= 0 || num > 50){
			printf("Takiej wartosci nie mozesz wybrac! \n");
		}
		else{
			score = num;
		}
	}


    if(score == 50){
        return 2;
    }

    return 1;

}

void restart_game(){
    score = 0;
    first_move = !first_move;
    is_started = 0;

    if(first_move != is_host){
        printf("Zaczynamy kolejna rozgrywke, poczekaj na swoja kolej.\n");
    }
    else{
        printf("Zaczynamy kolejna rozgrywke.\n");
    }
}

int pid1 = 1, pid2 = 1;

int is_ended = 0;

void v2(int sockfd){

    char data[BUFLEN];
    int numbytes;
    int curr_score;
    int opt;
 	num_of_childs = 0;

   	while(1){
		if(num_of_childs < 2){
			if(currFlag == 0 && pid1 != 0 && pid2 != 0){
				currFlag = 1;
				pid1 = fork();	
				num_of_childs++;
				if(pid1 == 0){
					//receive optioni
					

					opt = receive_option(sockfd);

					if(opt == 1){
						receive_points(sockfd);

						got_move = 1;
						printf("%s podal wartoscc %d, podaj kolejna wartosc.\n", enemy_nickname, score);
					}
					else if(opt == 2){
						enemy_score += 1;  
						printf("Przegrana!\n");
						restart_game();
					}
					else if(opt == 4){
						printf("%s zakonczyl gre, mozesz poczekac na kolejnego gracza\n", enemy_address);

						score = 0;
						first_move = 1;
						is_started = 0;
						connected = 0;
						is_host = 1;
					}

					kill(getppid(), 1);
					exit(0);
				}
			}
			if(currFlag == 1 && pid1 != 0 && pid2 != 0){
				currFlag = 0;
				pid2 = fork();
				num_of_childs++;
				if(pid2 == 0){
					//options selection
						if(got_move == 1 && is_started == 0 && is_host == first_move){

							score = (rand() % 10) + 1;

							printf("Losowa wartosc poczatkowa: %d, podaj kolejna wartosc.\n", score); 

							is_started = 1;
						}

					while(1){

						opt = make_move(sockfd);
						send_option(sockfd, opt);


						if(opt == 1 && got_move == 0){
							printf("Teraz tura gracza %s, poczekaj na swoja kolej.\n", enemy_nickname);
						}
						else if(opt == 1){
							send_points(sockfd);
							got_move = 0;

							break;
						}
						else if(opt == 2){
							//send_win(sockfd);
							my_score += 1;
							printf("Wygrana!\n");
							restart_game();
							break;
						}
						else if(opt == 4){
							exit(0);
							//jakis exit=1??????
							is_ended=1;
						}

					}
					kill(getppid(), 2);
					exit(0);	
				}
			
			}
		}
  		if(is_ended == 1){
			close(sockfd);
			exit(0);	
		}
	} 

}


void get_request_and_respond(int sockfd){
    char data[BUFLEN];
    int numbytes;
    int curr_score;


    int opt;
    
    if(got_move == 1){


    }
    else{

    }        

}

int setup_server(char *address, char *port){
    
    int host = 0;

    int sockfd; // deskryptor socket
    struct in_addr naddr;

    struct sockaddr_in me_addr;

    int numbytes;
    char data[BUFLEN];

    bzero(&me_addr, sizeof(struct sockaddr_in));
    me_addr.sin_family = AF_INET;
    me_addr.sin_port = htons(atoi(port));
    me_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (inet_pton(AF_INET, address, &naddr) != 1) {
        struct hostent *host = gethostbyname(address);
        if (host == NULL) {
            fprintf(stderr, "Nie można rozwiązać nazwy domeny.\n");
            exit(1);
        }
        naddr = *(struct in_addr *) host->h_addr;
	inet_ntop(AF_INET, &host->h_addr, enemy_address, INET_ADDRSTRLEN);
    }


    bzero(&other_addr, sizeof(struct sockaddr_in));
    other_addr.sin_family = AF_INET;
    other_addr.sin_port = htons(atoi(port));
    other_addr.sin_addr = naddr;

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("socket");
        exit(1);
    }

    int yes;
    int sockopt = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (sockopt == -1){
        perror("sockopt");
        exit(1);
    }


    if(bind(sockfd, (struct sockaddr*)&me_addr, sizeof(me_addr)) == -1){
        perror("bind");
        exit(1);
    }

    

    if ((numbytes = sendto(sockfd, "start", 6, 0,
                             (struct sockaddr *)&other_addr,
                             sizeof(other_addr))) == -1){
        perror("sendto");
        exit(1);
        
    }
    printf("Propozycja gry wyslana \n"); 
    
    return sockfd;
    

 
}

void signal_handler(int signal){
	int status;
	wait(&status);
	if(status == 1){
		currFlag = 1;		
		kill(pid2, 9);
	}

}

int main(int argc, char* argv[]){
   
    if(argc != 3 && argc != 4){
        fprintf(stderr, "usage: ip port\n");
        exit(1);
    } 
  
    srand(time(NULL));
    char opcja[BUFLEN];
    
    int sockfd;

    printf("Gra w 50, wersja B \n");
    
    sockfd = setup_server(argv[1], argv[2]);
    printf("Rozpoczynam gre z %s. Napisz ""koniec"" by zakonczyc lub ""wynik"" by wyswietlic aktualny wynik \n", enemy_address); 


    signal(1, signal_handler);
    signal(2, signal_handler);

    while(1){
        
        if(connected == 0){
            conn(sockfd);
            
            if(is_host){
                if(argc == 4){
                    send_nickname(sockfd, argv[3]);
                }
                else{
                    send_nickname(sockfd, nickname_ip);
                }
                receive_nickname(sockfd);
            }else{
                receive_nickname(sockfd);
                if(argc == 4){
                    send_nickname(sockfd, argv[3]);
                }
                else{
                    send_nickname(sockfd, nickname_ip);
                }
            }
        }

        if(is_started == 0 && is_host == 1){
            send_option(sockfd, 3);
            send_start(sockfd, first_move);
            
            if(is_host != first_move){
                is_started = 1;
            }
        }
        else if(is_started == 0 && is_host == 0){
            receive_option(sockfd);
            receive_start(sockfd);
            if(is_host != first_move){
                is_started = 1;
            }
        }

        //get_request_and_respond(sockfd);
	v2(sockfd);        
    }


    close(sockfd);

    return 0;
}
