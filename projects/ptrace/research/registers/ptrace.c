#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/syscall.h>
#include <string.h>
#include <stdio.h>


void reverse(char *str)
{   int i, j;
    char temp;
    for(i = 0, j = strlen(str) - 2;
        i <= j; ++i, --j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

int main() {
    pid_t pid;
    int status;
    struct user_regs_struct regs; // Used to access CPU registers
    char* str; // Assuming a maximum string length of 100 characters

    if ((pid = fork()) == 0) {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execl("/bin/echo", "echo", "HELLO WORLD");
    } else {
        while (1) {
            wait(&status);

            if (WIFEXITED(status)) {
                break;
            }

             ptrace(PTRACE_GETREGS, pid, NULL, &regs);

        // Wartość wskazywana przez RDI będzie wskaźnikiem do stringa
        long long string_addr = regs.rdi;

        // Teraz możemy odczytać string z pamięci procesu potomnego
        char buffer[256];
        long data;

        int i = 0;
        while (1) {
            data = ptrace(PTRACE_PEEKDATA, pid, string_addr + i, NULL);
            if (data == -1) {
                break;
            }
            buffer[i] = (char)(data & 0xFF);
            i++;
        }
        buffer[i] = '\0';
        printf("Odczytany string: %s\n", buffer);

        ptrace(PTRACE_CONT, pid, NULL, NULL);
        }
        printf("TA KURWA JUZ NIE DZIALA \n");
    }
    return 0;
}
