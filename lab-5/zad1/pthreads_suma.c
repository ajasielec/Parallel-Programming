#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<math.h>

#include"pomiar_czasu.h"

#define ROZMIAR 1000000
#define LICZBA_W 2 // lepiej: stała - LICZBA_W_MAX i parametr p - liczba wątków 

pthread_mutex_t muteks;       // mutex do synchronizacji
pthread_t watki[LICZBA_W];    // tablica id watkow
double global_array_of_local_sums[LICZBA_W];  // sumy lokalne każdego watku

// funkcje dla watkow
void *suma_w( void *arg_wsk);
void *suma_w_no_mutex( void *arg_wsk);

double *tab;    // dane do sumowania
double suma=0;  // zmienna na sume (wersja z mutexem)

int main( int argc, char *argv[] ){
  int i; 
  double t1, t2, t3;

  int indeksy[LICZBA_W];
  for(i = 0; i < LICZBA_W; i++) indeksy[i] = i;   // inicjalizacja indeksow

  tab = (double *) malloc(ROZMIAR * sizeof(double));  // alokacja pamieci
  for(i = 0; i < ROZMIAR; i++){
    tab[i] = ((double) i + 1) / ROZMIAR; // wypelnienie tablicy 
  }


  // Sekwencyjne oblicznie sumy (bez watkow)
  //printf("Obliczenia sekwencyjne\n");
  t1 = czas_zegara();
  for (i = 0; i < ROZMIAR; i++){ 
    suma += tab[i]; 
  }
  t1 = czas_zegara() - t1;
  //printf("suma = %lf\n", suma);
  printf("%lf\n", t1);


  pthread_mutex_init( &muteks, NULL);   // inicjalizacja mutexa


  // Wersji z watkami i mutexem
  //printf("Poczatek tworzenia watkow\n");
  t1 = czas_zegara();
  suma = 0;

  // tworzenie watkow, przekazanie indeksu przez wskaznik na element tablicy 'indeksy'
  for(i = 0; i < LICZBA_W; i++){
    pthread_create( &watki[i], NULL, suma_w, (void *) &indeksy[i] );
    // code below - synchronization error !!!
    // pthread_create( &watki[i], NULL, suma_w, (void *) &i );
  }

  // oczekiwanie na zakonczenie kazdego watku
  for(i = 0; i < LICZBA_W; i++) pthread_join(watki[i], NULL);

  // pomiar czasu dla wersji równoleglej z mutexem
  t1 = czas_zegara() - t1;
  //printf("suma = %lf\n", suma);
  printf("%lf\n", t1);


  // Wersja bez mutexa, z tablica lokalnych sum
  suma =0;
  //printf("Poczatek tworzenia watkow\n");
  t1 = czas_zegara();

  // tworzenie watkow bez mutexa
  for(i=0; i<LICZBA_W; i++ ) {
    global_array_of_local_sums[i] = 0.0;  // suma lokalna dla watku
    pthread_create( &watki[i], NULL, suma_w_no_mutex, (void *) &indeksy[i] );
    // synchronization error !!!
    //pthread_create( &watki[i], NULL, suma_w, (void *) &i );
  }

  // oczekiwanie na zakonczenie kazdego watku i sumowanie wynikow lokalnych
  for(i=0; i<LICZBA_W; i++ ) {
    pthread_join(watki[i], NULL);
    suma += global_array_of_local_sums[i];
  }

  // pomiar czasu dla wersji bez mutexa
  t1 = czas_zegara() - t1;
  //printf("suma = %lf\n", suma);
  printf("%lf\n", t1);

/*   suma =0; */

/*   printf("Poczatek tworzenia watkow OpenMP\n"); */
/*   t1 = czas_zegara(); */

/* #pragma omp parallel for num_threads(LICZBA_W) default(none) shared (tab) reduction(+:suma)  */
/*   for(i=0;i<ROZMIAR;i++){ */
/*     suma += tab[i]; */
/*   } */


/*   t1 = czas_zegara() - t1; */
/*   printf("suma = %lf\n", suma); */
/*   printf("Czas obliczen OpenMP = %lf\n", t1); */

}

/* Funkcja watku z użyciem mutexa */
void *suma_w( void *arg_wsk){
  int i, moj_id;
  double moja_suma=0;

  moj_id = *( (int *) arg_wsk ); //odczytanie id watku

  // oblicznie przedzialu, ktorym zajmuje sie watek
  // j = ceil((float)ROZMIAR / LICZBA_W);
  // co w przypadku jeśli ROZMIAR jest niepodzielne przez LICZBA_W i następuje
  // zaokrąglenie w górę? - jak zagwarantować nieprzekroczenie w wersji
  // równoległej górnej granicy iteracji N?
  // if(j * LICZBA_W != ROZMIAR) { printf("Error! Exiting.\n"); exit(0);}

  // oblicznie przedzialu, ktorym zajmuje sie watek (niweluje problem niepodzielnosci)
  int start = (ROZMIAR / LICZBA_W) * moj_id;
  int end = (moj_id == LICZBA_W - 1) ? ROZMIAR : start + (ROZMIAR / LICZBA_W);

  // sumowanie elementow w zakresie
  for(i = start; i < end; i++) {
    moja_suma += tab[i]; 
  }

  // sekcja krytyczna - dodanie sumy lokalnej do globalnej (z uzyciem mutexa)
  pthread_mutex_lock( &muteks );
  suma += moja_suma;
  pthread_mutex_unlock( &muteks );

  pthread_exit( NULL );
}


/* Funkcja watku bez uzycia mutexa */
void *suma_w_no_mutex( void *arg_wsk){
  int i, j, moj_id;
  double tmp = 0.0;   // suma lokalna

  moj_id = *( (int *) arg_wsk );    // odczytanie id watku

  // oblicznie przedzialu, ktorym zajmuje sie watek
  j=ceil( (float)ROZMIAR/LICZBA_W ); // could be double as well 

  // sumowanie elementow w zakresie
  for( i = j * moj_id; i < j * (moj_id + 1); i++){ 
    // not optimal - possible false sharing of data in single cache line
    //global_array_of_local_sums[moj_id] += tab[i];
    // better - probably would be employed by the compiler anyway
    tmp += tab[i];
  }

  // przypisanie sumy lokalnej do tablicy wyników bez mutexa
  // no critical section, but requires an array with length nr_threads
  global_array_of_local_sums[moj_id] = tmp;

  pthread_exit( (void *)0);
}
