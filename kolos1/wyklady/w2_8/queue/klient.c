#include "q.h"

int main(){
    struct my_msgbuf buf;
    int msqid;
    key_t key;

    if((key = ftok("q.h", 'B')) == -1){
        perror("ftok");
        exit(1);
    }

    if((msqid = msgget(key, 0644 | IPC_CREAT)) == -1){
        perror("msgget");
        exit(1);
    }

    printf("Podaj tekst: ");

    buf.mtype = 1;
    
    while(fgets(buf.mtext, MAXLINE, stdin) != NULL){
        if(msgsnd(msqid, (struct msgbuf *)&buf, sizeof(buf), 0) == -1){
            perror("msgsnd");
            exit(1);
        }
    }

}
