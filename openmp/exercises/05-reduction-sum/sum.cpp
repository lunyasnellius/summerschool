// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <cmath>
#include <string>
#include <omp.h>

int main(int argc, char* argv[])
{
    // Array size
    int n = 100000;
    if (argc > 1) {
        n = std::stoi(argv[1]);
    }
    printf("Array size: %d\n", n);

    // Start timing
    double t0 = omp_get_wtime();

    // Calculate sum
    double total1 = 0;
    #pragma omp parallel reduction(+:total1)
    #pragma omp for
    for (int i = 0; i < n; i++) {
        total1 += std::sin(static_cast<double>(i));
    }

    // End timing
    double t1 = omp_get_wtime();

    printf("Sum: %f\n", total1);
    printf("Calculation took %.3f milliseconds\n", (t1 - t0) * 1e3);

    double t2 = omp_get_wtime();

    // Calculate sum
    double total2 = 0;
    #pragma omp parallel
    #pragma omp for reduction(+:total2)
    for (int i = 0; i < n; i++) {
        total2 += std::sin(static_cast<double>(i));
    }

    double t3 = omp_get_wtime();

    printf("Sum: %f\n", total2);
    printf("Calculation took %.3f milliseconds\n", (t3 - t2) * 1e3);

    return 0;
}
