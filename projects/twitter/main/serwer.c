#include "data.h"

//PAMIEC WSPOLDZIELONA
struct database* db;
int shmid;
int shmid2;

union semun arg;
int semid;
int global_posts_count;

void signal_handler(int signal){
    switch(signal){
        case SIGTSTP:
            print_posts(db, 0);
            break;

        case SIGINT:
            printf("[Serwer]: Dostalem SIGINT -> koncze i sprzatam... ");

            //ODLOCZENIE OD PAMIECI WSPOLDZIELONEJ ODPOWIEDZIALNEJ ZA PRZECHOWYWANIE POSTOW
            if(shmdt(db) == -1){
                perror("shmdt");
                exit(1);
            }
            printf("(Odlaczenie shm: OK");

            //USUWANIE PAMIECI WSPOLDZIELONEJ ODPOWIEDZIALNEJ ZA PRZECHOWYWANIE POSTOW
            if(shmctl(shmid, IPC_RMID, 0) == -1){
                perror("shmctl remove");
                exit(1);
            }
            printf(", usuniecie shm: OK");

            //USUWANIE N SEMAFOROW
            if (semctl(semid, global_posts_count, IPC_RMID, arg) == -1) {
                perror("semctl");
                exit(1);
            }
            printf(", usuniecie sem: OK) \n");

            //USUWANIE DRUGIEGO SEGMENTU PAMIECI WSPOLDZIELONEJ ODPOWIEDZIALNEGO ZA ILOSC POSTOW
            if(shmctl(shmid2, IPC_RMID, 0) == -1){
                perror("shmctl remove");
                exit(1);
            }

            exit(0);
            //zakonczenie poprzedzone posprzataniem (semy, shm, shm2)
        default:
            printf("[Serwer]: Sygnal nie obsluzony \n");
    }
}

//INICJALIZACJA POSTA
void initializePost(struct post *p) {
    p->likes = 0;
    p->isSet = 0;
    memset(p->content, 0, MSG_SIZE);
    memset(p->username, 0, MSG_SIZE);
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

    int key;
    int key2;

    int posts_count = atoi(argv[2]);
    global_posts_count = posts_count;
    size_t rozmiar = sizeof(struct database) + posts_count * sizeof(struct post);
    int* count_ptr = &posts_count;

    printf("[Serwer]: Twitter 2.0 (wersja A)\n");

    printf("[Serwer]: tworze klucz na podstawie pliku data.h... ");
    if((key = ftok(argv[1], 'A')) == -1){
        perror("ftok");
        exit(1);
    }
    printf("OK(klucz: %d)\n", key);

    //TWORZE DRUGI SEGMENT PAMIECI WSPOLDZIELONEJ ODPOWIEDZIALNY ZA ROZMIAR
    if((key2 = ftok(argv[1], 'B')) == -1){
        perror("ftok");
        exit(1);
    }

    if((shmid2 = shmget(key2, sizeof(int), 0644 | IPC_CREAT)) == -1){
        perror("shmget");
        exit(1);
    }

    if((count_ptr = (int*) shmat(shmid2, (void*)0, 0)) == (void *) -1){
        perror("shmat");
        exit(1);
    }

    if(shmdt(count_ptr) == -1){
        perror("shmdt");
        exit(1);
    }

    printf("[Serwer]: Tworzę segment pamięci wspoldzielonej na %d wpisow po %ldb...  \n", posts_count, sizeof(struct post));
    if((shmid = shmget(key, rozmiar, 0644 | IPC_CREAT)) == -1){
        perror("shmget");
        exit(1);
    }
    printf("OK (id: %d, rozmiar: %ldb) \n", shmid, rozmiar);

    printf("[Serwer]: Dolaczam pamiec wspolna... ");

    if((db = (struct database*) shmat(shmid, (void*)0, 0)) == (void *) -1){
        perror("shmat");
        exit(1);
    }

    db->n = posts_count;
    db->curr_server = 0;
    for (int i = 0; i < posts_count; ++i) {
        initializePost(&db->posts[i]);
    }

    printf("OK (adres: %p) \n", (void*)db);

    // ---TWORZENIE SEMAFOROW---
    printf("[Serwer]: Tworzę %d semaforow...  ", posts_count);
    if ((semid = semget(key, posts_count, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }
    printf("OK (id: %d)\n", semid);


    printf("[Serwer]: Inicjalizacja semaforow... ");
    union semun arg;
    arg.val = 1;

    for(int i=0; i<posts_count;i++){
        if(semctl(semid, i, SETVAL, arg) == -1){
            perror("semctl");
            exit(1);
        }
    }

    printf("OK \n");

    printf("[Serwer]: nacisnij Crtl^Z by wyswietlic stan serwisu\n");
    printf("[Serwer]: nacisnij Crtl^C by zakonczyc program\n");

    while(1){
        sleep(1);
    }

    return 0;
}
