// Atividade 6

/*
Implementar um programa concorrente com M threads para
somar 1 a cada elemento de um vetor de N elementos.

Calcular a soma de todos os elementos do vetor, antes e
depois da adição, e retornar os valores parciais para o
programa principal.

Receber os parâmetros no programa principal, totalizar e
verificar se houve algum erro de execução.

Requisitos:
- M e N passados como argumentos de linha de comando.
- Divisão balanceada de carga.
- O programa deve ter duas versês: na primeira divisão do 
vetor deve ser feita em bloco e na segunda deve ser feita 
alternadamente, como apresentado em sala de aula.
- Implementar funções de inicialização e verificação de resultados.
Executar o programa com o maior número possível de threads e
verificar o ganho final obtido. Utilize as rotinas.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct{
	int *v;
	int workSize; // parcela de cada thread 
	int start; // índice de início dessa thread
	int end;  // índice final da thread
	int remainingIndex; // eventuais índices restantes para dividir entre as threads
} ThreadArgs;

void* task(void *arg);


int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Wrong arguments\n");
		exit(EXIT_FAILURE);
	}
	
	int n_threads = atoi(argv[1]);
	int N = atoi(argv[2]);
	
	int ws = N / n_threads;
	int remainder = N % n_threads;	

	int* v = (int *) calloc(N, sizeof(int));
	pthread_t* worker = (pthread_t *) malloc(n_threads*sizeof(pthread_t)); 
	ThreadArgs* targs = (ThreadArgs *) malloc(n_threads*sizeof(ThreadArgs));
	
	// inicialização de argumentos + criação das threads:
	for(int i = 0; i < n_threads; ++i){
		targs[i].v = v;
		targs[i].workSize = ws;
		targs[i].start = i*ws;
		targs[i].end = (i+1)*ws-1;
		if(remainder-- > 0) 
			targs[i].remainingIndex = N-1+i;
		else
			targs[i].remainingIndex = -1;

		pthread_create(&worker[i], NULL, (void *) task, &targs[i]);
	}
	
	for(int i = 0; i < n_threads; ++i)
		pthread_join(worker[i], NULL);
	
	// verif. do vetor 
	for(int i = 0; i < N; ++i)
		if(v[i] != 1) return 2; // não somou corretamente.
	
	free(v);
	free(worker);
	free(targs);
	return 0;
}


void* task(void *arg){
	ThreadArgs ta = *(ThreadArgs *) arg;	

	for(int i = ta.start; i <= ta.end; ++i){
		ta.v[i]++;
	}
	if(ta.remainingIndex != -1)
		ta.v[ta.remainingIndex]++;

	return NULL;
}
