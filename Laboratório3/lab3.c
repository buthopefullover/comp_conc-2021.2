/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 3 */
/* Codigo: Implementação concorrente, para encontrar a quantidade de valores de um vetor de numeros 
reais que estão dentro de uma faixa de valores dada. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

#define MAX_RND 10 //máximo elemento possível do vetor
#define MIN_RND 0 //mínimo elemento possível do vetor

long long int g_nElements; //quantidade de elementos
int g_nThreads; //quantidade de threads
float *g_vector; //vetor 

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread, limit, rangeInf, rangeSup;
} TArgs;

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

//funcao para contar sequencialmente o número de elementos dentro do limite
int countInRangeSeq (float* vector, int size, int rangeInf, int rangeSup){
    int counter = 0;

    //itera pelo vetor
    for (int i = 0; i<size; i++){
        //checa se o elemento esta dentro do limite, se estiver incrementa o contador
        if (vector[i] > rangeInf && vector[i] < rangeSup){
            counter += 1;
        }
    }
    return counter;
}

//funcao para contar concorrentemente o número de elementos dentro do limite
void *countInRangeConc(void* arg){
    TArgs *args = (TArgs *) arg;
    int *counter;

    counter = (int *) errorcheck_malloc(sizeof(int));

    int start = args->idThread*args->limit; //calcula a posição de inicio da operação, dependendo de qual thread esta sendo executada

    //verifica se estamos na última thread, caso esteja efetua a checagem até o final do vetor 
    //(para evitar erros com divisões com resto)
    if (args->idThread+1 == g_nThreads){
	    for (int i = start; i<g_nElements; i++){
            if (g_vector[i] > args->rangeInf && g_vector[i] < args->rangeSup){
                *counter += 1;
            }
        }
    }
    else{
        //caso não esteja na última thread faz a checagem até atingir a quantidade calculada anteriormente (divisions)
        for (int i = start; i <start+args->limit; i++){
            //checa se o elemento esta dentro do limite, se estiver incrementa o contador
		    if (g_vector[i] > args->rangeInf && g_vector[i] < args->rangeSup){
                *counter += 1;
            }
	    }
    }
    printf("counter %d\n", *counter);
    pthread_exit((void *) counter);  
}

int main(int argc, char *argv[]) {
    int isCorrect = 0; //bool para checar se operações estão corretas
    int sLim; //receberá o limite superior
    int iLim; //receberá o limite inferior
    int counterConc = 0; //receberá o resultando da contagem concorrente
    int counterSeq = 0; //receberá o resultado da contagem sequencial
    int *tidLocal; //identificadores locais das threads
    pthread_t *tidSistema; //identificadores das threads no sistema
    TArgs *arg; //receberá os argumentos para a thread
    int *ret; //ponteiro para pegar o retorno das threads
    int divisions; //quantos elementos cada thread operará em
    double start, end, totalSeq, totalConc; //variaveis auxiliares para o controle do tempo de execucao

    //checa se recebeu todos os argumentos necessários
    if(argc < 3) {
        printf("Digite: %s <dimensão do vetor> <número de threads>\n", argv[0]);
        return 1;
    }

    //pega os parametros da linha de comando
    g_nElements = atoll(argv[1]);
    g_nThreads = atoll(argv[2]);

    //calcula o limite das threads
    divisions = (g_nElements/g_nThreads)-1;

    //alocacao de memoria de variaveis auxiliares na concorrencia
    tidSistema  = (pthread_t *) errorcheck_malloc(sizeof(pthread_t) * g_nThreads);
    tidLocal  = (int *) errorcheck_malloc(sizeof(int) * g_nThreads);

    //alocacao de memoria para o vetor
    g_vector = (float *) errorcheck_malloc(sizeof(float) * g_nElements);

    //inicializa o vetor com valores aleatorios
    init_vector(g_vector, g_nElements);

    //pega os valores de limite da entrada padrão
    printf("Digite: <limiar inferior> <limiar superior>\n");
    scanf("%d %d", &iLim, &sLim);

    //realiza a contagem sequencial
    GET_TIME(start);
    counterSeq = countInRangeSeq(g_vector, g_nElements, iLim, sLim);
    GET_TIME(end);

    //calcula tempo de execucao sequencial
    totalSeq = end-start;

    GET_TIME(start);

    //cria as  threads
    for(int thread=0; thread<g_nThreads; thread++) {
        tidLocal[thread] = thread;

        arg = malloc(sizeof(TArgs));
        if (arg == NULL) {
            printf("--ERRO: malloc()\n"); exit(-1);
        }
        arg->idThread = thread; 
        arg->limit = divisions; 
        arg->rangeInf = iLim;
        arg->rangeSup = sLim;

        if (pthread_create(&tidSistema[thread], NULL, countInRangeConc, (void*) arg)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }
 
    //espera todas as threads terminarem
    for (int thread=0; thread<g_nThreads; thread++) {
        if (pthread_join(tidSistema[thread], (void**) &ret)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        }
        counterConc += *ret;

        free(ret);
    }
    GET_TIME(end);

    //calcula tempo de execucao concorrente
    totalConc = end - start;

    //compara o resultado das contagens, caso estejam iguais muda a variavel
    if (counterConc == counterSeq){
        isCorrect = 1;
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
    free(vector);

    pthread_exit(NULL);

}