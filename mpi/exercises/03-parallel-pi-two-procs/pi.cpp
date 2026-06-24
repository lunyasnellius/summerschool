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

  printf("Computing approximation to pi with N=%d\n", n);

  int istart = 1;
  int ihalf = n/2;
  int istop = n;

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double pi = 0.0;
  if(rank == 0) {
    for (int i=istart; i<ihalf; i++) {
      double x = (i - 0.5) / n;
      pi += 1.0 / (1.0 + x*x);
    }
    MPI_Send(&pi, 1, MPI_DOUBLE, 1, 42, MPI_COMM_WORLD);
  } else if(rank == 1) {
    for (int i=ihalf; i<=istop; ++i) {
      double x = (i - 0.5) / n;
      pi += 1.0 / (1.0 + x*x);
    }
    double pipart = 0.0;
    MPI_Recv(&pipart, 1, MPI_DOUBLE, 0, 42, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    pi += pipart;

    pi *= 4.0 / n;
    printf("Approximate pi=%18.16f (exact pi=%10.8f)\n", pi, M_PI);
  }

  MPI_Finalize();
}
