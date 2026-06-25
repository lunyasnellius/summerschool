// SPDX-FileCopyrightText: 2019 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <cmath>
#include <mpi.h>

constexpr int n = 840;

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);


  int istart = 1;
  int istop = n;

  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int ipart = n/size;

  istart += rank*ipart;
  istop = istart + ipart;

  if(n%size != 0 && rank == size-1) {
    istop = n+1;
  }

  double pi = 0.0;
  for (int i=istart; i<istop; i++) {
    double x = (i - 0.5) / n;
    pi += 1.0 / (1.0 + x*x);
  }
  double fullpi = 0.0;
  MPI_Reduce(&pi, &fullpi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if(rank == 0) {
    pi = fullpi * 4.0 / n;
    printf("Approximate pi=%18.16f (exact pi=%10.8f)\n", pi, M_PI);
  }

  MPI_Finalize();
}
