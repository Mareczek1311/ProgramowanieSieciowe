     #include <stdio.h>
     #include <unistd.h>
#include<stdlib.h>

int main(int argc, char* argv[]){

    FILE *fin, *fout;
    char buf[1024];


    fout = popen("/bin/ls", "w");

    while(fgets(buf, 1024, fout) != NULL){
        write(fileno(stdout), buf, 1024);
    }
    
    pclose(fout);

    return 0;
}

