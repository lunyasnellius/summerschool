#include <mpi.h>
#include <iostream>
#include <vector>


int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int ext[2];		// extents / dims
	if (size < 4) { printf("too small, try again"); }
	else if (size >=  4 && size < 16 ) { ext[0] = 2; }
	else if (size >= 16 && size < 64 ) { ext[0] = 4; }
	else if (size >= 64 && size < 256) { ext[0] = 8; }
	else 			      { ext[0] = 16; }

	ext[1] = size/ext[0];
	if(ext[0] * ext[1] != size) {
		std::cerr << "Incompatible dimensions:" << 
			ext[0] << "x" << ext[1] << "!=" << size << std::endl;
		MPI_Finalize();
		return -1;
	}

	int coords[2];			// coordinates
	int neighbors[4];		// neighboring rank id
	MPI_Comm cart_comm;		// cartesian communicator
	int cart_rank;			// rank in cart_comm
	int periods[2] = {1,1};		// boundary condition

	MPI_Dims_create(size, 2, ext);
	MPI_Cart_create(MPI_COMM_WORLD, 2, ext, periods, 1, &cart_comm);
	MPI_Comm_rank(cart_comm, &cart_rank);
	MPI_Cart_coords(cart_comm, cart_rank, 2, coords);
	MPI_Cart_shift(cart_comm, 0, 1, &neighbors[0], &neighbors[1]);
	MPI_Cart_shift(cart_comm, 1, -1, &neighbors[2], &neighbors[3]);

	MPI_Barrier(cart_comm);

	printf("rank %i: coords: %i,%i neighbors: x=(%i,%i), y=(%i,%i)\n",
			rank, coords[0], coords[1],
			neighbors[0], neighbors[1], neighbors[2], neighbors[3]);


	MPI_Finalize();
	return 0;
}//end of main



