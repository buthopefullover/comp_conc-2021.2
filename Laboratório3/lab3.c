#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

#define MAX_RND 10 //máximo elemento possível do vetor
#define MIN_RND 0 //mínimo elemento possível do vetor

long long int N; //quantidade de elementos
int qnt_valores; //quantidade de valores

//funcao para inicializar o vetor de forma aleatória com inteiro, nesse caso, de 0 até 10
void init_vector(float* vector, int dim) {
    srand(time(0));
    for(int i = 0; i < dim; i++) {
        vector[i] = rand() % (MAX_RND + 1 - MIN_RND) + MIN_RND;
    }
}

//funcao para tratar erros com o malloc
void *errorcheck_malloc(int size) {
    void *ptr = malloc(size);
    if(ptr == NULL) {
        printf("--ERRO: malloc()\n");
        exit(-1);
    }
    return ptr;
}

int main(int argc, char *argv[]) {
    float * vector;
    int sLim;
    int iLim;

    
    if(argc < 2) {
        printf("Digite: %s <dimensão do vetor>\n", argv[0]);
        return 1;
    }

    //pega o parametros da linha de comando
    N = atoll(argv[1]);

    //alocacao de memoria para as vetor
    vector = (float *) errorcheck_malloc(sizeof(float) * N);

    //inicializa o vetor com valores aleatorios
    init_vector(vector, N);

    printf("Digite: <limiar inferior> <limiar superior>\n");
    scanf("%d %d", &iLim, &sLim);



}