#include <stdlib.h>
     #include <sys/types.h>
     #include <sys/uio.h>
     #include <unistd.h>
     #include <stdio.h>
     #include <sys/errno.h>
     #include <string.h>

void mysyerr(char *msgerr){
    fprintf(stderr, "Error: %d %s", errno,  msgerr);
    exit(0);
}

int main(){
    
    char buf[128];
    int ile;
    if((ile = read(77, buf, 128)) == -1){
        mysyerr(strerror(errno));
    }
    return 0;
}
