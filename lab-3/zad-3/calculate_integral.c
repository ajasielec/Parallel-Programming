#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

typedef struct {
    // dane wejsciowe
    double start;
    double end;
    // dane wyjsciowe
    double result; // Pole na wynik
} ThreadData;

// funkcja watku, przyjmuje wskaźnik do swojej struktury
void* thread_function(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    data->result = (data->start + data->end) / 2.0; // Średnia jako przykładowa operacja
}

int main(){

    double a = 0.0, b = 10.0;
    pthread_t threads[NUM_THREADS];
    ThreadData data[NUM_THREADS];
    
    // tworzenie watkow
    double step = (b - a) / NUM_THREADS; // dzieli przedzial na mniejsze
    for (int i = 0; i < NUM_THREADS; i++) {
        data[i].start = a + i * step; 
        data[i].end = a + (i + 1) * step;
        pthread_create(&threads[i], NULL, thread_function, &data[i]); // tworzy tablicę struktur z argumentami dla każdego wątku (ograniczone przedziały)
    }
    // czekanie na zakonczenie watkow
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // sumowanie wyników, wypisywanie struktury
    double total = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("data[%d].start = %lf; ", i, data[i].start);
        printf("data[%d].start = %lf; ", i, data[i].start);
        printf("data[%d].result = %lf\n", i, data[i].result);
        total += data[i].result;
    }
    // obliczanie sredniej
    double avg = total/NUM_THREADS;
    
    printf("Thread number: %d\n", NUM_THREADS);
    printf("Sum of results: %f\n", total);
    printf("Avarage of a = %f and b = %f : %f\n", a, b, avg);
    return 0;
}