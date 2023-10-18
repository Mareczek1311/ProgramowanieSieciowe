#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

// brak obslugi bledow!

int main(int argc, char* argv[]) {
	DIR* dirp;
	struct dirent* direntp;

	dirp = opendir(argv[1]);

    if (dirp == NULL){
        perror("Blad. ");
        exit(EXIT_FAILURE);
    }

	while((direntp = readdir(dirp)) != NULL) {
        if(!strcmp(direntp->d_name, ".") 
        || !strcmp(direntp->d_name, "..")){

        }
        else{
		    printf( "%s\n", direntp->d_name);
        }
	}
	closedir(dirp);

	return 0;
}