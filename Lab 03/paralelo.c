#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define TAMANHO 100000000 // qtd elementos
#define NUM_THREADS 4

typedef struct{
	double *vecA;
	double *vecB;
	int start;
	int end;
	double resultado;
	int remainingIndex;
} ThreadArgs;


void* task(void* arg){
	ThreadArgs* ta = (ThreadArgs *) arg;
	double soma = 0;
	
	for(int i = ta->start; i <= ta->end; ++i)
		soma += ta->vecA[i]*ta->vecA[i];
		
	// se teve resto, computa.
	if(ta->remainingIndex != -1)
		soma += ta->vecA[ta->remainingIndex]*ta->vecB[ta->remainingIndex];
	
	ta->resultado = soma;
	
	return NULL;
}

// Rotina para medir o tempo fornecida
double tempoAtual() {
	struct timespec tempo;
	clock_gettime(CLOCK_MONOTONIC_RAW, &tempo);
	return tempo.tv_sec + tempo.tv_nsec / 1000000000.0;
	
	/* Fórmula: 
	
	Tempo atual = tv_sec + tv_nsec
	                       -------
	                         10⁹
	
	tv_sec:  Segundos (parte inteira)
	tv_nsec: Segundos em nanossegundos (parte decimal)
	
	De referência desde 01/01/1970
	*/
}

int main() {
	// 1. Alocação de memória para os vetores
	double *vecA = (double *)malloc(TAMANHO * sizeof(double));
	double *vecB = (double *)malloc(TAMANHO * sizeof(double));

	if (vecA == NULL || vecB == NULL) {
		printf("Erro ao alocar memória.\n");
		return 1;
	}

	int workSize = TAMANHO / NUM_THREADS;
	int remainder = TAMANHO % NUM_THREADS;

	pthread_t thread[NUM_THREADS];
	ThreadArgs ta[NUM_THREADS];
	
	for(int i = 0; i < NUM_THREADS; ++i){
		ta[i].vecA = vecA;
		ta[i].vecB = vecB;
		ta[i].start = i*workSize;
		ta[i].end = (i+1)*workSize-1;
		ta[i].remainingIndex = (i < remainder)? i : -1;
		ta[i].resultado = 0;
	}

	// 2. Inicialização dos vetores
	// usaremos valores previsíveis para facilitar a verificação:
	// vecA[i] = 1.0 e vecB[i] = 2.0. O produto interno deve ser TAMANHO * 2.0
	for (int i = 0; i < TAMANHO; i++) {
		vecA[i] = 1.0;
		vecB[i] = 2.0;
	}

	printf("Iniciando cálculo para %d elementos...\n", TAMANHO);

	// 3. Medição e Cálculo do Produto Interno
	double inicio = tempoAtual();

	for(int i = 0; i < NUM_THREADS; ++i)
		pthread_create(&thread[i], NULL, (void *) task, &ta[i]);

	for(int i = 0; i < NUM_THREADS; ++i)
		pthread_join(thread[i], NULL);

	free(vecA);
	free(vecB);

	double fim = tempoAtual();

	// 4. Verificação do resultado
	double resultado = 0;
	for(int i = 0; i < NUM_THREADS; ++i)
		resultado += ta[i].resultado;
    
	double esperado = (double) TAMANHO;

	printf("--- Resultados ---\n");

	printf("Tempo de Execução: %.6f segundos\n", fim - inicio);
	printf("Resultado Obtido:  %.2f\n", resultado);
	printf("Resultado Esperado: %.2f\n", esperado);

	if (resultado == esperado)
	  printf("Verificação: SUCESSO (O resultado está correto).\n");
	else
	  printf("Verificação: FALHA (Diferença detectada).\n");

    	return 0;
}

