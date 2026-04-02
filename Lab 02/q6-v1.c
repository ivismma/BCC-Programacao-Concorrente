// Atividade 6 (Versão soma por divisão de blocos e
// passagem de ponteiro do vetor + offset)

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
	int workSize;
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
	
	int* v = (int *) malloc(N*sizeof(int));
	pthread_t* worker = (pthread_t *) malloc(n_threads*sizeof(pthread_t)); 
	ThreadArgs* targs = (ThreadArgs *) malloc(n_threads*sizeof(ThreadArgs));
	
	for(int i = 0; i < n_threads; ++i){
		targs[i].v = v + i*ws;
		targs[i].workSize = ws;
		pthread_create(&worker[i], NULL, (void *) task, &targs[i]);
	}
	
	for(int i = 0; i < n_threads; ++i)
		pthread_join(worker[i], NULL);
	
	// verif. do vetor
	for(int i = 0; i < N; ++i)
		if(v[i] != 1) return 2;
	
	free(v);
	free(worker);
	free(targs);
	return 0;
}


void* task(void *arg){
	ThreadArgs ta = *(ThreadArgs *) arg;
	
	for(int i = 0; i < ta.workSize; ++i){
		ta.v[i]++;
	}

	return NULL;
}
