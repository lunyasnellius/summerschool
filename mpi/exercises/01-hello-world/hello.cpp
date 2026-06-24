// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{

	MPI_Init(&argc, &argv);

	int size;
	int rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	char name[MPI_MAX_PROCESSOR_NAME];
	int length;
	MPI_Get_processor_name(name, &length);

	// say hello! in parallel
	// printf("Hello from rank %i of %i\n", rank, size);

	if(rank == 0) {
		printf("rank %i: There's %i processes in total.\n", rank, size);
	} 
	if(rank == 42) {
		printf("rank %i: I'm the Answer to the Ultimate Question of Life, the Universe, and Everything!\n", rank);
	} 
	if(rank == (size-1)) {
		printf("rank %i: I am the last, but not the least!\n", rank);
	} 
	std::cout << "rank " << rank << " is running on processor " << name << std::endl;

	MPI_Finalize();

}
