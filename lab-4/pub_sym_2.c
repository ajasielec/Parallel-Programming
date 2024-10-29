#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex do synchronizacji
int wolne_kufle; // Liczba wolnych kufli

void * watek_klient(void * arg);

int main(void) {
    pthread_t *tab_klient;
    int *tab_klient_id;

    int l_kl, l_kf, i;

    printf("Liczba klientow: ");
    scanf("%d", &l_kl);

    printf("Liczba kufli: ");
    scanf("%d", &l_kf);

    // Inicjalizacja liczby wolnych kufli
    wolne_kufle = l_kf;

    tab_klient = (pthread_t *) malloc(l_kl * sizeof(pthread_t));
    tab_klient_id = (int *) malloc(l_kl * sizeof(int));
    for (i = 0; i < l_kl; i++) {
        tab_klient_id[i] = i;
    }

    printf("\nOtwieramy pub (simple)!\n");
    printf("Liczba wolnych kufli %d\n", wolne_kufle);

    for (i = 0; i < l_kl; i++) {
        pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]);
    }
    for (i = 0; i < l_kl; i++) {
        pthread_join(tab_klient[i], NULL);
    }

    printf("\nZamykamy pub!\n");
    free(tab_klient);
    free(tab_klient_id);

    return 0;
}

void * watek_klient(void * arg_wsk) {
    int moj_id = *((int *)arg_wsk);
    int i;

    printf("\nKlient %d, wchodzę do pubu\n", moj_id);

    for (i = 0; i < ILE_MUSZE_WYPIC; i++) {
        int wait_time = rand() % 3 + 1; // Losowy czas oczekiwania od 1 do 3 sekund

        printf("\nKlient %d, próbuję wybrać kufel\n", moj_id);

        // Aktywne czekanie na wolny kufel
        do {
            pthread_mutex_lock(&mutex);
            if (wolne_kufle > 0) {
                wolne_kufle--;
                pthread_mutex_unlock(&mutex);
                break; // Kufel dostępny, wychodzimy z pętli
            }
            pthread_mutex_unlock(&mutex);
            printf("\nKlient %d, nie ma wolnych kufli! Czekam %d sekund...\n", moj_id, wait_time);
            sleep(wait_time); // Oczekiwanie na dostępny kufel
            wait_time = rand() % 3 + 1; // Losowanie nowego czasu oczekiwania
        } while (1);

        printf("\nKlient %d, nalewam z kranu\n", moj_id);
        usleep(30);

        printf("\nKlient %d, pije, pozostało kufli %d\n", moj_id, wolne_kufle);
        nanosleep((struct timespec[]){{0, 50000000L}}, NULL);

        printf("\nKlient %d, odkładam kufel\n", moj_id);
        pthread_mutex_lock(&mutex);
        wolne_kufle++;
        pthread_mutex_unlock(&mutex);
    }

    printf("\nKlient %d, wychodzę z pubu\n", moj_id);
    return NULL;
}
