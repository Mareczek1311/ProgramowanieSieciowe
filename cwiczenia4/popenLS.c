 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <sys/types.h>
 #include <string.h>
 #define MAXLINE 1024
 #define PAGER "/usr/bin/ls"
 
 
 
 int main(int argc, char** argv){
 
 
     char line[MAXLINE];
     FILE *fpout;
 
 
     if((fpout = popen("ls", "r"))== NULL){
         perror("blad");
         exit(0);
     }
     while(fgets(line, MAXLINE, fpout) != NULL){
         printf("%s", line);
     }
 
     if(pclose(fpout)){
         perror("blad");
     }
 
     return 0;
 }
