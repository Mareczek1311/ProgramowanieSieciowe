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
#include <string.h>

//Posortowac dane po nazwie plikow

//Dodac Total na szczycie

int numOfRows(){
    DIR* dirp;
    struct dirent* dp;
    int rows = 0;

    dirp = opendir("./");

    if (dirp == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    while ((dp = readdir(dirp)) != 0) {
        rows++;
    }
    closedir(dirp);

    return rows;
}

char* init_permissions(mode_t mode) {
    char* res = (char*)malloc(strlen("drwxrwxrwx") + 1);
    strcpy(res, "drwxrwxrwx");

    for (int i = 1; i < 10; i++) {
        if (!(mode & (1 << (8 - i)))) {
            res[i] = '-';
        }
    }

    if(mode >> 11 == 16){
        res[0] = '-';
    }
    if(S_ISLNK(mode)){
        res[0] = 'l';
    }

  
    //printf("%s \n" , res);
    return res;

}
//funkcja dla formatowania liczb w tablice
char* FormatText(int num){
    char str[20];
    char place[20];
    int length = 0;
    sprintf(str, "%d", num);

    for(int i=0; i<20; i++){
        if(str[i] != '\0'){
            //printf("%c \n", str[i]);
            length ++;
        }
        else{
            break;
        }
    }

    char* res = (char*)malloc(length * sizeof(char));

    for(int i=0; i<length; i++){
        res[i] = str[i];
    }

    return res;
}

char*** initializeStructure(int rows){
    char*** structure = (char ***)malloc(rows * sizeof(char **));

    for(int i=0; i<rows; i++){
        structure[i] = (char**) malloc(9 * sizeof(char*));
    }

    return structure;
}

char* init_time(struct stat st){
    char buffer[1024];
    int length=0;
    time_t mod_time = st.st_mtimespec.tv_sec;

    strftime(buffer, sizeof(buffer), "%m-%d %H:%M", localtime(&mod_time));

    //printf("%s ", buffer);

    while(buffer[length] != '\0'){
        length++;
    }

    char* res = (char*) malloc(length * sizeof(char));

    for(int i=0; i<length; i++){
        res[i] = buffer[i];
    }

    return res;
}

char* init_link(struct stat st, struct dirent* dp){
    if(!S_ISLNK(st.st_mode)){
        return NULL;
    }

    char buffer[1024];
    ssize_t link_string_length;
    if((link_string_length = readlink(dp->d_name, buffer, link_string_length)) == -1){
        perror("readlink");
    }
    else{
        buffer[link_string_length] = '\0';
        //printf(" -> %s ", buffer);
    }

    char* res = (char*) malloc(link_string_length * sizeof(char));

    for(int i=0; i<link_string_length; i++){
        res[i] = buffer[i];
    } 

    return res;
}

void clearData(char*** matrix, int x, int y){
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
    free(matrix);

}

int sizeOfTab(char* tab){
    int i = 0;

    while(tab[i] != '\0'){
        i++;
    }

    return i;    
}

int maxSizeInColumn(char*** matrix, int rowCount, int col){

    int maxSize = sizeOfTab(matrix[0][col]);
    int currSize;
    
    for(int i=1; i<rowCount; i++){
        currSize = sizeOfTab(matrix[i][col]);
        if(currSize > maxSize){
            maxSize = currSize;
        }    
    }

    return maxSize;
}

void printLS(char*** matrix, int* sizes, int rows){
    int size;
    for(int i=0; i<rows; i++){
        for(int j=0; j<8; j++){
            if(j<6){
                size = sizeOfTab(matrix[i][j]);
                for(int k=(sizes[j]-size); k>0; k--){
                    printf(" ");
                }
            }
            if(matrix[i][j])
            {
                if(j == 7){printf("-> ");}
                printf("%s", matrix[i][j]);   
            }
            if(j<6){
                printf("  ");
            }
            else{
                printf(" ");
            }
        }
        printf("\n");
    }
}

void swap(char** a, char** b) { }

//sort by filename
void sort(char*** matrix, int rows){
    for (int i = 0; i < rows - 1; i++) {
        for (int j = 0; j < rows - i - 1; j++) {
            if (strcmp(matrix[j][6], matrix[j + 1][6]) > 0) {
                 char** temp = matrix[j]; 
                 matrix[j] = matrix[j+1]; 
                 matrix[j+1] = temp;
            }
        }
    }

}



int main(int argc, char* argv[]) {
    //Deklaracja zmiennych
    DIR* dirp;
    struct dirent* dp;
    struct stat st;
    struct passwd* pw;
    struct group* gr;
    int line = 0;
    int* sizesOfCols = (int*) malloc (8 * sizeof(int));

    int rows = numOfRows();

    char*** lines = initializeStructure(rows);

    //Otworzenie katalogu ./
    dirp = opendir("./");

    //Kontrola bledow
    if (dirp == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((dp = readdir(dirp)) != 0) {

        //Kontrola bledow
        //uzywam lstat bo lstat daje informacje ze plik jest linkiem
        if (lstat(dp->d_name, &st) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        if ((gr = getgrgid(st.st_gid)) == NULL) {
            perror("getgrgid");
            exit(EXIT_FAILURE);
        }

        if ((pw = getpwuid(st.st_uid)) == NULL) {
            perror("getpwuid");
            exit(EXIT_FAILURE);
        }
     
        //Drukowanie informacji

        char* permissions = init_permissions(st.st_mode);
        char* link = FormatText(st.st_nlink);
        char* userName = pw->pw_name;
        char* groupName = gr->gr_name;
        char* size = FormatText(st.st_size);
        char* date = init_time(st);
        char* fileName = dp->d_name;
        char* fileLink = init_link(st, dp);

        lines[line][0] = permissions;
        lines[line][1] = link;
        lines[line][2] = userName;
        lines[line][3] = groupName;
        lines[line][4] = size;
        lines[line][5] = date;
        lines[line][6] = fileName;
        lines[line][7] = fileLink;
        
        line++;
    }
    
    for(int i=0; i<7; i++){
        sizesOfCols[i] = maxSizeInColumn(lines, rows, i);
    }

    sort(lines, rows);
    printLS(lines, sizesOfCols, rows);

    free(sizesOfCols);
    closedir(dirp);



    //clearData(lines, rows, 8);

    return 0;
}
