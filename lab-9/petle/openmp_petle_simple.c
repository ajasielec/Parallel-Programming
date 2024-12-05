#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 18

int main ()
{
  double a[WYMIAR];

  // SPOSOBY USTAWIENIA LICZBY WATKOW
  // 1. zmienna srodowiskowa
  // w terminalu: export OMP_NUM_THREADS=4

  // 2. klauzula num_threads w dyrektywe pragma - tylko dla danej dyrektywy
  /*
    #pragma  omp parallel for num_threads(4) default(none) shared(a)
    for(int i = 0; i < WYMIAR; i++) a[i] = 1.02 * i;
  */

  // 3. funkcja omp_set_num_threads - dla wszystkich rownoleglych akcji
  omp_set_num_threads(4);

  // inicjalizacja tablicy
  #pragma omp parallel for default(none) shared(a)
  for(int i = 0; i < WYMIAR; i++) a[i] = 1.02 * i;

  // sekwencyjne obliczanie sumy
  double suma = 0.0;
  for(int i = 0; i < WYMIAR; i++) {
      suma += a[i];
  }
  
  printf("Suma wyrazów tablicy: %lf\n", suma);

  // rownolegle obliczanie sumy
  double suma_parallel = 0.0;

  #pragma omp parallel for default(none) shared(a) reduction(+ : suma_parallel) ordered
  // #pragma omp parallel for default(none) shared(a) schedule (static, 3) reduction (+ : suma_parallel) ordered
  // #pragma omp parallel for default(none) shared(a) schedule (static) reduction (+ : suma_parallel) ordered
  // #pragma omp parallel for default(none) shared(a) schedule (dynamic, 2) reduction (+ : suma_parallel) ordered
  // #pragma omp parallel for default(none) shared(a) schedule (dynamic) reduction (+ : suma_parallel) ordered

  for(int i = 0; i < WYMIAR; i++) {
    int id_w = omp_get_thread_num();
    suma_parallel += a[i];

    // wymuszona kolejnosc operacji
    #pragma omp ordered
    printf("a[%2d]->W_%1d  \n", i, id_w); 
  }

  printf("\nSuma wyrazów tablicy równolegle (z klauzulą - ordered): %lf\n",
	 suma_parallel);

  return 0;
}
