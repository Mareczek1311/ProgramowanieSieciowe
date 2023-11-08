#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file = popen("nm my_program", "r");
    if (file == NULL) {
        perror("popen");
        exit(1);
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Przetwarzaj dane z pliku, aby uzyskać symbole
        printf("%s", buffer);  // Możesz dostosować przetwarzanie do swoich potrzeb
    }

    pclose(file);

    return 0;
}
