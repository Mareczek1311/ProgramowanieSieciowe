#include <stdio.h>

int main() {
    printf("child3 starts...\n");

    asm("pushq %rbx\n\t"
        "movq $143, %rbx\n\t"
        "L1: cmpq $245, %rbx\n\t"
        "jne L1\n\t"
        "popq %rbx\n\t");

    printf("child3 outside loop...\n");

    return 0;
}
