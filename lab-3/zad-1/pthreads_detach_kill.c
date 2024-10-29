#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

void * zadanie_watku (void * arg_wsk);

int zmienna_wspolna=0;

#define WYMIAR 1000
#define ROZMIAR WYMIAR*WYMIAR
double a[ROZMIAR],b[ROZMIAR],c[ROZMIAR];

// funkcja wykonuje operacje mnozenia macierzy (czasochłonne zadanie)
double czasozajmowacz(){
  int i, j, k;
  int n=WYMIAR;
  for(i=0;i<ROZMIAR;i++) a[i]=1.0*i;
  for(i=0;i<ROZMIAR;i++) b[i]=1.0*(ROZMIAR-i);
  for(i=0;i<n;i++){
    for(j=0;j<n;j++){
      c[i+n*j]=0.0;
      for(k=0;k<n;k++){
	c[i+n*j] += a[i+n*k]*b[k+n*j];
      }
    }
  }
  return(c[ROZMIAR-1]);
}

void * zadanie_watku (void * arg_wsk)
{
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); // wyłącza możliwość zabicia wątku
	printf("\twatek potomny: uniemozliwione zabicie\n");

	czasozajmowacz();	// symulacja pracy

	printf("\twatek potomny: umozliwienie zabicia\n");
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); // wątek podatny na anulowanie

	pthread_testcancel();	//sprawdza czy wątek został anulowany

	zmienna_wspolna++;		// sygnal ze zadanie zostalo wykonane
	printf("\twatek potomny: zmiana wartosci zmiennej wspolnej\n");

	return(NULL);
}

int main()
{
	pthread_t tid;
	pthread_attr_t attr;
	void *wynik;
	int i;

	// 1. Tworzenie i anulowanie pierwszego wątku (wątek przyłączalny)
	printf("watek glowny: tworzenie watku potomnego nr 1\n");
	pthread_create(&tid, NULL, zadanie_watku, NULL); // domyślne wartości
	sleep(2);	// czas na uruchomienie wątku

	printf("\twatek glowny: wyslanie sygnalu zabicia watku\n");
	pthread_cancel(tid); // próba zabicia wątku

	pthread_join(tid, &wynik);	// czekanie na zakończenie wątku, sprawdzanie wynik jego zakończenia (czy został anulowany)

	//Co nalezy zrobić przed sprawdzeniem czy wątki się skonczyły? joina
	if (wynik == PTHREAD_CANCELED)
		printf("\twatek glowny: watek potomny zostal zabity\n");
	else
		printf("\twatek glowny: watek potomny NIE zostal zabity - blad\n");


	// 2. Tworzenie drugiego wątku i jego odłączenie
	zmienna_wspolna = 0;
	printf("watek glowny: tworzenie watku potomnego nr 2\n");
	pthread_create(&tid, NULL, zadanie_watku, NULL); //domyślne wartości
	sleep(2);	//czas na uruchomienie watku

	printf("\twatek glowny: odlaczenie watku potomnego\n");
	pthread_detach(tid);	//odłączenie wątku

	printf("\twatek glowny: wyslanie sygnalu zabicia watku odlaczonego\n");
	pthread_cancel(tid);	//próba zabicia odłączonego wątku, pthread_join nie jest możliwy

	// sprawdzanie, czy wątek zmienił zmienną wspólną
	printf("\twatek glowny: czy watek potomny zostal zabity?\n");
	printf("\twatek glowny: sprawdzanie wartosci zmiennej wspolnej\n");
	for (i = 0; i < 10; i++){
		sleep(1);
		if (zmienna_wspolna != 0) break;
	}

	if (zmienna_wspolna == 0)
		printf("\twatek glowny: odlaczony watek potomny PRAWDOPODOBNIE zostal zabity\n");
	else
		printf("\twatek glowny: odlaczony watek potomny PRAWDOPODOBNIE NIE zostal zabity\n");

	
	// 3. Tworzenie wątku z atrybutami (odłączony od początku)
	pthread_attr_init(&attr);	// inicjalizacja atrybutów
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); // ustawianie typu watku na odłaczony

	printf("watek glowny: tworzenie odlaczonego watku potomnego nr 3\n");
	pthread_create(&tid, &attr, zadanie_watku, NULL); // tworzenie wątku z atrybutami
	pthread_attr_destroy(&attr);	// niszczenie atrybutów po użyciu

	printf("\twatek glowny: koniec pracy, watek odlaczony pracuje dalej\n");
	pthread_exit(NULL); // - pozwala glownemu watkowi zakonczyc sie, ale zostawia watki potomne, do zakonczenia ich dzialania
	// exit(0); // - program zakończy wszystkie wątki bez względu na tak czy działają czy nie
}


