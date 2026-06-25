#include <mpi.h>
#include <iostream>
#include <vector>


void init_buf(std::vector<int> &buffer);
void print_buf(std::vector<int> &buffer);

using buf_t = std::vector<int>;

int main(int argc, char ** argv) {
	MPI_Init(&argc, &argv);

	int g_size, g_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &g_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &g_rank);

	int bufsize = 8;
	buf_t buffer(bufsize);
	buf_t rbuffer(bufsize, -1);
	init_buf(buffer);

	int color;
	if(g_rank < 2) {
		color = 1;
	} else {
		color = 2;
	}

	MPI_Comm subcomm;
	MPI_Comm_split(MPI_COMM_WORLD, color, g_rank, &subcomm);

	int c_rank;
	MPI_Comm_rank(subcomm, &c_rank);

	MPI_Reduce(buffer.data(), rbuffer.data(), bufsize, MPI_INT, MPI_SUM, 0, subcomm);

	print_buf(rbuffer);

	MPI_Finalize();
}// end of main


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

