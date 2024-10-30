// Symulacja pubu - wersja bez zabezpieczenia dostepu do zmiennej wspolnej

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 333

void *watek_klient(void *arg);
int wolne_kufle;    // zmienna globalna, wspolna dla wszystkich watkow


int main( void ){
    pthread_t *tab_klient;
    int *tab_klient_id;
    int l_kl, l_kf, i;

    printf("\nLiczba klientow: "); scanf("%d", &l_kl);
    printf("\nLiczba kufli: "); scanf("%d", &l_kf);

    wolne_kufle = l_kf;     // poczatkowa liczba kufli

    tab_klient = (pthread_t *) malloc(l_kl*sizeof(pthread_t));
    tab_klient_id = (int *) malloc(l_kl*sizeof(int));
    for(i=0;i<l_kl;i++) tab_klient_id[i]=i;

    printf("\nOtwieramy pub (simple)!\n");
    printf("\nLiczba wolnych kufli %d\n", wolne_kufle); 

    for(i = 0; i < l_kl; i++){
        pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]); 
    }
    for(i = 0; i < l_kl; i++){
        pthread_join( tab_klient[i], NULL);
    }

    // porownanie poczatkowej i koncowej liczby kufli
    if (wolne_kufle != l_kf){
        printf("\nBlad: Zmiana calkowitej liczby kufli");
        printf("\nPoczatkowa liczba kufli: %d", l_kf);
        printf("\nKoncowa liczba kufli: %d\n", wolne_kufle);
    }

    printf("\nZamykamy pub! Pozostało kufli: %d\n", wolne_kufle);

    free(tab_klient);
    free(tab_klient_id);

    return 0;
}

void * watek_klient (void * arg_wsk){
  int moj_id = * ((int *)arg_wsk);
  int i;

  printf("\nKlient %d, wchodzę do pubu\n", moj_id); 
    
  for(i=0; i<ILE_MUSZE_WYPIC; i++){
        // klient bierze kufel
        wolne_kufle--;
        // printf("\nKlient %d, pije, pozostało kufli %d\n", moj_id, wolne_kufle); 

        // sprawdzenie czy klient wzial kufel mimo ze nie ma juz wolnych kufli
        if (wolne_kufle < 0){
            printf("\nBlad: Klient %d, pobral kufel mimo braku kufli!\n", moj_id);
            printf("Liczba kufli: %d\n", wolne_kufle);
        }

        // klient pije
        usleep(1);
        // nanosleep((struct timespec[]){{0, 50000000L}}, NULL);
        // printf("\nKlient %d, odkladam kufel\n", moj_id);

        // klient odklada kufel
        wolne_kufle++;
  }

   printf("\nKlient %d, wychodzę z pubu\n", moj_id); 
    
  return(NULL);
} 