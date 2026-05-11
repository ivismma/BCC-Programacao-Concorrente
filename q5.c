/*
Atividade 5
Objetivo: Alterar o programa soma-lock-atom.c (do Lab 4). A thread ExecutaTarefa soma valores. A cada múltiplo de 1000, ela deve pausar e esperar uma thread Extra imprimir o valor atual.

Requisito do Código:
- Thread ExecutaTarefa: Soma números. Se soma % 1000 == 0, sinaliza a thread Extra e dorme.
- Thread Extra: Dorme até receber sinal. Imprime a soma. Sinaliza ExecutaTarefa para continuar.
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

long int soma = 0; //variavel compartilhada entre as threads

bool imprimir = false;  // Thread ExecutaTarefa sinaliza Thread Extra
bool impresso = false; // Thread Extra sinaliza ExecutaTarefa

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //variavel de lock para exclusao mutua

pthread_cond_t condExecuta = PTHREAD_COND_INITIALIZER;
pthread_cond_t condExtra = PTHREAD_COND_INITIALIZER;

//funcao executada pelas threads
void *ExecutaTarefa (void *arg) {
	int id = *(int *) arg;
	printf("Thread : %d esta executando...\n", id);

	while(1) {
		pthread_mutex_lock(&mutex);
		soma++; 
		
		if(soma%1000 == 0){
			imprimir = true;
			pthread_cond_signal(&condExtra);
			
			while(!impresso){
				pthread_cond_wait(&condExecuta, &mutex);
			}
			if(soma == 100000){
				pthread_mutex_unlock(&mutex);
				break;
			}
			impresso = false;
		}
		
		pthread_mutex_unlock(&mutex);
	}
	printf("Thread : %d terminou!\n", id);
	pthread_exit(NULL);
}

//funcao executada pela thread de log
void *extra (void *args) {
	printf("Extra : esta executando...\n");
	
	while(1){
		pthread_mutex_lock(&mutex);
		
		while(!imprimir){
			pthread_cond_wait(&condExtra, &mutex);
		}
		printf("%ld\n", soma);
		imprimir = false;
		impresso = true;
		
		if(soma == 100000){
			pthread_mutex_unlock(&mutex);
			pthread_cond_broadcast(&condExecuta);
			break;
		}
		else
			pthread_cond_signal(&condExecuta);
			
		pthread_mutex_unlock(&mutex);
	}
	
	printf("Thread extra terminou\n");
	pthread_exit(NULL);
}

//fluxo principal
int main(int argc, char *argv[]) {
	pthread_t *tid; //identificadores das threads no sistema
	int nthreads; //qtde de threads (passada linha de comando)

	//--le e avalia os parametros de entrada
	if(argc<2) {
		printf("Digite: %s <numero de threads>\n", argv[0]);
		return 1;
	}
	nthreads = atoi(argv[1]);
	int id[nthreads];

	//--aloca as estruturas
	tid = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads+1));
	if(tid==NULL) {puts("ERRO--malloc"); return 2;}

	//--inicilaiza o mutex (lock de exclusao mutua)
	pthread_mutex_init(&mutex, NULL);

	//--cria as threads
	for(int t=0; t<nthreads; t++) {
		id[t] = t;
		if (pthread_create(&tid[t], NULL, ExecutaTarefa, &id[t])) {
			printf("--ERRO: pthread_create()\n"); exit(-1);
		}
	}

	//--cria thread de log
	if (pthread_create(&tid[nthreads], NULL, extra, NULL)) {
		printf("--ERRO: pthread_create()\n"); exit(-1);
	}

	//--espera todas as threads terminarem
	for (int t=0; t<nthreads+1; t++) {
		if (pthread_join(tid[t], NULL)) {
			printf("--ERRO: pthread_join() \n"); exit(-1);
		}
	}

	//--finaliza o mutex
	pthread_mutex_destroy(&mutex);

	printf("Valor de 'soma' = %ld\n", soma);

	return 0;
}
