#include<stdio.h>
#include<sys/types.h>
#include<pwd.h>
#include<sys/stat.h>
     #include <grp.h>
     #include <uuid/uuid.h>
/* Brak obslugi bledow! */
     #include <stdio.h>

int main(int argc, char **argv) {

    if(argc != 2){
        printf("-- ./a.out [nazwa_pliku] --\n");
        return -1;
    }

	struct stat statbuf;
	struct passwd *pwd;
    struct group *gr;

	stat(argv[1], &statbuf);
	pwd = getpwuid(statbuf.st_uid);
    gr = getgrgid(statbuf.st_gid);

	printf("Wlasciciel: %s\n", pwd -> pw_name);
    printf("Grupa: %s \n", gr -> gr_name);
	return 0;
}

