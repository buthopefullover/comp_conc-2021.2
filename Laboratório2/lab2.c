#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_RND 10 //máximo elemento possível do vetor
#define MIN_RND 0 //mínimo elemento possível do vetor

float *matrix1; //matriz multiplicadora
float *matrix2; //matriz multiplicadora 
float *result; //matriz de saida

void mulMatrix (int dim){
    int i, j, k;
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            result[i*dim + j] = 0;
            for (k = 0; k < dim; k++)
                result[i*dim + j] += matrix1[i*dim + k] * matrix2[k*dim + j];
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

    if (argc<2) {
        printf ("Digite: %s <dimensão das matrizes>", argv[0]);
        return 1;
    }

    //Pega os argumentos da linha de comando
    dim = atoi(argv[1]);
    
    //Alocacao de memoria para as matrizes
    matrix1 = (float *) errorcheck_malloc(sizeof(float) * dim * dim);
    matrix2 = (float *) errorcheck_malloc(sizeof(float) * dim * dim);
    result = (float *) errorcheck_malloc(sizeof(float) * dim * dim);


    // Inicializa as matrizes aleatoriamente
    init_matrix(matrix1, dim);
    init_matrix(matrix2, dim);

    
    mulMatrix(dim);

    printf("Matriz resultante: \n");
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++)
            printf("%f ", result[i*dim + j]);
        printf("\n");
    }
}