#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>

#include "mpi.h"

int main( int argc, char** argv ){ 
  
  int rank, ranksent, size, source, dest, tag, i; 
  MPI_Status status;
  
  // inicjalizacja MPI
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); // ranga (ID) procesu
  MPI_Comm_size( MPI_COMM_WORLD, &size ); // liczba procesow

  const int stringSize = 256;
  char hostname[stringSize];
  memset(hostname, 0, sizeof(hostname));

  gethostname(hostname, sizeof(hostname));  // pobieranie nazwy hosta
  
  // komunikacja procesow
  if(size>1)
  {
    if( rank != 0 ){
      // wysylanie swojej nazwy do procesu 0
      MPI_Send( hostname, stringSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD );
      printf("Rank %d: Sent hostname: '%s' to rank 0.\n", rank, hostname);
    } else {
        // odbieranie wiadomości przez proces 0
        for( i=1; i<size; i++ ) {
          char nameSent[stringSize];
          memset(nameSent, 0, sizeof(nameSent));
          MPI_Recv(nameSent, stringSize, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
          nameSent[stringSize - 1] = '\0';
          printf("Dane od procesu o randze (status.MPI_SOURCE ->) %d: %d, %s (i=%d)\n", 
          status.MPI_SOURCE, ranksent, nameSent, i );
      }
    }
  }
  else{
	  printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
  }

  // zakonczenie programu
  MPI_Finalize(); 
  return(0);
}

