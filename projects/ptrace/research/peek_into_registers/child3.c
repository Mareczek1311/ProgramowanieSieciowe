#include <stdio.h>

int main() {
    printf("child3 starts...\n");

    int ebx_value;  // Deklarujmy zmienną do przechowywania wartości EBX

    asm(
        "pushl %%ebx\n\t"
        "movl  $143, %0\n\t"  // Używamy %0 do odwołania się do ebx_value
        "L1: cmpl $245, %0\n\t"
        "jne L1\n\t"
        "popl %%ebx\n\t"
    );

    printf("child3 outside loop...\n");

    return 0;
}

