#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SCALAR double
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

int main (int argc, char **argv) {
  int rank, size, max_iterations;
  SCALAR local_sum = 0.0, global_sum = 0.0;

  // inicjalizacja mpi
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // pobieranie rangi procesu
  MPI_Comm_size(MPI_COMM_WORLD, &size);   // pobieranie rozmiaru komunikatora

  // proces 0 pobiera liczbe iteracji i rozsyla ja do wszystkich procesow
  if (rank == 0) {
    printf("Podaj maksymalną liczbę wyrazów do obliczenia przybliżenia PI: ");
    fflush(stdout);
    scanf("%d", &max_iterations);
  }

  int root = 0;

  MPI_Bcast(&max_iterations, 1, MPI_INT, root, MPI_COMM_WORLD);

  // obliczenie zakresu iteraccji dla kazdego procesu (kompozycja blokowa)
  int chunk_size = max_iterations / size;
  int start = rank * chunk_size;
  int end = (rank == size - 1) ? max_iterations : start + chunk_size;

  // obliczanie lokalnej sumy dla danego procesu
  for (int i = start; i < end; ++i) {
    int j = 1 + 4 * i;
    local_sum += 1.0 / j - 1.0 / (j + 2.0);
  }

  // redukcja wyników lokalnych do procesu 0
  MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);

  // proces 0 wypisuje wynik
  if (rank == 0) {
    SCALAR pi_approx = 4.0 * global_sum;
    printf("PI obliczone: \t\t\t%20.15lf\n", pi_approx);
    printf("PI z biblioteki matematycznej: \t%20.15lf\n", M_PI);
  }

  // finalizacja MPI
  MPI_Finalize();
  return 0;
}