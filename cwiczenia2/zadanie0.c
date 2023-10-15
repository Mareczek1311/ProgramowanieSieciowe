#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>

void mysyserr(char *mymsg){
    fprintf(stderr, "ERROR: %s (errno: %d, %s) \n", mymsg, errno, strerror(errno));
    exit(EXIT_FAILURE);
}

int main() {

    char buf[128];
    int ile;
    if((ile = read(77, buf, 128)) == -1)
        mysyserr("blad funkcji read");

    return 0;

}