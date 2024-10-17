#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "pomiar_czasu.h"
 
const int liczba = 100000;

void main(){

  double a, b, c;
  int i,j,k,l;
  double t1, t2;

  k = -50000;
  // poczatek pomiaru czasu
  t1 = czas_zegara();
  t2 = czas_CPU();
  for(i=0;i<liczba;i++){

    printf("%d ",k+i);

  }
  printf("\n");
  // koniec pomiaru czasu
  t1 = czas_zegara() - t1;
  t2 = czas_CPU() - t2;

  printf("Czas wykonania %d operacji wejscia/wyjscia: \n",liczba);
  printf("czas zegara: %lf\n", t1);
  printf("czas procesora: %lf\n", t2);
  
  a = 1.000001;
  // poczatek pomiaru czasu
  t1 = czas_zegara();
  t2 = czas_CPU();
  for(i=0;i<liczba;i++){

    a = 1.000001*a+0.000001; 

  }
  // koniec pomiaru czasu
  t1 = czas_zegara() - t1;
  t2 = czas_CPU() - t2;

  printf("Wynik operacji arytmetycznych: %lf\n", a);
  printf("Czas wykonania %d operacji arytmetycznych: \n",liczba);
  printf("czas zegara: %lf\n", t1);
  printf("czas procesora: %lf\n", t2);

}
