#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    
    MPI_Init(&argc, &argv);
    
    int value = 0;

    // define the communicator size and rank
    int commSize, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // slave processes
    if (rank) {
        // recieve data from the previous process
        MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    printf("process id: %d, value = %d;  value = %d -> process %d\n", rank, value, value+1, (rank+1) % commSize);

    value++;

    // send data to the next process
    MPI_Send(&value, 1, MPI_INT, (rank+1) % commSize, 0, MPI_COMM_WORLD);

    // master process
    if (rank == 0) {
        // recieve data from the last process
        MPI_Recv(&value, 1, MPI_INT, commSize - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);            
        
        printf("END LOOP: process id: %d, value = %d\n", rank, value);
    }

    MPI_Finalize();

    return 0;
}