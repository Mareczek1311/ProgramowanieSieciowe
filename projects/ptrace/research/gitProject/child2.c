#include <stdio.h>

int main() {
    char myString[6]; // Zmienna string na "siema" (5 znaków + terminator null)

    asm(
        "movq $0, %rbx\n\t"      // Ustawienie wartości 0 w rejestrze rdi
        "movb $'s', (%rbx)\n\t"  // Zapisanie 's' na pierwszej pozycji w myString
        "inc %rbx\n\t"            // Inkrementacja rdi (przesunięcie do następnego bajtu)
        "movb $'i', (%rbx)\n\t"  // Zapisanie 'i' na drugiej pozycji w myString
        "inc %rbx\n\t"            // Inkrementacja rdi
        "movb $'e', (%rbx)\n\t"  // Zapisanie 'e' na trzeciej pozycji w myString
        "inc %rbx\n\t"            // Inkrementacja rdi
        "movb $'m', (%rbx)\n\t"  // Zapisanie 'm' na czwartej pozycji w myString
        "inc %rbx\n\t"            // Inkrementacja rdi
        "movb $'a', (%rbx)\n\t"  // Zapisanie 'a' na piątej pozycji w myString
    );


    return 0;
}
