// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <vector>
#include <mpi.h>

#define MAX_PRINT_SIZE 12

void init_buffer(std::vector<int> &buffer);
void print_buffer(std::vector<int> &buffer);


int main(int argc, char *argv[])
{
    int size, rank, bufsize;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    bufsize = 12800;// * size;
    int sctrsize = bufsize / size;

    std::vector<int> buf(bufsize);
    std::vector<int> rbuf(bufsize);

    /* Initialize message buffer */
    init_buffer(buf);

    /* Print data that will be sent */
    print_buffer(buf);

    /* Start timing */
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    MPI_Request request[size-1];
    if(rank==0) {
    /* Send everywhere from 0 */
	    for(int i=1; i<size; ++i) {
		    MPI_Isend(&buf[i*sctrsize], sctrsize, MPI_INT, 
			    i, i, MPI_COMM_WORLD, &request[i-1]);
	    }
    } else {
    /* Recv from 0 everywhere */
	    MPI_Recv(buf.data(), sctrsize, MPI_INT, 
		    0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    if(rank == 0) {
	    MPI_Waitall(size-1, request, MPI_STATUS_IGNORE);
	    for(int i=sctrsize; i<bufsize; ++i) buf[i]=0;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    /* End timing */
    double t1 = MPI_Wtime();

    /* Print data that was received */
    print_buffer(buf);
    if (rank == 0) {
        printf("Time elapsed manual implementation: %6.8f s\n\n", t1 - t0);
    }

    init_buffer(buf);
    print_buffer(buf);

    MPI_Barrier(MPI_COMM_WORLD);
    t0 = MPI_Wtime();

    MPI_Scatter(buf.data(), sctrsize, MPI_INT, rbuf.data(), sctrsize, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Bcast(buf.data(), buf.size(), MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    t1 = MPI_Wtime();

    /* Print data that was received */
    print_buffer(rbuf);
    if (rank == 0) {
        printf("Time elapsed using Scatter: %6.8f s\n", t1 - t0);
    }

    MPI_Finalize();
    return 0;
}


void init_buffer(std::vector<int> &buffer)
{
    int rank;
    int buffersize = buffer.size();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        for (int i = 0; i < buffersize; i++) {
            buffer[i] = i;
        }
    } else {
        for (int i = 0; i < buffersize; i++) {
            buffer[i] = -1;
        }
    }
}


void print_buffer(std::vector<int> &buffer)
{
    int rank, size;
    int buffersize = buffer.size();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> printbuffer(buffersize * size);

    MPI_Gather(buffer.data(), buffersize, MPI_INT,
               printbuffer.data(), buffersize, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int j = 0; j < size; j++) {
            printf("Task %2i:", j);
            for (int i = 0; i < MAX_PRINT_SIZE; i++) {
                printf(" %2i", printbuffer[i + buffersize * j]);
            }
            if (MAX_PRINT_SIZE < buffersize) {
                printf(" ...");
            }
            printf("\n");
        }
        printf("\n");
    }
}
