#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

//Wartosci sa na sztywno, nie zaleza od systemu

#define SHM_SIZE 1248
#define MSG_SIZE 60
#define POSTS_COUNT 9

struct post{
    int likes;
    char content[MSG_SIZE];
    char username[MSG_SIZE];
    int isSet;
};

struct database{
    int n;
    int curr_server;
    struct post posts[POSTS_COUNT];
};

union semun {
    int val;
    struct semid_ds *buf;
    struct *database db;
};

int isEmpty(struct database* db){
    if(db->curr_server == 0 && db->posts[0].isSet == 0){
        return 1;
    }
    return 0;
}
//1 klient 0 serwer
void print_posts(struct database* db, int mode){
    //aktualna zawartosc
    if(isEmpty(db) && mode == 0){
        printf("Brak wpisow\n");
    }
    else{
        if(mode == 0)
        {
            printf("___________  Twitter 2.0:  ___________\n");
            for(int i=0; i<=db->curr_server; i++){
                if(db->posts[i].isSet){
                    printf("[%s]: %s [Polubienia: %d]\n", db->posts[i].username, db->posts[i].content, db->posts[i].likes);
                }
            }
        }
        else{
            printf("Istniejace wpisy: \n");
            for(int i=0; i<=db->curr_server; i++){
                if(db->posts[i].isSet){
                    printf("    %d. %s [Autor: %s, Polubienia: %d] \n", i+1, db->posts[i].content, db->posts[i].username, db->posts[i].likes);
                }
            }
        }
    }
}