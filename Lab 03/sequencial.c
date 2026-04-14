#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO 100000000 // qtd elementos
#define VEZES 1 // Número de vezes que o cálculo será repetido

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
	double resultado = 0.0;

	if (vecA == NULL || vecB == NULL) {
		printf("Erro ao alocar memória.\n");
		return 1;
	}

	// 2. Inicialização dos vetores
	// Usaremos valores previsíveis para facilitar a verificação:
	// vecA[i] = 1.0 e vecB[i] = 2.0. O produto interno deve ser TAMANHO * 2.0
	for (int i = 0; i < TAMANHO; i++) {
		vecA[i] = 1.0;
		vecB[i] = 2.0;
	}

	printf("Iniciando cálculo para %d elementos...\n", TAMANHO);

	// 3. Medição e Cálculo do Produto Interno
	double inicio = tempoAtual();
	// Para aumentar o tempo total de execução do programa
	  resultado = 0.0;
	  for (int i = 0; i < TAMANHO; i++)
		resultado += vecA[i] * vecB[i];

	double fim = tempoAtual();

	free(vecA);
    	free(vecB);


	// 4. Verificação do resultado
	// Como sabemos que 1.0 * 2.0 = 2.0, a soma total deve ser 2.0 * TAMANHO
	double esperado = (double)TAMANHO * 2.0;

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
