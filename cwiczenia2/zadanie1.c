#include<stdio.h>
#include<sys/types.h>
#include <sys/errno.h>
#include<pwd.h>
#include<sys/stat.h>
#include <grp.h>
#include <uuid/uuid.h>
#include <string.h>
#include<stdlib.h>

/* Brak obslugi bledow! */

int main(int argc, char **argv) {

	struct stat statbuf;
	struct passwd *pwd;
	struct group *gwd;


	//Dodac obsluge bledow
	if(stat(argv[1], &statbuf) == -1){
		perror("Brak pliku");
		exit(EXIT_FAILURE);
	}

	pwd = getpwuid(statbuf.st_uid);
	gwd = getgrgid(statbuf.st_gid);

	printf("Wlasciciel: %s\n", pwd -> pw_name);
	printf("Grupa: %s\n", gwd -> gr_name);

	return 0;
}