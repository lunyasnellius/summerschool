#include <mpi.h>
#include <iostream>
#include <vector>


void init_buf(std::vector<int> &buffer);
void print_buf(std::vector<int> &buffer);


int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);

	int b_size = 8;
	std::vector<int> sbuf(b_size);
	std::vector<int> case1(b_size, -1);
	std::vector<int> case2(b_size, -1);
	std::vector<int> case3(b_size, -1);
	std::vector<int> case4(b_size, -1);

	init_buf(sbuf);
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	print_buf(sbuf);
	/*
	 * Case 1:
	 */
	init_buf(case1);
	MPI_Bcast(case1.data(), case1.size(), MPI_INT, 0, MPI_COMM_WORLD);

	if(rank == 0) printf("Case 1:\n");
	print_buf(case1);

	/*
	 * Case 2:
	 */
	MPI_Scatter(sbuf.data(), 2, MPI_INT, case2.data(), 2, MPI_INT, 0, MPI_COMM_WORLD);

	if(rank == 0) printf("Case 2:\n");
	print_buf(case2);

	/*
	 * Case 3:
	 */
	int sendcount = (rank < 2 ? 1 : 2*(rank-1));
	int recvcount[4];
	int displ[4];
	recvcount[0] = 1;
	displ[0] = 0;
	for(int i=1; i<4; ++i) {
		recvcount[i] = (i < 2 ? 1 : 2*(i-1));
		displ[i] = recvcount[i];
	}
	
	MPI_Gatherv(sbuf.data(), sendcount, MPI_INT, 
		case3.data(), recvcount, displ, MPI_INT, 1, MPI_COMM_WORLD);

	if(rank == 0) printf("Case 3:\n");
	print_buf(case3);

	/*
	 * Case 4:
	 */
	MPI_Alltoall(sbuf.data(), 2, MPI_INT, case4.data(), 2, MPI_INT, MPI_COMM_WORLD);

	if(rank == 0) printf("Case 4:\n");
	print_buf(case4);

	MPI_Finalize();
}

void init_buf(std::vector<int> &buffer) {
	int buf_size = buffer.size();
	int size, rank;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	for (int i=0; i<buf_size; ++i) buffer[i] = i + rank * buf_size;
}

void print_buf(std::vector<int> &buffer) {
	int buf_size = buffer.size();
	int size, rank;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	std::vector<int> prnt_buf(size*buf_size);

	MPI_Gather(buffer.data(), buf_size, MPI_INT, prnt_buf.data(), buf_size, 
		MPI_INT, 0, MPI_COMM_WORLD);

	if(rank == 0) {
		int prnt_x = (size > 6 ? 6 : size);
		int prnt_y = (buf_size > 10 ? 10 : buf_size);
		for(int i=0; i<prnt_x; ++i) {
			printf("rank %i: ",i);
			for(int j=0; j<prnt_y; ++j) {
				printf("%i ", prnt_buf[j + i*buf_size]);
			}
			if(buf_size > 10) printf("...");
			printf("\n");
		}
		if(size > 6) printf(".\n.\n.\n");
		printf("\n");
	}

}

