#include "data.h"

key_t key;
key_t key2;
    
int shmid;
int shmid2;
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
    int post_count;
    int* post_count_ptr;

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


    if((key2 = ftok(argv[1], 'A')) == -1){
        perror("ftok");
        exit(1);
    }

    // --- TWORZENIE PAMIECI WSPOLDZIELONEJ ODPOWIEDZIALNEJ ZA ILSOC POSTOW ---
    if((shmid2 = shmget(key2, sizeof(int), 0666 | IPC_EXCL)) == -1){
        perror("shmget");
        exit(1);
    }

    if((post_count_ptr = (int*) shmat(shmid2, (void*)0, 0)) == (void *) -1){
        perror("shmat");
        exit(1);
    }

    post_count = *post_count_ptr;

    printf("DOSTALEM %d wpisow \n", post_count);


    // --- TWORZENIE SEMAFOROW ---
    if((semid = semget(key, post_count, 0666 | IPC_EXCL)) == -1){
        perror("semget");
        exit(1);
    }

    lock_sem(-1, semid, post_count);
    
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
    printf("Wolnych %d wpisow (na %d)", post_count - db->curr_server, post_count);

    if(!isEmpty(db)){
        print_posts(db, 1);
    }

    unlock_sem(-1, semid, post_count);

    printf("Podaj akcje (N)owy wpis, (L)ike \n");

    scanf("%c", &option);

    switch(option){
        case 'N':

            if(db->curr_server >= post_count){
                printf("Brak miejsca na nowy wpis\n");
                break;
            }

            lock_sem(db->curr_server, semid, post_count);

            printf("Napisz co chodzi ci po glowie: \n");
            printf("> ");

            char c;
            while ((c = getchar()) != '\n' && c != EOF);
            fgets(msg, sizeof(msg), stdin);
            msg[strcspn(msg, "\n")] = '\0';
            

            strncpy(db->posts[db->curr_server].content, msg, MSG_SIZE);
            strncpy(db->posts[db->curr_server].username, username, MSG_SIZE);
            db->posts[db->curr_server].likes = 0;
            db->posts[db->curr_server].isSet = 1;

            unlock_sem(db->curr_server, semid, post_count);

            db->curr_server++;
            break;
        case 'L':

            printf("Ktory wpis chcesz polubic: \n");
            printf("> ");
            scanf("%d", &post);
            post-=1;
            if(post >= db->curr_server || post < 0){
                if(db->posts[post].isSet == 0){
                    printf("Podany wpis nie istnieje\n");
                    break;
                }
            }

            lock_sem(post, semid, post_count);

            if(post<db->curr_server){
                db->posts[post].likes++;
            }
            
            unlock_sem(post, semid, post_count);

            break;
        default:
            printf("Podana opcja nie istnieje\n");
    }

    if(shmdt(db) == -1){
        perror("shmdt");
        exit(1);
    }

    if(shmdt(post_count_ptr) == -1){
        perror("shmdt");
        exit(1);
    }

    printf("Dziekuje za skorzystanie z aplikacji Twitter 2.0\n");

    return 0;
}