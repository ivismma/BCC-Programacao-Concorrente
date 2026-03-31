/*  Atividade 5: Implementação Prática

Desafio: Implementar um programa concorrente com M threads 
para somar 1 a cada elemento de um vetor de $N$ elementos.

Requisitos: Divisão balanceada de carga entre as threads. M e 
N passados como argumentos de linha de comando. Implementar 
funções de inicialização e verificação de resultados.

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct{
	int start;
	int finish;
	int remainingIndexOffset;
	// remainingIndexOffset é o offset p/ subtracao do fim do
	// vetor p/ compensar os índices n contemplados pela distribuição.
	// -1 é pq não há nenhum a ser trabalhado por aquela thread.
} t_params;

int* v;
int arraySize;

pthread_t *thread;
t_params* thread_params;

void* task(void *arg);
void showArray(int *v, int size);

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Invalid arguments\n");		
		exit(EXIT_FAILURE);
	}
	
	int n_threads = atoi(argv[1]); // M (nº threads)
	arraySize = atoi(argv[2]);     // N (elementos vetor)
	
	if((thread = (pthread_t *) malloc(n_threads*sizeof(pthread_t))) == NULL)
		exit(2);
	if((thread_params = (t_params *) malloc(n_threads*sizeof(t_params))) == NULL)
		exit(3);
	if((v = (int *) calloc(arraySize, sizeof(int))) == NULL)
		exit(4);
	
	int workSize = arraySize / n_threads;   // parcela de cada thread
	int remainder = arraySize % n_threads;  // restante a ser dividido pelas threads
	
	if(remainder == 0){
		for(int i = 0; i < n_threads; ++i){
			thread_params[i].start = workSize*i;
			thread_params[i].finish = workSize*(i+1)-1;
			thread_params[i].remainingIndexOffset = -1;		
		}
	}
	else{
		for(int i = 0; i < n_threads; ++i){
			thread_params[i].start = workSize*i;
			thread_params[i].finish = workSize*(i+1)-1;
		
		if(remainder-- > 0) // se ainda tem resto, dá o offset dos elem. restantes p/ as threads finalizarem
			thread_params[i].remainingIndexOffset = i;
		else
			thread_params[i].remainingIndexOffset = -1;
		}
	}
		
	// clock_t start = clock();

	// criacao das threads	
	for(int i = 0; i < n_threads; ++i)
		pthread_create(&thread[i], NULL, (void *) task, (void*) &thread_params[i]);
	
	// espera pelas threads
	for(int i = 0; i < n_threads; ++i)
		pthread_join(thread[i], NULL);
	
	// clock_t end = clock();
	// double diff = ((double) (end-start) / CLOCKS_PER_SEC);
	// printf("tempo exec.: %fs\n", diff);

	free(thread);
	free(thread_params);
	
	showArray(v, arraySize);
	free(v);

	return 0;
}

void* task(void *arg){
	t_params* params = (t_params *) arg;
	
	for(int i = params->start; i <= params->finish; ++i)
		++v[i];

	if(params->remainingIndexOffset != -1)
		++v[arraySize-params->remainingIndexOffset-1];

	return NULL;
}





void showArray(int *v, int size){
	for(int i = 0; i < size; ++i)
		printf("%d ", v[i]);
		
	printf("\n");
}

