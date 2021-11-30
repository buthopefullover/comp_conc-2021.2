#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

#define MAX_RND 10 //máximo elemento possível do vetor
#define MIN_RND 0 //mínimo elemento possível do vetor

float *matrix1; //matriz multiplicadora
float *matrix2; //matriz multiplicadora 
float *resultSeq; //matriz resultante sequencial
float *resultConc; //matriz resultante concorrente

typedef struct{
 int id, size, total_threads;
}TArgs;

void *mulMatrixConc(void *arg) {
    TArgs *args = (TArgs *) arg;

    //realiza a multiplicação concorrente intercalando
    //as linhas para cada thread
    for(int i = args->id; i < args->size; i += args->total_threads) {
        for(int j = 0; j < args->size; j++) {
            for(int k = 0; k < args->size; k++) {
                resultConc[i*args->size + j] += matrix1[i*args->size + k] * matrix2[k*args->size + j];
            }
        }
    }

    pthread_exit(NULL);
}

void mulMatrixSeq (float matrix1[], float matrix2[], float result[], int dim){
    int i, j, k;

    // realiza multiplicação sequencial das matrizes
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            result[i*dim + j] = 0;
            for (k = 0; k < dim; k++)
                result[i*dim + j] += matrix1[i*dim + k] * matrix2[k*dim + j];
        }
    }
}

//funcao para tratar erros com o malloc
void *errorcheck_malloc(int size) {
    void *ptr = malloc(size);
    if(ptr == NULL) {
        printf("-- Erro no malloc --\n");
        exit(-1);
    }
    return ptr;
}

//funcao para inicializar o vetor de forma aleatoria com float, nesse caso, de 0 até 10
void init_matrix(float matrix[], int dim) {
    srand(time(0));
    for(int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++){
            matrix[i*dim + j] = ((float)rand())/MAX_RND;
        }
    }
}

int main (int argc, char* argv[]){
    int dim;
    int nThreads;
    pthread_t *tidSistema; //identificadores das threads no sistema
    int thread; //variavel auxiliar
    int *tidLocal; //identificadores locais das threads
    TArgs *arg; //receberá os argumentos para a thread
    double start, end, totalSeq, totalConc; //variaveis auxiliares para o controle do tempo de execucao

    if (argc<3) {
        printf ("Digite: %s <dimensão das matrizes> <numero de threads>", argv[0]);
        return 1;
    }
    
    //armazena as entradas
    dim = atoi(argv[1]);
    nThreads = atoi(argv[2]);

    //evita criacao de threads desnecessarias 
    if(nThreads > dim) {
        nThreads = dim;
    }

    //alocacao de memoria de variaveis auxiliares na concorrencia
    tidSistema  = (pthread_t *) errorcheck_malloc(sizeof(pthread_t) * nThreads);
    tidLocal  = (int *) errorcheck_malloc(sizeof(int) * nThreads);
    
    //alocacao de memoria para as matrizes
    matrix1 = (float *) errorcheck_malloc(sizeof(float) * dim * dim);
    matrix2 = (float *) errorcheck_malloc(sizeof(float) * dim * dim);
    resultConc = (float *) errorcheck_malloc(sizeof(float) * dim * dim);
    resultSeq = (float *) errorcheck_malloc(sizeof(float) * dim * dim);


    //inicializa as matrizes aleatoriamente
    init_matrix(matrix1, dim);
    init_matrix(matrix2, dim);

    //realiza a multiplicacao sequencial
    GET_TIME(start);
    mulMatrixSeq(matrix1, matrix2, resultSeq, dim);
    GET_TIME(end);

    //calcula tempo de execucao sequencial
    totalSeq = end-start;

    GET_TIME(start);
    //cria as  threads para execucao concorrente
    for(thread=0; thread<nThreads; thread++) {
        tidLocal[thread] = thread;

        arg = (TArgs *) errorcheck_malloc(sizeof(TArgs) * dim * dim);

        arg->id = thread; 
        arg->size = dim;
        arg->total_threads = nThreads; 

        if (pthread_create(&tidSistema[thread], NULL, mulMatrixConc,  (void*) arg)) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    //espera todas as threads terminarem
    for (thread=0; thread<nThreads; thread++) {
        if (pthread_join(tidSistema[thread], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    }
    GET_TIME(end);

    //calcula tempo de execucao concorrente
    totalConc = end - start;

    //bool para checar se operações estão corretas
    int isCorrect = 1;

    //itera pelas matrizes comparando cada elemento entre si
    //caso algum não seja igual muda a variavel
    for(int i = 0; i < dim; i++) {
        for (int j=0; j < dim; j++)
            if(resultSeq[i*dim + j] != resultConc[i*dim + j]) {
                isCorrect = 0;
                break;
            }
    }

    //imprime se o resultado esta correto ou não
    printf("%s\n", isCorrect ? "Calculos corretos" : "Calculos errados");

    //imprime os tempo de execucoes
    printf("Tempo de execução sequencial: %f\n", totalSeq);
    printf("Tempo de execução concorrente: %f\n", totalConc);


    //libera os espaços alocados
    free(tidSistema);
    free(tidLocal);
    free(arg);
    free(matrix1);
    free(matrix2);
    free(resultConc);
    free(resultSeq);

    pthread_exit(NULL);
}