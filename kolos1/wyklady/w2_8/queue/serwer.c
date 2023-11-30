#include "q.h"

int main(void){
    struct my_msgbuf buf;
    int msqid;
    key_t key;

    if((key = ftok("klient.c", 'B')) == -1){
        perror("ftok");
        exit(1);
    }
    
    if ((msqid = msgget(key, 0644)) == -1) {
        perror("msgget");
        exit(1);
    }
    printf("czytam \n");

    for(;;){
                if (msgrcv(msqid, (struct msgbuf *)&buf, sizeof(buf), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        printf("Czytam: %s \n", buf.mtext);
    }

    return 0;
}
