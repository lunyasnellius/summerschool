#include <mpi.h>

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);

	int rank;
	int size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLS, &size);

	int send-message[size];
	for(int i=0; i<size; ++i) {
		message[i] = rank;
	}

	int to-rank;
	int from-rank;
	if(rank == size-1) {
		to-rank = 0;
		from-rank = rank-1;
	} else if(rank == 0) {
		to-rank = rank+1;
		from-rank = size-1;
	} else {
		to-rank = rank+1;
		from-rank = rank-1;
	}

	int recv-message[size];
	if(rank%2 == 0) {
		MPI_Send(*send-message, size, MPI_INT, to-rank, rank, MPI_COMM_WORLD);
	} else {
		MPI_Recv(*recv-message, size, MPI_INT, from-rank, rank, MPI_COMM_WORLD);
	}
	if(rank%2 == 0) {
		MPI_Recv(*recv-message, size, MPI_INT, from-rank, rank, MPI_COMM_WORLD);
	} else {
		MPI_Send(*send-message, size, MPI_INT, to-rank, rank, MPI_COMM_WORLD);
	}

	printf("On rank %i, the sent message is %i and the received message is %i\n",
			rank, send-message[0], recv-message[0]);

	MPI_Finalize();
}
