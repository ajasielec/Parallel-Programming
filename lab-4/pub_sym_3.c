// wykorzystanie trylock podczas wybierania zasobu, 
// zliczamy prace w trakcie oczekiwania na zasób

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex do synchronizacji
pthread_mutex_t mutexKran;  // Mutex do synchronizacji dostępu do kranow

int wolne_kufle; // Liczba wolnych kufli

void * watek_klient(void * arg);

int main(void) {

    srand(time(NULL));
    pthread_t *tab_klient;
    int *tab_klient_id;

    int l_kl, l_kf, l_kr, i;

    printf("Liczba klientow: ");
    scanf("%d", &l_kl);

    printf("Liczba kufli: ");
    scanf("%d", &l_kf);

    // inicjalizacja liczby wolnych kufli i kranow
    wolne_kufle = l_kf;
    l_kr = 1;

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutexKran, NULL);

    // alokacja pamieci
    tab_klient = (pthread_t *) malloc(l_kl * sizeof(pthread_t));
    tab_klient_id = (int *) malloc(l_kl * sizeof(int));
    for (i = 0; i < l_kl; i++) {
        tab_klient_id[i] = i;
    }

    printf("\nOtwieramy pub (simple)!\n");
    printf("Liczba wolnych kufli %d\n", wolne_kufle);

    // uruchamianie klientow
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

    printf("\nZamykamy pub!\n");

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutexKran);
    free(tab_klient);
    free(tab_klient_id);

    return 0;
}

void * watek_klient(void * arg_wsk) {
    int moj_id = *((int *)arg_wsk);
    int i, j;
    int wykonana_praca = 0;

    printf("\nKlient %d, wchodzę do pubu\n", moj_id);

    for (i = 0; i < ILE_MUSZE_WYPIC; i++) {
        int wait_time = rand() % 3 + 1; // Losowy czas oczekiwania od 1 do 3 sekund
        int success = 0;
        printf("\nKlient %d, próbuję wybrać kufel\n", moj_id);

        // Aktywne czekanie na wolny kufel
        do {
            if (pthread_mutex_trylock(&mutex) == 0){
                if (wolne_kufle > 0) {
                    // klient pobiera kufel
                    wolne_kufle--;
                    pthread_mutex_unlock(&mutex);   //odblokowanie dostepu do zmiennej
                    success = 1; // Kufel dostępny, wychodzimy z pętli

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
                    printf("\nKlient %d, odkładam kufel\n", moj_id);
                    pthread_mutex_lock(&mutex);
                    wolne_kufle++;
                }
                    pthread_mutex_unlock(&mutex);
            }
            if (!success) {
                printf("\nKlient %d, nie ma wolnych kufli!\n", moj_id);
                // sleep(wait_time);           // Oczekiwanie na dostępny kufel
                // wait_time = rand() % 3 + 1; // Losowanie nowego czasu oczekiwania
                wykonana_praca ++;
            }
            } while (!success);
    }

    printf("\nKlient %d, wychodzę z pubu, wykonana praca: %d\n", moj_id, wykonana_praca);
    return NULL;
}
