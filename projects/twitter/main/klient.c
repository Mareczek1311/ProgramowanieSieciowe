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

int lock(int semid, int posts_count){
    struct sembuf sem_op;

    for (int i = 0; i < posts_count; ++i) {
        // Sprawdź, czy semafor jest zajęty
        sem_op.sem_num = i;
        sem_op.sem_op = -1;
        sem_op.sem_flg = IPC_NOWAIT;

        if (semop(semid, &sem_op, 1) == -1) {
            //perror("semop");
            //printf("Semafor %d jest zajęty.\n", i);
        }
        else if(db->posts[i].isSet == 1){
            //printf("Pamiec %d jest juz zajeta \n", i);
        }
        else {
            return i;
        }
    }
    return -1;
}

void releaseSemaphore(int semid, int semnum) {
    struct sembuf sb;
    sb.sem_num = semnum;
    sb.sem_op = 1;
    sb.sem_flg = 0;

    if (semop(semid, &sb, 1) == -1) {
        perror("semop");

    }
}

int main(int argc, char* argv[]){

    char option;
    char msg[MSG_SIZE];
    char username[MSG_SIZE];
    int post;
    int post_count;
    int* post_count_ptr;
    size_t rozmiar;
    int locked;

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

    //printf("Ilosc postow: %d \n", post_count);

    rozmiar = sizeof(struct database) + post_count * sizeof(struct post);

    //printf("Rozmiar: %d \n", rozmiar);

    // --- TWORZENIE SEMAFOROW ---
    if((semid = semget(key, post_count, 0666 | IPC_EXCL)) == -1){
        perror("semget");
        exit(1);
    }

    // --- TWORZENIE PAMIECI WSPOLDZIELONEJ ---
    if((shmid = shmget(key, rozmiar, 0666)) == -1){
        perror("shmget");
        exit(1);
    }

    db = (struct database *)shmat(shmid, NULL, 0);
    if (db == (struct database *)(-1)) {
        perror("shmat");
        exit(1);
    }

    printf("Twitter 2.0 wita! (wersja C)\n");
    printf("Wolnych %d wpisow (na %d) \n", post_count - db->curr_server, post_count);

    if(!isEmpty(db)){
        print_posts(db, 1);
    }

    printf("Podaj akcje (N)owy wpis, (L)ike \n");

    scanf("%c", &option);

    switch(option){
        case 'N':
            locked = lock(semid, post_count);

            if(locked == -1){
                printf("Brak miejsca na nowy wpis\n");
                break;
            }

            //printf("Zablokowalem %d semafor \n" ,locked);
            printf("Napisz co chodzi ci po glowie: \n");
            printf("> ");

            char c;
            while ((c = getchar()) != '\n' && c != EOF);
            fgets(msg, sizeof(msg), stdin);
            msg[strcspn(msg, "\n")] = '\0';

            strncpy(db->posts[locked].content, msg, MSG_SIZE);
            strncpy(db->posts[locked].username, username, MSG_SIZE);
            db->posts[locked].likes = 0;
            db->posts[locked].isSet = 1;

            releaseSemaphore(semid, locked);

            db->curr_server++;

            break;
        case 'L':
            printf("Ktory wpis chcesz polubic: \n");
            printf("> ");
            scanf("%d", &post);
            post-=1;
            if(post >= db->curr_server || post <= 0){
                if(post < 0){

                    printf("Podany wpis nie istnieje\n");
                    break;
                }


                if(db->posts[post].isSet == 0 ){
                    printf("Podany wpis nie istnieje\n");
                    break;
                }
            }

            if(post<db->curr_server){
                db->posts[post].likes++;
            }

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
