#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

double Sum(long long num, long long size) {
    double part_sum = 0.0;

    for(long long i = 0; i < size; i++) {
        part_sum += 1.0 / num++;
    }
    
    return part_sum;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Usage: %s [N] for Sum_1/N !\n", argv[0]);
        return 0;
    }

    // MPI_Status status;
    
    MPI_Init(&argc, &argv);
    
    const long long num = atoll(argv[1]);
    long long part_size = 0;

    // define communicator size and rank
    int commSize, rank;
   
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    long long elem_per_process = num / commSize;

    // master process
    if (rank == 0) {

        int i; // iterations for processes

        // check if more than 1 processes are run
        if (commSize > 1) {
            // distribute the portion of sum
            for (i = 1; i < commSize - 1; i++) {
                MPI_Send(&elem_per_process, 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD);
            }

            long long elem_left = num - elem_per_process * i; 

            // last process add remaining elements 
            MPI_Send(&elem_left, 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD);
        }

        // partial sum for master process
        double sum = Sum(1, elem_per_process);

        printf("process id: %d, part_sum = %.16lg\n", rank, sum);

        double tmp = 0.0;
        for (i = 1; i < commSize; i++) {
            MPI_Recv(&tmp, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);            
            // int sender = status.MPI_SOURCE; //not use
            sum += tmp;
        }

        printf("Tolal Sum = %.16lg\n", sum);

    } 
    // slave processes
    else {
        // receive partial size for calculaling partial sum for each process
        MPI_Recv(&part_size, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        double part_sum = Sum(elem_per_process*rank + 1 , part_size);

        printf("process id: %d, part_sum = %.16lg\n", rank, part_sum);

        // send part_sum to master process 
        MPI_Send(&part_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

    }

    MPI_Finalize();

    return 0;
}
