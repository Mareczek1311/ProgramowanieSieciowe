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
            lock_sem(-1, semid, POSTS_COUNT);

            print_posts(db, 0);

            unlock_sem(-1, semid, POSTS_COUNT);
            break;

        case SIGINT:
            printf("[Serwer]: Dostalem SIGINT -> koncze i sprzatam... ");
            if(shmdt(db) == -1){
                perror("shmdt");
                exit(1);
            }
            printf("(Odlaczenie shm: OK \n");

            if(shmctl(shmid, IPC_RMID, 0) == -1){
                perror("shmctl remove"); 
                exit(1);
            }
            printf(", usuniecie shm: OK \n");
            
            if (semctl(semid, POSTS_COUNT, IPC_RMID, arg) == -1) {
                perror("semctl");
                exit(1);
            }
            printf(", usuniecie sem: OK) \n");

            exit(0);
            //zakonczenie poprzedzone posprzataniem (sem pojedynczy, shm)
        default:
            printf("[Serwer]: Sygnal nie obsluzony \n");
    }
}


// argv[1] - nazwa_pliku
// argv[2] - maksymalna_liczba_wiadomosci
int main(int argc, char* argv[]){

    if(argc != 3){
        printf("!WRONG ARGUMENTS!\n");
        printf("./s.c file_name msg_count \n");
        exit(1);
    }

    signal(SIGTSTP, signal_handler);
    signal(SIGINT, signal_handler);

    key_t key;
    
    int rozmiar; // TRZEBA UZUPELNIC

    printf("[Serwer]: Twitter 2.0 (wersja C)\n");    

    printf("[Serwer]: tworze klucz na podstawie pliku data.h... ");

    if((key = ftok(argv[1], 'A')) == -1){
        perror("ftok");
        exit(1);
    }
    printf("OK(klucz: %d)\n", key);

    printf("[Serwer]: Tworzę segment pamięci wspoldzielonej na 10 wpisow po 128b...  \n");
    if((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1){
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


    for(int i=0; i<POSTS_COUNT; i++){
        db->posts[i].isSet = 0;
    }

    printf("OK (adres: %p) \n", (void*)db);

    // ---TWORZENIE SEMAFORA--- 
    printf("[Serwer]: Tworzę %d semaforow...  \n", POSTS_COUNT);

    if ((semid = semget(key, POSTS_COUNT, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }
    printf("OK (id: %d)\n", semid);

    printf("[Serwer]: Inicjalizuję semafory... ");
    
    for (int i = 0; i < POSTS_COUNT; i++) {
        if (semctl(semid, i, SETVAL, 1) == -1) {
            perror("semctl");
            exit(1);
        }
    }
    printf("OK \n");

    printf("[Serwer]: nacisnij Crtl^Z by wyswietlic stan serwisu\n");
    printf("[Serwer]: nacisnij Crtl^C by zakonczyc program\n");

    /*
    printf("Segment zawiera: \n");
    printf("Username: %s \n", p->username);
    printf("Content: %s \n", p->content);
    printf("Likes: %d \n", p->likes);
    */

   //A JAK SERWER BEDZIE DZIALAC Z SEMAFOROW!!!!!!!!

    while(1){
        sleep(1);
    }

    return 0;
}