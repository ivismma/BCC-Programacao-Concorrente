/*
14. Implementar uma rotina para emular o comportamento de uma barreira para 3 threads
utilizando apenas um Mutex e uma Variável de Condição (sem usar pthread_barrier_t).
A última thread a chegar deve dar o sinal para todas as outras. (1,0 ponto)
*/

#include <stdio.h>
#include <pthread.h>

#define N_THREADS 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int count = 0;

void* task(void *arg);

int main(void){
	pthread_t worker[N_THREADS];
	
	for(int i = 0; i < N_THREADS; ++i)
		pthread_create(&worker[i], NULL, (void *) task, NULL);
		
	for(int i = 0; i < N_THREADS; ++i)
		pthread_join(worker[i], NULL);

	return 0;
}

void* task(void *arg){
	printf("Efetuando alguma coisa...\n");
	
	pthread_mutex_lock(&mutex);
	while(count < 3){
		if(++count != 3)
			pthread_cond_wait(&cond, &mutex);
		else
			pthread_cond_broadcast(&cond);
	}
	pthread_mutex_unlock(&mutex);
	
	printf("Efetuando alguma coisa DEPOIS da barreira\n");
	return NULL;
}
