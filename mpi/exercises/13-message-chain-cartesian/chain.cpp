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

	int dims[1] = {size};
	int coords[1];			// coordinates
	int neighbors[2];		// neighboring rank id
	MPI_Comm cart_comm;		// cartesian communicator
	int cart_rank;			// rank in cart_comm
	int period[1] = {1};		// boundary condition

	MPI_Dims_create(size, 1, dims);
	MPI_Cart_create(MPI_COMM_WORLD, 1, dims, period, 1, &cart_comm);
	MPI_Comm_rank(cart_comm, &cart_rank);
	MPI_Cart_coords(cart_comm, cart_rank, 1, coords);
	MPI_Cart_shift(cart_comm, 0, 1, &neighbors[0], &neighbors[1]);


    //      rank1 -> rank2 -> rank3 -> ...
    // Treat boundaries with MPI_PROC_NULL.
    int source = neighbors[0];
    int destination = neighbors[1];

    // Start measuring the time spent in communication
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

	MPI_Sendrecv(message.data(), numElements, MPI_INT, neighbors[1], rank,
		receiveBuffer.data(), numElements, MPI_INT, neighbors[0], MPI_ANY_TAG,
		cart_comm, MPI_STATUS_IGNORE);

    printf("Sender: %d. Sent elements: %d. Tag: %d. Receiver: %d\n",
           rank, numElements, rank, neighbors[1]);
    printf("Receiver: %d. first element %d\n", rank, receiveBuffer[0]);


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
