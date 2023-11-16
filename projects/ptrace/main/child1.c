#include <stdio.h>
#include<time.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
char* szyfrowanie(char mess[], int przes, int len){

    for(int i=0; i<len; i++){
        mess[i] += przes;
    }
    return mess;
}

int main() {
    char* ptr = (char*)malloc(7 * sizeof(char));
    char* klucz = (char*) malloc(7*sizeof(char));

    if (ptr == NULL) {
        perror("perror");
        exit(EXIT_FAILURE);  // Zakończ program z kodem błędu
    }
    if
    strcpy(ptr, "hellob");
    strcpy(klucz, "hellob");
    ptr[7] = '\0';
    klucz[7] = '\0';


    int przes = 1;
    for (int i = 0; i < 7; i++) {
        ptr[i] += przes;
    }

    printf("%s \n", ptr);
    asm (
        "mov $1, %%rbx\n"
        "L1: mov %0, %%rsi\n"
        "mov $1, %%rcx\n" //przesuniedzie jako stala
        "mov $7, %%rdx\n"
        "cmp $1, %%rbx\n"
        "je L1\n\t"
        "jne L2\n"
        "mov $60, %%rax\n"
        "xor %%rdi, %%rdi\n"
        "syscall\n"
        "L2:\n"
        :
        : "r"(ptr), "r"(przes)
        : "%rax", "%rdi", "%rsi", "%rdx", "%rbx", "%rcx"
    );
    printf("PTR:  %s \n", ptr);
    printf("KLUCZ:  %s \n", klucz);
    if (strcmp(ptr, klucz) == 0) {
        printf("WYCIEK DANYCH \n");
    }

    return 0;
}
