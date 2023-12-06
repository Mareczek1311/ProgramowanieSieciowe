#include "data.h"

//PAMIEC WSPOLDZIELONA
struct database* db;
int shmid;

//POJEDYNCZY SEMAFOR
union semun arg;
int semid;

void signal_handler(int signal){
    switch(signal){
        case SIGTSTP:
            print_posts(db, 0);
            break;

        case SIGINT:
            printf("[Serwer]: Dostalem SIGINT -> koncze i sprzatam... ");
            if(shmdt(db) == -1){
                perror("shmdt");
                exit(1);
            }
            printf("(Odlaczenie: OK ");

            if(shmctl(shmid, IPC_RMID, 0) == -1){
                perror("shmctl remove");
                exit(1);
            }
            printf(", usuniecie: OK)\n");
            exit(1);
            //zakonczenie poprzedzone posprzataniem
        default:
            printf("[Serwer]: Sygnal nie obsluzony \n");
    }
}

int main(int argc, char* argv[]){

/*
    if(argc != 3){
        printf("!WRONG ARGUMENTS!\n");
        printf("./s.c file_name n \n");
        exit(1);
    }
*/

    signal(SIGTSTP, signal_handler);
    signal(SIGINT, signal_handler);

    key_t key;
    printf("[Serwer]: Twitter 2.0 (wersja C)\n");    

    printf("[Serwer]: tworze klucz na podstawie pliku data.h... ");
    //trzeba zastąpić prawidlowym plikiem podanym od admina
    if((key = ftok("data.h", 'A')) == -1){
        perror("ftok");
        exit(1);
    }
    printf("OK(klucz: %d)\n", key);

    printf("[Serwer]: Tworzę segment pamięci wspoldzielonej na 10 wpisow po 128b...  \n");
    if((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT | IPC_EXCL)) == -1){
        perror("shmget");
        exit(1);
    }
    printf("OK (id: %d, rozmiar: %db) \n", shmid, SHM_SIZE);

    printf("[Serwer]: Dolaczam pamiec wspolna... ");
    if((db = (struct database*) shmat(shmid, (void*)0, 0)) == (void *) -1){
        perror("shmat");
        exit(1);
    }
    db->n = 9;
    db->curr_server = 0;

    for(int i=0; i<db->n; i++){
        db->posts[i].isSet = 0;
    }

    printf("OK (adres: %p) \n", (void*)db);

    // ---TWORZENIE SEMAFORA--- 
    printf("[Serwer]: Tworzę pojedynczy semafor dla calego serwisu...  \n");

    if ((semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL)) == -1) {
        perror("semget");
        exit(1);
    }
    printf("OK\n");

    printf("[Serwer]: Inicjalizuję semafor... ");
    if(semctl(semid, 0, SETVAL, arg) == -1){

    }


    printf("[Serwer]: nacisnij Crtl^Z by wyswietlic stan serwisu\n");
    printf("[Serwer]: nacisnij Crtl^C by zakonczyc program\n");

    /*
    printf("Segment zawiera: \n");
    printf("Username: %s \n", p->username);
    printf("Content: %s \n", p->content);
    printf("Likes: %d \n", p->likes);
    */

    while(1){
        sleep(1);
    }

    return 0;
}