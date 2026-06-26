// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include <omp.h>


int main(int argc, char *argv[])
{
	int rank, ntasks, size;
	int provided, required;
	required = MPI_THREAD_MULTIPLE;
	MPI_Init_thread(&argc, &argv, required, &provided);

	if(provided < required) {
		printf("not supporting required thread level\n");
		MPI_Abort(MPI_COMM_WORLD, -1);
		return 0;
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int tid = 0;
	int msg = -1;
	int tag = 123;
#pragma omp parallel
{
	tid = omp_get_thread_num();
	ntasks = omp_get_num_threads();
	if (rank == 0) {
		msg = tid;
		for (int i = 1; i < size; i++) {
			MPI_Send(&msg, 1, MPI_INT, i, tid, MPI_COMM_WORLD);
		}
	} else {
		MPI_Recv(&msg, 1, MPI_INT, 0, tid, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Rank %d thread %d received %d\n", rank, tid, msg);
	}
}
	MPI_Finalize();
	return 0;
}
