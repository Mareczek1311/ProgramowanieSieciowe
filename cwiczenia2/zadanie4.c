#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


//sprawdzic czy jest katalogiem jak jest to d jak nie to -

//Utworzenie macierzy charow  

void print_permissions(mode_t mode) {
    char perms[] = "drwxrwxrwx"; 
    for (int i = 1; i < 10; i++) {
        if (!(mode & (1 << (8 - i)))) {
            perms[i] = '-';
        }
    }


    if(mode >> 11 == 16){
        perms[0] = '-';
    }
    if(S_ISLNK(mode)){
        perms[0] = 'l';

    }

    printf("%s", perms);
}


//smiec funkcja raczej
void FormatText(int num){
    char str[20];
    char place[20];
    int length = 0;
    sprintf(str, "%d", num);
    printf("%s \n", str);

    for(int i=0; i<20; i++){
        if(str[i] != '\0'){
            printf("%c \n", str[i]);
            length ++;
        }
        else{
            break;
        }
    }

    for(int i=0; i<20; i++){

    }

}




int main(int argc, char* argv[]) {
    //Deklaracja zmiennych
    DIR* dirp;
    struct dirent* dp;
    struct stat st;
    struct passwd* pw;
    struct group* gr;
    char buffer[1024];
    char buffer2[1024];
    ssize_t link_string_length;

    //Otworzenie katalogu ./
    dirp = opendir("./");

    //Kontrola bledow
    if (dirp == NULL) {
        perror("opendir");
        return 1;
    }

    while ((dp = readdir(dirp)) != 0) {

        //Kontrola bledow
        //uzywam lstat bo lstat daje informacje ze plik jest linkiem
        if (lstat(dp->d_name, &st) == -1) {
            perror("stat");
            continue;
        }

        if ((gr = getgrgid(st.st_gid)) == NULL) {
            perror("getgrgid");
            continue;
        }

        if ((pw = getpwuid(st.st_uid)) == NULL) {
            perror("getpwuid");
            continue;
        }

        //Ustawianie czasu
        time_t mod_time = st.st_mtimespec.tv_sec;

        strftime(buffer, sizeof(buffer), "%m-%d %H:%M", localtime(&mod_time));

        
        printf("\n");
        //Drukowanie informacji
        print_permissions(st.st_mode);
        printf(" %d ", st.st_nlink);
        printf("%s ", pw->pw_name);
        printf("%s ", gr->gr_name);
        printf("%lld ", (long long)st.st_size);
        printf("%s ", buffer);
        printf("%s", dp->d_name);
        if(S_ISLNK(st.st_mode)){
            if((link_string_length = readlink(dp->d_name, buffer2, link_string_length)) == -1){
                perror("readlink");
            }
            else{
                buffer[link_string_length] = '\0';
                printf(" -> %s ", buffer2);
            }

        }
    }

    closedir(dirp);
    return 0;
}
