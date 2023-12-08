#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/sem.h>
#include <sys/types.h>
//Wartosci sa na sztywno, nie zaleza od systemu

#define MSG_SIZE 60
#define POSTS_COUNT 10

struct post{
    int likes;
    char content[MSG_SIZE];
    char username[MSG_SIZE];
    int isSet;
};

struct database{
    int n;
    int curr_server;
    struct post posts[];
};

union semun {
    int val;
    struct semid_ds *buf;
    struct post p;
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


// do odblokowania od zablokowania powinienem sprawdzac czy podaje poprawny index?????
void lock_sem(int sem, int semid, int msg_count){
    //Zablokowanie semaforow (sem=-1 kazdy, 0 <= sem < POST_COUNT okreslony)
    if(sem == -1){
        struct sembuf operacje_blokujace[msg_count];

        for(int i=0; i<msg_count; i++){
            operacje_blokujace[i].sem_num = i;
            operacje_blokujace[i].sem_op = -1; // Blokowanie semafora
            operacje_blokujace[i].sem_flg = SEM_UNDO;
        }
        if (semop(semid, operacje_blokujace, 1) == -1) {
            perror("Blad semop (blokowanie)");
            exit(EXIT_FAILURE);
        }
    }
    else{
        struct sembuf sb;

        sb.sem_num = sem;
        sb.sem_op = -1;
        sb.sem_flg = 0;
        if (semop(semid, &sb, 1) == -1) {
            perror("semop");
            exit(1);
        }
    }

}

void unlock_sem(int sem, int semid, int msg_count){
    //Odblokowanie semaforow (sem=-1 kazdy, 0 <= sem < POST_COUNT okreslony)
    if(sem == -1){
        struct sembuf operacje_odblokowujace[msg_count];

        for (int i = 0; i < msg_count; ++i) {
            operacje_odblokowujace[i].sem_num = i;
            operacje_odblokowujace[i].sem_op = 1; // Odblokowanie semafora
            operacje_odblokowujace[i].sem_flg = SEM_UNDO;
        }

        if (semop(semid, operacje_odblokowujace, 1) == -1) {
            perror("Blad semop (odblokowanie)");
            exit(EXIT_FAILURE);
        }
    }
    else{
        struct sembuf sb;

        sb.sem_num = sem;
        sb.sem_op = 1;
        sb.sem_flg = 0;
        if (semop(semid, &sb, 1) == -1) {
            perror("semop");
            exit(1);
        }
    }
}
