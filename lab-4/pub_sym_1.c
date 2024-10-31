// Symulacja pubu - zabezpieczenie dostepu do zasobu lock/unlock
// duzo kufli, 1 kran, 
// zabezpieczamy odbieranie, kran i oddawanie
// 2 mutexy, odbieranie i oddawanie, drugi na kran 

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 3

void *watek_klient(void *arg);

pthread_mutex_t mutex;  // Mutex do synchronizacji dostępu do kufli
pthread_mutex_t mutexKran;  // Mutex do synchronizacji dostępu do kranow

int wolne_kufle;        // zmienna wspolna dla watkow


int main(void) {
    pthread_t *tab_klient;
    int *tab_klient_id;
    int l_kl, l_kf, l_kr, i;

    printf("\nLiczba klientow: "); scanf("%d", &l_kl);
    printf("\nLiczba kufli: "); scanf("%d", &l_kf);

    wolne_kufle = l_kf;
    l_kr = 1;

    // Inicjalizacja mutexa i zmiennej warunkowej
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutexKran, NULL);

    tab_klient = (pthread_t *)malloc(l_kl * sizeof(pthread_t));
    tab_klient_id = (int *)malloc(l_kl * sizeof(int));
    for (i = 0; i < l_kl; i++) tab_klient_id[i] = i;

    printf("\nOtwieramy pub!\n");
    printf("\nLiczba wolnych kufli %d\n", l_kf);

    for (i = 0; i < l_kl; i++) {
        pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]);
    }
    for (i = 0; i < l_kl; i++) {
        pthread_join(tab_klient[i], NULL);
    }

   // porownanie poczatkowej i koncowej liczby kufli
    if (wolne_kufle != l_kf){
        printf("\nBlad: Zmiana calkowitej liczby kufli");
        printf("\nPoczatkowa liczba kufli: %d", l_kf);
        printf("\nKoncowa liczba kufli: %d\n", wolne_kufle);
    }

    printf("\nZamykamy pub! Liczba kufli: %d\n", l_kf);

    // Czyszczenie zasobów
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutexKran);
    free(tab_klient);
    free(tab_klient_id);

    return 0;
}

void *watek_klient(void *arg_wsk) {
    int moj_id = *((int *)arg_wsk);
    int i, j;

    printf("\nKlient %d, wchodzę do pubu\n", moj_id);

    for (i = 0; i < ILE_MUSZE_WYPIC; i++) {

        // bezpieczne pobieranie kufla
        pthread_mutex_lock(&mutex); // zablokowanie dostępu do kufli
        wolne_kufle --;             // klient bierze kufel
        pthread_mutex_unlock(&mutex);
        
        printf("\nKlient %d, wybieram kufel\n", moj_id); 

        // klient nalewa z kranu
        pthread_mutex_lock(&mutexKran);
        j=0;
        printf("\nKlient %d, nalewam z kranu %d\n", moj_id, j);
        usleep(30);
        pthread_mutex_unlock(&mutexKran);

        // klient pije
        printf("\nKlient %d, pije, pozostało kufli %d\n", moj_id, wolne_kufle);
        nanosleep((struct timespec[]){{0, 50000000L}}, NULL);

        // klient odklada kufel
        pthread_mutex_lock(&mutex); 
        wolne_kufle++;
        pthread_mutex_unlock(&mutex); // odblokowanie dostępu do kufli
        
        printf("\nKlient %d, odkladam kufel\n", moj_id);
    }

    printf("\nKlient %d, wychodzę z pubu\n", moj_id);
    return NULL;
}