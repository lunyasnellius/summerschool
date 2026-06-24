// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <vector>
#include <mpi.h>

void print_ordered(double t);

int main(int argc, char *argv[]) {

    constexpr int numElements = 10000000;
    std::vector<int> message(numElements);
    std::vector<int> receiveBuffer(numElements);

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Initialize buffers
    for (int i = 0; i < numElements; i++) {
        message[i] = rank;
        receiveBuffer[i] = -1;
    }

    //      rank1 -> rank2 -> rank3 -> ...
    // Treat boundaries with MPI_PROC_NULL.
    int source = rank-1;
    int destination = rank+1;

    if(rank == size-1) {
      destination = MPI_PROC_NULL;
    } else if(rank == 0) {
      source = MPI_PROC_NULL;
    }


    // Start measuring the time spent in communication
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    if(rank%2 == 0) {
    MPI_Send(message.data(), numElements, MPI_INT, destination, rank+1, MPI_COMM_WORLD);
    printf("Sender: %d. Sent elements: %d. Tag: %d. Receiver: %d\n",
           rank, numElements, rank + 1, destination
    );

    MPI_Recv(receiveBuffer.data(), numElements, MPI_INT, source, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Receiver: %d. first element %d\n", rank, receiveBuffer[0]);
    } else {
    MPI_Recv(receiveBuffer.data(), numElements, MPI_INT, source, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Receiver: %d. first element %d\n", rank, receiveBuffer[0]);

    MPI_Send(message.data(), numElements, MPI_INT, destination, rank+1, MPI_COMM_WORLD);
    printf("Sender: %d. Sent elements: %d. Tag: %d. Receiver: %d\n",
           rank, numElements, rank + 1, destination
    );
    }

    // Finalize measuring the time and print it out
    double t1 = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    fflush(stdout);

    print_ordered(t1 - t0);

    MPI_Finalize();
    return 0;
}

// Function that prints out timings. No need to modify this
void print_ordered(double t)
{
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {

        printf("Time elapsed in rank %2d: %6.3f\n", rank, t);
        for (int i = 1; i < size; i++) {
            MPI_Recv(&t, 1, MPI_DOUBLE, i, 11, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Time elapsed in rank %2d: %6.3f\n", i, t);
        }
    } else {
        MPI_Send(&t, 1, MPI_DOUBLE, 0, 11, MPI_COMM_WORLD);
    }
}
