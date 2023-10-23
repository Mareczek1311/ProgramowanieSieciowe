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

#include <limits.h> // Dla PATH_MAX
#include <unistd.h> // Dla realpath

//Drugi tryb;

//naprawic komende clear!

//inne formatowanie czasu
//problem z czyszczeniem pamieci

int countSubdirectories(const char *dirPath) {
    DIR *dir;
    struct dirent *entry;
    int subdirectoryCount = 0;

    if ((dir = opendir(dirPath)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                subdirectoryCount++;
            }
        }
    }

    closedir(dir);

    return subdirectoryCount;
}

long long countTotalBlocks(){
    long long totalBlocks = 0;
    DIR* dirp;
    struct dirent* dp;
    struct stat st;

    dirp = opendir("./");

    if (dirp == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    while ((dp = readdir(dirp)) != 0) {
        if(lstat(dp->d_name, &st) == 0){
            totalBlocks += st.st_blocks;
        }
    }
    closedir(dirp);

    return totalBlocks;
}

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
    int length = 0;
    sprintf(str, "%d", num);

    for(int i=0; i<20; i++){
        if(str[i] != '\0'){
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
    res[length] = '\0';
    return res;
}

char*** initializeStructure_Mode0(int rows){
    char*** structure = (char ***)malloc(rows * sizeof(char **));

    for(int i=0; i<rows; i++){
        structure[i] = (char**) malloc(9 * sizeof(char*));
    }

    return structure;
}

char** initializeStructure_Mode1(){
    char** structure = (char **)malloc(20 * sizeof(char *));
    return structure;
}

char* init_time(time_t mod_time){
    char buffer[1024];
    int length=0;

    strftime(buffer, sizeof(buffer), "%m-%d %H:%M", localtime(&mod_time));

    //printf("%s ", buffer);

    while(buffer[length] != '\0'){
        length++;
    }

    char* res = (char*) malloc(length * sizeof(char));

    for(int i=0; i<length; i++){
        res[i] = buffer[i];
    }

    res[length] = '\0';

    return res;
}

char* init_link(struct stat st, struct dirent* dp){
    if(!S_ISLNK(st.st_mode)){
        return NULL;
    }

    char buffer[1024];
    ssize_t link_string_length = -1;
    if((link_string_length = readlink(dp->d_name, buffer, 1024)) == -1){
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

void printLS_Mode0(char*** matrix, int* sizes, int rows, long long totalBlocks){
    
    printf("total %lld \n", totalBlocks);
    
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

void printLS_Mode1(char** tab){

    char resolved_path[PATH_MAX];

    printf("Informacje o %s \n", tab[8]);

    printf("Typ pliku: ");
    switch( tab[0][0] ){
        case 'd':
            printf("katalog \n");
            break;
        case '-':
            printf("zwykly plik \n");
            break;
        case 'l':
            printf("link \n");
            break;
        default:
            printf("TYPE NOT DEFIND \n");

    }
    
    if (realpath(tab[8], resolved_path) != NULL) {
            printf("Sciezka: %s \n", resolved_path);
    } else {
        perror("Błąd");
        exit(EXIT_FAILURE);
    }
    
    //DLA LINKOW SCIEZKA


    switch( tab[0][0] ){
        case 'd':
            printf("Liczba podkatalogow: %d \n", countSubdirectories(tab[8]));
            break;
        case '-':
            printf("Rozmiar: %s \n", tab[4]);
            break;
        case 'l':
            printf("Rozmiar: %s \n", tab[4]);
            break;
        default:
            printf("TYPE NOT DEFIND \n");

    }

    printf("Uprawnienia: ");

    printf("uzytkownik: ");
    for(int i=1; i<4; i++){
        if(tab[0][i] != '-'){printf("%c", tab[0][i]);}
    }

    printf(", grupa: ");
    for(int i=4; i<7; i++){
        if(tab[0][i] != '-'){printf("%c", tab[0][i]);}

    }

    printf(", inni: ");
    for(int i=7; i<10; i++){
        if(tab[0][i] != '-'){printf("%c", tab[0][i]);}
    }
    printf("\n");

    printf("Ostatnio uzywany: %s \n", tab[5]);
    printf("Ostatnio modyfikowany: %s \n", tab[6]);
    printf("Ostatnio zmieniany stan: %s \n", tab[7]);
    
}

//sort by filename need better sorting algorithm
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

void lsFunction(char*** lines, char** linesMode1,int mode, char* fileName){
 //Otworzenie katalogu ./
    int line = 0;
    
    DIR* dirp;
    struct dirent* dp;
    struct stat st;
    struct passwd* pw;
    struct group* gr;

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
     
        if(mode == 1){
            if(!strcmp(fileName, dp->d_name)){
                char* permissions = init_permissions(st.st_mode);
                char* link = FormatText(st.st_nlink);
                char* userName = pw->pw_name;
                char* groupName = gr->gr_name;
                char* size = FormatText(st.st_size);
                char* lastAccess = init_time(st.st_atim.tv_sec);
                char* lastModification = init_time(st.st_mtim.tv_sec);
                char* lastStatusChange = init_time(st.st_ctim.tv_sec);
                char* fileName = dp->d_name;
                char* fileLink = init_link(st, dp);

                linesMode1[0] = permissions;
                linesMode1[1] = link;
                linesMode1[2] = userName;
                linesMode1[3] = groupName;
                linesMode1[4] = size;
                linesMode1[5] = lastAccess;
                linesMode1[6] = lastModification;
                linesMode1[7] = lastStatusChange;
                linesMode1[8] = fileName;
                linesMode1[9] = fileLink;

            }
        }

        //Przypisywanie informacji MODE 0
        if(mode == 0){
            char* permissions = init_permissions(st.st_mode);
            char* link = FormatText(st.st_nlink);
            char* userName = pw->pw_name;
            char* groupName = gr->gr_name;
            char* size = FormatText(st.st_size);
            char* date = init_time(st.st_mtim.tv_sec);
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
    }
    closedir(dirp);

    //FREE
}

int main(int argc, char* argv[]) {

    //Deklaracja zmiennych

    //Jezeli mamy argument to tryb jest = 1
    //jezeli nie to mamy 0
    int mode = 0;

    int* sizesOfCols = (int*) malloc (8 * sizeof(int));

    long long totalBlocks = countTotalBlocks();
    int rows = numOfRows();

    char*** lines;
    char** linesMode1;
    
    if(mode == 0){
        lines = initializeStructure_Mode0(rows);
    } 
    else{
        linesMode1 = initializeStructure_Mode1();
    }     

    if( argc > 1 ) { mode = 1; }
    
    lsFunction(lines, linesMode1, mode, argv[1]);

    if(mode == 0){
        for(int i=0; i<7; i++){
            sizesOfCols[i] = maxSizeInColumn(lines, rows, i);
        }
        sort(lines, rows);

        printLS_Mode0(lines, sizesOfCols, rows, totalBlocks);
    }

    if(mode == 1){
        printLS_Mode1(linesMode1);
    }

    free(sizesOfCols);

    //Trzeba to naprawic :(
    //clearData(lines, rows, 7);

    return 0;
}

