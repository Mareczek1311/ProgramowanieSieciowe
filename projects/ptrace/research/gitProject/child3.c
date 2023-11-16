#include <stdio.h>

int main() {
    char message[] = "hello!\n";

    asm (

        "mov %0, %%rsi\n"          // pointer to the message
        "mov $7, %%rdx\n"          // message length
        "L1: cmpq $245, (%%rsi)\n\t"

        "jne L1\n\t"
        "mov $60, %%rax\n"         // syscall number for sys_exit
        "xor %%rdi, %%rdi\n"       // exit code 0
        "syscall"
        :
        : "r"(message)
        : "%rax", "%rdi", "%rsi", "%rdx"
    );

    return 0;
}
