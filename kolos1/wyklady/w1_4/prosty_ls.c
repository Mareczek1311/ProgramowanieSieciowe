#include <stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<stdlib.h>

int main(int argc, char* argv[]){

    DIR* dir;
    struct dirent * dp;
    struct stat statbuf;

    if((dir = opendir(argv[1])) == NULL){
        perror("opendir");
        exit(1);
    }

    while((dp = readdir(dir)) != NULL){
        if(stat(dp->d_name, &statbuf) == -1){
            continue;
        }
        printf("%o %s \n", statbuf.st_mode, dp->d_name);
    }

    closedir(dir);

    return 0;
}