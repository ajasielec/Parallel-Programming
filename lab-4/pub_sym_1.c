#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 3

void *watek_klient(void *arg);
pthread_mutex_t mutex; // Mutex do synchronizacji dostępu do kufli
pthread_cond_t cond;   // Zmienna warunkowa dla klientów
int l_kf;              // Liczba kufli
int wolne_kufle;      // Liczba wolnych kufli

int main(void) {
    pthread_t *tab_klient;
    int *tab_klient_id;
    int l_kl, i;

    printf("\nLiczba klientow: "); scanf("%d", &l_kl);
    printf("\nLiczba kufli: "); scanf("%d", &l_kf);

    // Inicjalizacja mutexa i zmiennej warunkowej
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    wolne_kufle = l_kf; // Początkowa liczba wolnych kufli

    tab_klient = (pthread_t *)malloc(l_kl * sizeof(pthread_t));
    tab_klient_id = (int *)malloc(l_kl * sizeof(int));
    for (i = 0; i < l_kl; i++) tab_klient_id[i] = i;

    printf("\nOtwieramy pub!\n");
    printf("\nLiczba wolnych kufli %d\n", wolne_kufle);

    for (i = 0; i < l_kl; i++) {
        pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]);
    }
    for (i = 0; i < l_kl; i++) {
        pthread_join(tab_klient[i], NULL);
    }

    printf("\nZamykamy pub!\n");

    // Czyszczenie zasobów
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    free(tab_klient);
    free(tab_klient_id);

    return 0;
}

void *watek_klient(void *arg_wsk) {
    int moj_id = *((int *)arg_wsk);
    int i;

    printf("\nKlient %d, wchodzę do pubu\n", moj_id);

    for (i = 0; i < ILE_MUSZE_WYPIC; i++) {
        pthread_mutex_lock(&mutex); // Zablokowanie dostępu do kufli
        wolne_kufle--; // Zajmowanie kufla
        printf("\nKlient %d, nalewam z kranu\n", moj_id);
        usleep(30);
        printf("\nKlient %d, pije, pozostało kufli %d\n", moj_id, wolne_kufle);
        nanosleep((struct timespec[]){{0, 50000000L}}, NULL);

        wolne_kufle++; // Oddawanie kufla
        printf("\nKlient %d, odkladam kufel\n", moj_id);
        pthread_mutex_unlock(&mutex); // Odblokowanie dostępu do kufli
    }

    printf("\nKlient %d, wychodzę z pubu\n", moj_id);
    return NULL;
}

// klienci korzystaja tylko z 1 kufla