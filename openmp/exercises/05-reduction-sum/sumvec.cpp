#include <omp.h>
#include <iostream>
#include <vector>

using vec = std::vector<int>;


int main(int argc, char** argv) {

	vec vec1(6,2);
	vec vec2(6,4);
	int out = 0;

	printf("Beginning val: %i\n", out);

#pragma omp parallel reduction(+:out)
	{
#pragma omp for
	for(int i=0; i<6; ++i) {
		out += vec1[i]*vec2[i];
	}
	}

	printf("End val: %i\n", out);

	return 0;
}

