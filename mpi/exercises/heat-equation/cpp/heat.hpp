// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#pragma once
#include <string>
#include "matrix.hpp"
#include <mpi.h>

// Class for basic parallelization information
struct ParallelData {
    int size;            // Number of MPI tasks
    int rank;
    int nup, ndown;      // Ranks of neighbouring MPI tasks
	MPI_Comm cart_comm;
	int nleft=0, nright=0;

    ParallelData() {      // Constructor
      // start: query number of MPI tasks and store it in
      // the size attribute of the class
	    MPI_Comm_size(MPI_COMM_WORLD, &size);

	    constexpr int n_dims = 1;	// number of dims
	    int dims[n_dims] = {0};	// MPI splits tasks onto the number of dims
	    int periods[n_dims] = {1};	// true = 1 = periodic boundary 
					// false = 0 = not periodic

	    MPI_Dims_create(size, n_dims, dims);
	    MPI_Cart_create(MPI_COMM_WORLD, n_dims, dims, periods, 
			    1, &cart_comm); 	// bool = 1 = MPI can reorder tasks
      // Query MPI rank of this task and store it in the rank attribute
	    MPI_Comm_rank(cart_comm, &rank);

      // Determine up and down neighbours of this domain
	    MPI_Cart_shift(cart_comm, 0, 1, &nup, &ndown);

    };

    ParallelData(int in_dims) {      // Constructor
      // start: query number of MPI tasks and store it in
      // the size attribute of the class
	    MPI_Comm_size(MPI_COMM_WORLD, &size);

	    constexpr int n_dims = in_dims;	// number of dims
	    int dims[n_dims];			// MPI splits tasks onto the number of dims
	    int periods[n_dims];		// true = 1 = periodic boundary 
						// false = 0 = not periodic

	    for (int i=0; i<n_dims; ++i) periods[i]=1;

	    MPI_Dims_create(size, n_dims, dims);
	    MPI_Cart_create(MPI_COMM_WORLD, n_dims, dims, periods, 
			    1, &cart_comm); 	// bool = 1 = MPI can reorder tasks
      // Query MPI rank of this task and store it in the rank attribute
	    MPI_Comm_rank(cart_comm, &rank);

      // Determine up and down neighbours of this domain
	    MPI_Cart_shift(cart_comm, 0, 1, &nup, &ndown);
	    MPI_Cart_shift(cart_comm, 1, 1, &nleft, &nright);

    };

};

// Class for temperature field
struct Field {
    // nx and ny are the true dimensions of the field. The temperature matrix
    // contains also ghost layers, so it will have dimensions nx+2 x ny+2
    int nx;                     // Local dimensions of the field
    int ny;
    int nx_full;                // Global dimensions of the field
    int ny_full;                // Global dimensions of the field
    double dx = 0.01;           // Grid spacing
    double dy = 0.01;

    Matrix<double> temperature;

    void setup(int nx_in, int ny_in, ParallelData parallel);

    void generate(ParallelData parallel);

    // standard (i,j) syntax for setting elements
    double& operator()(int i, int j) {return temperature(i, j);}

    // standard (i,j) syntax for getting elements
    const double& operator()(int i, int j) const {return temperature(i, j);}

};

// Function declarations
void initialize(int argc, char *argv[], Field& current,
                Field& previous, int& nsteps, ParallelData parallel);

void exchange(Field& field, const ParallelData parallel, MPI_Request* requests);

void wait(MPI_Request* request);

void evolve(Field& curr, const Field& prev, const double a, const double dt);

void evolve_surface(Field& curr, const Field& prev, const double a, const double dt);

void write_field(const Field& field, const int iter, const ParallelData parallel);

void read_field(Field& field, std::string filename,
                const ParallelData parallel);

double average(const Field& field, const ParallelData parallel);
