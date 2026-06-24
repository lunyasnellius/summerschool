#include <mpi.h>
#include <iostream>

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);

	int rank;
	int size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int msg_size=100000;
	int send_message[msg_size];
	for(int i=0; i<msg_size; ++i) {
		send_message[i] = rank;
	}

	int to_rank;
	int from_rank;
	if(rank == size-1) {
		to_rank = 0;
		from_rank = rank-1;
	} else if(rank == 0) {
		to_rank = rank+1;
		from_rank = size-1;
	} else {
		to_rank = rank+1;
		from_rank = rank-1;
	}

	int recv_message[size];
	if(rank%2 == 0) {
		MPI_Send(send_message, msg_size, MPI_INT, to_rank, rank, MPI_COMM_WORLD);
	} else {
		MPI_Recv(recv_message, msg_size-2, MPI_INT, from_rank, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if(rank%2 == 0) {
		MPI_Recv(recv_message, msg_size, MPI_INT, from_rank, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	} else {
		MPI_Send(send_message, msg_size, MPI_INT, to_rank, rank, MPI_COMM_WORLD);
	}

	printf("On rank %i, the sent message is %i and the received message is %i\n",
			rank, send_message[0], recv_message[0]);

	MPI_Finalize();
}
