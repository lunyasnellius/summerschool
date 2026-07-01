// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

// Main solver routines for heat equation solver

#include <mpi.h>

#include "heat.hpp"

// Exchange the boundary values
void exchange(Field& field, const ParallelData parallel, MPI_Request* requests)
{
	double* sbuf1;
	double* rbuf1;
	double* sbuf2;
	double* rbuf2;
	double* sbuf3;
	double* rbuf3;
	double* sbuf4;
	double* rbuf4;

	// using the nx and ny values of the full matrix,
	// including the ghost layers!!
	// confusion from: field.nx = field.temperature.nx-1
	int nx = field.temperature.nx;
	int ny = field.temperature.ny;

	int sendcountx = field.nx_full;
	int sendcounty = field.ny_full;

	if(parallel.size > 1) {
	sbuf1 = field.temperature.data(1,1);
	rbuf1 = field.temperature.data(nx-1,1);
	sbuf2 = field.temperature.data(nx-2,1);
	rbuf2 = field.temperature.data(0,1);

	// halo exchange:
	// utilizing the data() method of the Matrix class to obtain pointer
	// to element, e.g. field.temperature.data(i, j)
	// Send to up, receive from down
	MPI_Isend(sbuf1, sendcounty, MPI_DOUBLE, parallel.nup, parallel.rank,
		     parallel.cart_comm, &requests[0]);
	MPI_Irecv(rbuf1, sendcounty, MPI_DOUBLE, parallel.ndown, parallel.ndown,
		     parallel.cart_comm, &requests[1]);

	// Send to down, receive from up
	MPI_Isend(sbuf2, sendcounty, MPI_DOUBLE, parallel.ndown, parallel.rank,
		     parallel.cart_comm, &requests[2]);
	MPI_Irecv(rbuf2, sendcounty, MPI_DOUBLE, parallel.nup, parallel.nup,
		     parallel.cart_comm, &requests[3]);

	if(parallel.nleft != 0) {
		sbuf3 = field.temperature.data(1,1);
		rbuf3 = field.temperature.data(1,ny-1);
		sbuf4 = field.temperature.data(nx-2,1);
		rbuf4 = field.temperature.data(0,1);

	}
	}
}

void wait(MPI_Request* request)
{
	MPI_Waitall(4, request, MPI_STATUS_IGNORE);
}

// Update the temperature values using five-point stencil 
void evolve(Field& curr, const Field& prev, const double a, const double dt)
{
	// using the nx and ny values of the full matrix,
	// including the ghost layers!!
	// confusion from: field.nx = field.temperature.nx-1
	int nx = curr.temperature.nx;
	int ny = curr.temperature.ny;

  // Compilers do not necessarily optimize division to multiplication, so make it explicit
  auto inv_dx2 = 1.0 / (prev.dx * prev.dx);
  auto inv_dy2 = 1.0 / (prev.dy * prev.dy);

  // Determine the temperature field at next time step
  // As we have fixed boundary conditions, the outermost gridpoints
  // are not updated.
  // BULK COMP!!
  for (int i = 2; i < nx - 2; i++) {
	for (int j = 1; j < ny - 1; j++) {
			curr(i, j) = prev(i, j) + a * dt * (
		   ( prev(i + 1, j) - 2.0 * prev(i, j) + prev(i - 1, j) ) * inv_dx2 +
		   ( prev(i, j + 1) - 2.0 * prev(i, j) + prev(i, j - 1) ) * inv_dy2 );
	}
  }

}
void evolve_surface(Field& curr, const Field& prev, const double a, const double dt)
{
	// using the nx and ny values of the full matrix,
	// including the ghost layers!!
	// confusion from: field.nx = field.temperature.nx-1
	int nx = curr.temperature.nx;
	int ny = curr.temperature.ny;

  // Compilers do not necessarily optimize division to multiplication, so make it explicit
  auto inv_dx2 = 1.0 / (prev.dx * prev.dx);
  auto inv_dy2 = 1.0 / (prev.dy * prev.dy);

  // Determine the temperature field at next time step
  // As we have fixed boundary conditions, the outermost gridpoints
  // are not updated.
  // SURFACE COMP!!
  for (int i = 1; i < nx - 1; i += (nx - 3)) {
	for (int j = 1; j < ny - 1; j++) {
			curr(i, j) = prev(i, j) + a * dt * (
		   ( prev(i + 1, j) - 2.0 * prev(i, j) + prev(i - 1, j) ) * inv_dx2 +
		   ( prev(i, j + 1) - 2.0 * prev(i, j) + prev(i, j - 1) ) * inv_dy2 );
	}
  }

}

