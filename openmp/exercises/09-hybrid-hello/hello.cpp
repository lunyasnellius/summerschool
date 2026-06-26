// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <omp.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

	int rank = 0;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	#pragma omp parallel
	{
		int tid = omp_get_thread_num();
		printf("Hello from thread %d in process %d!\n", tid, rank);
	}

	MPI_Finalize();
	return 0;
}
