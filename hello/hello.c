#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    // define communicator size and rank
    int commSize, rank;
   
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello World!!! -> Communicator size = %d, rank = %d\n", commSize, rank);

    MPI_Finalize();

    return 0;
}