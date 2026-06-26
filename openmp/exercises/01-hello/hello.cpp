// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <omp.h>

int main()
{
    printf("Hello world!\n");

    int nums = 1;
    int threadnum = 1;
    #pragma omp parallel private(threadnum)
    {
#ifdef _OPENMP
	    nums = omp_get_num_threads();
	    threadnum = omp_get_thread_num();
#endif
	    printf("Hello from thread %i!\n", threadnum);
    }

    printf("Total number of threads = %i\n", nums);

    return 0;
}
