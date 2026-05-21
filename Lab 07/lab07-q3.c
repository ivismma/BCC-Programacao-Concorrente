#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
	int N = 1000000;
	double* A = malloc(sizeof(double) * N);
	double* soma_local;
	
	int cpu_count = omp_get_num_procs();
	int n_threads;
	
	soma_local = (double *) calloc(cpu_count, sizeof(double));
	
	for (int i = 0; i < N; i++){
		A[i] = 1.0;
	}
	
	double soma = 0.0; // global
	
	
	#pragma omp parallel
	{
		int tid = omp_get_thread_num();
		n_threads = omp_get_num_threads();
		
		for (int i = tid; i < N; i += n_threads){
			soma_local[tid] += A[i];
		}
	}
	
	for(int i = 0; i < n_threads; ++i)
		soma += soma_local[i];
	
	printf("Soma: %.0f\n", soma);
	
	free(A);
	free(soma_local);
	return 0;
}
