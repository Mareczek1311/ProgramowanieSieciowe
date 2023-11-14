section .data
    rok dd 2023  ; zmienna rok o rozmiarze double word (4 bajty)

section .text
    global _start

_start:
    ; Inicjalizacja zmiennych i rejestrów
    mov eax, [rok]  ; Wczytaj wartość zmiennej rok do rejestru eax

    ; Wypisanie wartości zmiennej rok
    mov eax, 1      ; Numer systemowego wyjścia (STDOUT)
    mov ebx, 1      ; Deskryptor pliku (STDOUT)
    mov ecx, eax    ; Wartość zmiennej rok do wypisania
    mov edx, 4      ; Długość danych do wypisania (4 bajty)
    int 0x80        ; Przerwanie systemowe - wywołanie sys_write

    ; Zakończenie programu
    mov eax, 1      ; Numer systemowego wyjścia (EXIT)
    xor ebx, ebx    ; Kod wyjścia 0
    int 0x80        ; Przerwanie systemowe - wywołanie sys_exit
