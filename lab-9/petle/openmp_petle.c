#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 5

int main ()
{
  double a[WYMIAR][WYMIAR];

  // inicjalizacja tablicy
  for(int i = 0; i < WYMIAR; i++) {
    for(int j = 0; j < WYMIAR; j++) {
        a[i][j] = 1.02 * i + 1.01 * j;
    }
  }

  // sekwencyjne sumowanie
  double suma = 0.0;
  for(int i = 0; i < WYMIAR; i++) {
    for(int j = 0; j < WYMIAR; j++) {
      suma += a[i][j];
    }
  }
  
  printf("Suma wyrazów tablicy: %lf\n", suma);

  // ustawienie opcji dla zagniezdzonych petli
  omp_set_nested(1);


  // dekompozycja wierszowa
  double suma_parallel_wierszowa = 0.0;
  #pragma omp parallel for default(none) shared(a) reduction(+:suma_parallel_wierszowa) ordered
  for (int i = 0; i < WYMIAR; i++) {
    for (int j = 0; j < WYMIAR; j++){
      suma_parallel_wierszowa += a[i][j];

      #pragma omp ordered
      printf("a [%1d,%1d] - W_%1d \n", i, j, omp_get_thread_num());
    }
  }

  printf("Suma wyrazów tablicy równolegle (dekompozycja wierszowa): %lf\n\n", suma_parallel_wierszowa);

  
  // dekompozycja kolumnowa
  double suma_parallel_kolumnowa = 0.0;
  #pragma omp parallel for default(none) shared(a) reduction(+:suma_parallel_kolumnowa) ordered
  for (int j = 0; j < WYMIAR; j++) {
    for (int i = 0; i < WYMIAR; i++){
      suma_parallel_kolumnowa += a[i][j];

      #pragma omp ordered
      printf("a [%1d,%1d] - W_%1d \n", i, j, omp_get_thread_num());
    }
  }

  printf("Suma wyrazów tablicy równolegle (dekompozycja kolumnowa): %lf\n", suma_parallel_kolumnowa);




  // // podwójna pętla - docelowo równolegle
  // double suma_parallel=0.0; int i,j;
  // // ...
  // for(i=0;i<WYMIAR;i++) {
  //   int id_w = omp_get_thread_num();
  //   // #pragma omp ordered
  //   // ...
  //   for(j=0;j<WYMIAR;j++) {
  //     suma_parallel += a[i][j];
  //     // #pragma omp ordered
  //     // dla dekompozycji 1D
  //     //printf("(%1d,%1d)-W_%1d ",i,j,omp_get_thread_num()); 
  //     // dla dekompozycji 2D
  //     //printf("(%1d,%1d)-W_%1d,%1d ",i,j,id_w,omp_get_thread_num()); 
  //   }
  //   // #pragma omp ordered
  //   printf("\n");
  // }

  // printf("Suma wyrazów tablicy równolegle: %lf\n", suma_parallel);

  return 0;
}
