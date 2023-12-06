#include <stdio.h>
#include <dirent.h>
#include <string.h>
// brak obslugi bledow!

int main(int argc, char* argv[]) {
    
    if(argc != 2){
        printf("-- ./a.out [sciezka] --\n");
        return -1;
    }

	DIR* dirp;
	struct dirent* direntp;

	dirp = opendir(argv[1]);
	while((direntp = readdir(dirp)) != NULL) {
        if(!strcmp(".", direntp->d_name) || !strcmp("..", direntp->d_name)){
            continue;
        }
		printf( "%s\n", direntp->d_name);
	}
	closedir(dirp);

	return 0;
}

