#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LICZBA_WATKOW 5

// funkcja wykonywana przez kazdy watek
void *zadanie_watku(void *arg) {    // arg jest wsk do id watku
    int id_watku = *((int *)arg);
    printf("Watek %d: systemowy ID = %lu\n", id_watku, pthread_self());
    return NULL;
}

int main(){
    pthread_t watki[LICZBA_WATKOW];
    int id[LICZBA_WATKOW];
    void *wynik;

    // Tworzenie watkow
    for (int i = 0; i < LICZBA_WATKOW; i++){
        id[i] = i;
        pthread_create(&watki[i], NULL, zadanie_watku, &id[i]);
    }

    // Dołączanie wątków (oczekiwanie na ich zakonczenie)
    for (int i = 0; i < LICZBA_WATKOW; i++){
        pthread_join(watki[i], &wynik);   // zapewnia ze program glowny czeka na zakonczenie watkow
    }

    // // potencjalny błąd z synchronizacją
    // pthread_t watki[LICZBA_WATKOW];
    // int id; // zmienna do przesłania, jest wspoldzielona przez wszystkie watki podczas ich towrzenia

    // for (id = 0; id < LICZBA_WATKOW; id++) {
    //     pthread_create(&watki[id], NULL, zadanie_watku, &id); // Przekazywanie adresu zmiennej id
    // }

    // for (id = 0; id < LICZBA_WATKOW; id++) {
    //     pthread_join(watki[id], NULL); // Czekanie na zakończenie wątków
    // }

    pthread_exit(NULL);
}