#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "czytelnia.h"

/*** Implementacja procedur interfejsu ***/
pthread_mutex_t mutex;

// zmienna warunku

void inicjuj(cz_t* cz_p) {
    pthread_mutex_init(&mutex, NULL);
    cz_p->l_p = 0;
    cz_p->l_c = 0;
}

int my_read_lock_lock(cz_t* cz_p) {
    pthread_mutex_lock(&mutex);
    cz_p->l_c++;
    pthread_mutex_unlock(&mutex);

    // w ifie pthread_cont_wait
}

int my_read_lock_unlock(cz_t* cz_p) {
    pthread_mutex_lock(&mutex);
    cz_p->l_c--;
    pthread_mutex_unlock(&mutex);
}

int my_write_lock_lock(cz_t* cz_p) {
    pthread_mutex_lock(&mutex);
    cz_p->l_p++;
    pthread_mutex_unlock(&mutex);
}

int my_write_lock_unlock(cz_t* cz_p) {
    pthread_mutex_lock(&mutex);
    cz_p->l_p--;
    pthread_mutex_lock(&mutex);    
}


void czytam(cz_t* cz_p) {
    printf("\t\t\t\t\tczytam: l_c %d, l_p %d\n", cz_p->l_c, cz_p->l_p);
    
    // Sprawdzamy poprawność liczby czytelników i pisarzy
    if (cz_p->l_p < 0 || cz_p->l_c < 0 || cz_p->l_p > 1 || (cz_p->l_p == 1 && cz_p->l_c > 0)) {
        printf("Blad: niepoprawna liczba pisarzy (%d) lub czytelników (%d)!\n", cz_p->l_p, cz_p->l_c);
        exit(0);
    }
    
    usleep(rand() % 3000000);
}

void pisze(cz_t* cz_p) {
    printf("\t\t\t\t\tpisze: l_c %d, l_p %d\n", cz_p->l_c, cz_p->l_p);
    
    // Sprawdzamy poprawność liczby czytelników i pisarzy
    if (cz_p->l_p < 0 || cz_p->l_c < 0 || cz_p->l_p > 1 || (cz_p->l_p == 1 && cz_p->l_c > 0)) {
        printf("Blad: niepoprawna liczba pisarzy (%d) lub czytelników (%d)!\n", cz_p->l_p, cz_p->l_c);
        exit(0);
    }
    
    usleep(rand() % 3000000);
}
