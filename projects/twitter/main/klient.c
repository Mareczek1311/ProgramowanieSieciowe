#include "data.h"


//argv[1] - sciezka_do_pliku
//argv[2] - username

int main(int argc, char* argv[]){

    key_t key;
    int shmid;
    struct database* db;
    char option;
    char msg[MSG_SIZE];
    char username[MSG_SIZE];
    int post;

    /*
        if(argv != 3){
            ///
        }
    */  

   strncpy(username, argv[1], MSG_SIZE);

    //trzeba zastąpić prawidlowym plikiem podanym od klienta
    if((key = ftok("data.h", 'A')) == -1){
        perror("ftok");
        exit(1);
    }

    if((shmid = shmget(key, SHM_SIZE, 0644 | IPC_EXCL)) == -1){
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

    printf("Podaj akcje (N)owy wpis, (L)ike \n");

    scanf("%c", &option);

    switch(option){
        case 'N':
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
            db->curr_server++;
            //Nowy wpis
            break;
        case 'L':
            //Like
            printf("Ktory wpis chcesz polubic: \n");
            printf("> ");
            scanf("%d", &post);
            post-=1;
            if(post<db->curr_server){
                db->posts[post].likes++;
            }
            break;
        default:
            printf("Podana opcja nie istnieje\n");
    }
    printf("Dziekuje za skorzystanie z aplikacji Twitter 2.0\n");
    /*
    p = (struct post*) shmat(shmid, (void*)0, 0);


    if(argc == 4){
        printf("Dodaje do posta...\n");
        strncpy(p->username, argv[1], MSG_SIZE);
        strncpy(p->content, argv[2], MSG_SIZE);
        p->likes = atoi(argv[3]);
    }  
    else{
        printf("Segment zawiera: \n");
        printf("Username: %s \n", p->username);
        printf("Content: %s \n", p->content);
        printf("Likes: %d \n", p->likes);
    }
    */
    if(shmdt(db) == -1){
        perror("shmdt");
        exit(1);
    }


    return 0;
}