#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int liczba = 0; // Wartość przekazywana w sztafecie
    int source, dest; // Źródło i cel komunikatu

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Sprawdzenie liczby procesów
    if (size < 2) {
        printf("Potrzeba przynajmniej dwóch procesów do realizacji sztafety!\n");
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        // Proces o randze 0 inicjuje sztafetę
        liczba = 1; // Wartość startowa
        dest = rank + 1; // Wysyłamy do procesu 1
        printf("Proces %d wysłał liczbę %d do procesu %d\n", rank, liczba, dest);
        MPI_Send(&liczba, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        
    } else {
        source = rank - 1; 
        MPI_Recv(&liczba, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Proces %d odebrał liczbę %d od procesu %d\n", rank, liczba, source);

        liczba += 1;
        
        if (rank < size - 1) { 
            dest = rank + 1; 
            printf("Proces %d wysłał liczbę %d do procesu %d\n", rank, liczba, dest);
            MPI_Send(&liczba, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        } 
    }

    MPI_Finalize();
    return 0;
}
