#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MAX_RND 10 //máximo elemento possível do vetor
#define MIN_RND 0 //mínimo elemento possível do vetor

float *matrix1; //matriz multiplicadora
float *matrix2; //matriz multiplicadora 
float *resultSeq; //matriz resultante sequencial
float *resultConc; //matriz resultante concorrente

typedef struct{
 int id;
 int size;
 int total_threads;
}TArgs;

void *mulMatrixConc(void *arg) {
    TArgs *args = (TArgs *) arg;

    // Realiza a multiplicação para cada linha do resultado
    // intercalando as linhas para cada thread
    for(int i = args->id; i < args->size; i += args->total_threads) {
        for(int j = 0; j < args->size; j++) {
            for(int k = 0; k < args->size; k++) {
                resultConc[i*args->size + j] += matrix1[i*args->size + k] * matrix2[k*args->size + j];
            }
        }
    }

    pthread_exit(NULL);
}

void mulMatrixSeq (int dim){
    int i, j, k;
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            resultSeq[i*dim + j] = 0;
            for (k = 0; k < dim; k++)
                resultSeq[i*dim + j] += matrix1[i*dim + k] * matrix2[k*dim + j];
        }
    }
}

// Função para tratar erros com o malloc
void *errorcheck_malloc(int size) {
    void *ptr = malloc(size);
    if(ptr == NULL) {
        printf("-- Erro no malloc --\n");
        exit(-1);
    }
    return ptr;
}

//funcao para inicializar o vetor de forma aleatória com inteiro, nesse caso, de 0 até 10
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
    int nthreads;
    pthread_t *tidSistema; //identificadores das threads no sistema
    int thread; //variavel auxiliar
    int *tidLocal; //identificadores locais das threads
    TArgs *arg; //receberá os argumentos para a thread

    if (argc<3) {
        printf ("Falta argumentos. Digite: %s <dimensão das matrizes> <numero de threads>", argv[0]);
        return 1;
    }

    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);

    if(nthreads > dim) {
        nthreads = dim;
    }

    tidSistema  = (pthread_t *) errorcheck_malloc(sizeof(pthread_t) * nthreads);
    tidLocal  = (int *) errorcheck_malloc(sizeof(int) * nthreads);
    
    //Alocacao de memoria para as matrizes
    matrix1 = (float *) errorcheck_malloc(sizeof(float) * dim * dim);
    matrix2 = (float *) errorcheck_malloc(sizeof(float) * dim * dim);
    resultConc = (float *) errorcheck_malloc(sizeof(float) * dim * dim);
    resultSeq = (float *) errorcheck_malloc(sizeof(float) * dim * dim);


    // Inicializa as matrizes aleatoriamente
    init_matrix(matrix1, dim);
    init_matrix(matrix2, dim);

    mulMatrixSeq(dim);

    //cria as  threads
    for(thread=0; thread<nthreads; thread++) {
        tidLocal[thread] = thread;

        arg = (TArgs *) errorcheck_malloc(sizeof(TArgs) * dim * dim);

        arg->id = thread; 
        arg->size = dim;
        arg->total_threads = nthreads; 

        if (pthread_create(&tidSistema[thread], NULL, mulMatrixConc,  (void*) arg)) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    //espera todas as threads terminarem
    for (thread=0; thread<nthreads; thread++) {
        if (pthread_join(tidSistema[thread], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    }

    //bool para checar se operações estão corretas
    int isCorrect = 1;

    //itera pelo vetor comparando cada elemento do vetor original elevado ao quadrado com o vetor modificado
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

    pthread_exit(NULL);
}