#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>

int zmienna_globalna = 0;

#define ROZMIAR_STOSU 1024 * 64

int funkcja_wątku(void* argument) {
    int lokalna_zmienna = *((int*) argument);
    for(int i = 0; i < 100000; i++) {
        zmienna_globalna ++;
        lokalna_zmienna ++;
    }

    printf ("Wątek zakończony: zmienna_globalna = %d, lokalna_zmienna = %d\n",
            zmienna_globalna, lokalna_zmienna);
    return 0;
}

int main() {
    void* stos1;
    void* stos2;
    pid_t pid1, pid2;

    // alokacja stosu dla wątku
    stos1 = malloc(ROZMIAR_STOSU);
    stos2 = malloc(ROZMIAR_STOSU);
    if (stos1 == 0 || stos2 == 0) {
        printf("Błąd alokacji stosu");
        exit(1);
    }

    int lokalna_zmienna1 = 0;
    int lokalna_zmienna2 = 0;

    // tworzenie pierwszego wątku
    pid1 = clone(&funkcja_wątku, (void*)stos1 + ROZMIAR_STOSU,
                CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM,
                &lokalna_zmienna1);

    // tworzenie drugiego wątku
    pid2 = clone(&funkcja_wątku, (void*)stos2 + ROZMIAR_STOSU,
                CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM,
                &lokalna_zmienna2);

    // oczekiwanie na zakończenie
    waitpid(pid1, NULL, __WCLONE);
    waitpid(pid2, NULL, __WCLONE);

    // wypisanie wartości zmiennych
    printf("Wszystkie wątki zakończone: zmienna_globalna = %d, lokalna_zmienna1 = %d, lokalna_zmienna2 = %d\n",
            zmienna_globalna, lokalna_zmienna1, lokalna_zmienna2);

    free(stos1);
    free(stos2);

    return 0;
}

/*
-> wartość zmiennych lokalnych jest zgodna z oczekiwaniami (100000)
-> zmienna globalna (ok. 150000) jest mniejsza od oczekiwanego wyniku (200000)
    czyli niektóre zwiększenia zostały utracone z powodu współbieżnego dostępu
-> zmienne lokalne są unikalne dla każdego wątku, natomiast globalne są współdzielone
*/