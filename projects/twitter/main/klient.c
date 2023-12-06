#include "data.h"

key_t key;
    
int shmid;
struct database* db;
    
int semid;
struct sembuf sb;

//argv[1] - sciezka_do_pliku
//argv[2] - username

int main(int argc, char* argv[]){

    char option;
    char msg[MSG_SIZE];
    char username[MSG_SIZE];
    int post;

   if(argc != 3){
        printf("--- WRONG NUMBER OF ARGUMENTS ---\n");
        printf("--- ./k FILEPATH USERNAME ---\n");
        exit(1);
   }

    strncpy(username, argv[2], MSG_SIZE);

    if((key = ftok(argv[1], 'A')) == -1){
        perror("ftok");
        exit(1);
    }

    // --- TWORZENIE SEMAFOROW ---
    if((semid = semget(key, db->n, 0666 | IPC_EXCL)) == -1){
        perror("semget");
        exit(1);
    }

    /*
    SCHEMAT DZIALANIA:
    - zablokowanie wszystkich postow
    - wypisanie ich
    - odblokowanie

    OPCJE:
        DODAWANIE POSTA
        - zablokowanie semafora do ktorego bede dodawac nowy post
        - odblokowanie go po dodaniu

        LIKE
        - zablokowanie semafora do ktorego bede dawac like
        - odblokowanie go

    */

    lock_sem(-1, semid, db->n);
    
    printf("Loading...\n");

    // --- TWORZENIE PAMIECI WSPOLDZIELONEJ ---
    if((shmid = shmget(key, SHM_SIZE, IPC_EXCL)) == -1){
        perror("shmget");
        exit(1);
    }

    if((db = (struct database*) shmat(shmid, (void*)0, 0)) == (void *) -1){
        perror("shmat");
        exit(1);
    }

    printf("Twitter 2.0 wita! (wersja C)\n");
    printf("Wolnych %d wpisow (na %d)", db->n-db->curr_server, db->n);

    if(!isEmpty(db)){
        print_posts(db, 1);
    }

    unlock_sem(-1, semid, db->n);

    printf("Podaj akcje (N)owy wpis, (L)ike \n");

    scanf("%c", &option);

    switch(option){
        case 'N':
            //Brakuje sprawdzenia czy jest miejsce na nowy wpis
            if(db->curr_server == db->n){
                printf("Brak miejsca na nowy wpis\n");
                break;
            }

            printf("Napisz co chodzi ci po glowie: \n");
            printf("> ");

            char c;
            while ((c = getchar()) != '\n' && c != EOF);
            fgets(msg, sizeof(msg), stdin);
            msg[strcspn(msg, "\n")] = '\0';
            
            lock_sem(db->curr_server, semid, db->n);

            strncpy(db->posts[db->curr_server]->content, msg, MSG_SIZE);
            strncpy(db->posts[db->curr_server]->username, username, MSG_SIZE);
            db->posts[db->curr_server]->likes = 0;
            db->posts[db->curr_server]->isSet = 1;

            sleep(10);

            unlock_sem(db->curr_server, semid, db->n);

            db->curr_server++;
            //Nowy wpis
            break;
        case 'L':
            //Brakuje sprawdzenia czy indeks jest poprawny  

            //Like
            printf("Ktory wpis chcesz polubic: \n");
            printf("> ");
            scanf("%d", &post);
            post-=1;
            
            if(post > db->curr_server && post < 0){
                printf("Podany wpis nie istnieje\n");
                break;
            }

            lock_sem(post, semid, db->n);

            if(post<db->curr_server){
                db->posts[post]->likes++;
            }
            
            unlock_sem(post, semid, db->n);

            break;
        default:
            printf("Podana opcja nie istnieje\n");
    }

    if(shmdt(db) == -1){
        perror("shmdt");
        exit(1);
    }

    printf("Dziekuje za skorzystanie z aplikacji Twitter 2.0\n");

    return 0;
}