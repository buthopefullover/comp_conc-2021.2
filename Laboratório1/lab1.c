/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: Implementação concorrente, com duas threads (alem da thread 
principal), para elevar ao quadrado cada elemento de um vetor de 10000 elementos. */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  2 //total de threads a serem criadas
#define NELEMENTS 1000 //total de elementos no vetor
#define MAX_RND 10 //máximo elemento possível do vetor
#define MIN_RND 0 //mínimo elemento possível do vetor

int g_pwrVector[NELEMENTS]; //vetor que será modificado, inicialmente identico ao original

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread, limit;
} TArgs;


//funcao para inicializar o vetor de forma aleatória com inteiro, nesse caso, de 0 até 10
void init_vector(int* vector) {
    srand(time(0));
    for(int i = 0; i < NELEMENTS; i++) {
        vector[i] = rand() % (MAX_RND + 1 - MIN_RND) + MIN_RND;
        g_pwrVector[i] = vector[i]; //copia cada elemento do vetor original no vetor que modificaremos
    }
}

//funcao executada pelas threads que efetua a elevação ao quadrado dos elementos do vetor
void *pwr2 (void* arg) {
    TArgs *args = (TArgs *) arg;

    int start = args->idThread*args->limit; //calcula a posição de inicio da operação, dependendo de qual thread esta sendo executada

    //verifica se estamos na última thread, caso esteja efetua a operação até o final do vetor 
    //(para evitar erros com divisões com resto)
    if (args->idThread+1 == NTHREADS){
	    for (int i = start; i<NELEMENTS; i++){
            g_pwrVector[i] = g_pwrVector[i] * g_pwrVector[i];
        }
    }
    else{
        //caso não esteja na última thread faz as operações nos elementos até atingir a quantidade calculada anteriormente (divisions)
        for (int i = start; i <start+args->limit; i++){
		    g_pwrVector[i] = g_pwrVector[i] * g_pwrVector[i];
	    }
    }
    pthread_exit(NULL);                                                                       
}

//funcao principal do programa
int main() {
    pthread_t tidSistema[NTHREADS]; //identificadores das threads no sistema
    int thread; //variavel auxiliar
    int tidLocal[NTHREADS]; //identificadores locais das threads
    int divisions = (NELEMENTS/NTHREADS)-1; //quantos elementos cada thread operará em
    TArgs *arg; //receberá os argumentos para a thread
    int vector[NELEMENTS]; //vetor original
    
    init_vector(vector); //inicializa o vetor

    //cria as  threads
    for(thread=0; thread<NTHREADS; thread++) {
        tidLocal[thread] = thread;

        arg = malloc(sizeof(TArgs));
        if (arg == NULL) {
            printf("--ERRO: malloc()\n"); exit(-1);
        }
        arg->idThread = thread; 
        arg->limit = divisions; 

        if (pthread_create(&tidSistema[thread], NULL, pwr2,  (void*) arg)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    //espera todas as threads terminarem
    for (thread=0; thread<NTHREADS; thread++) {
        if (pthread_join(tidSistema[thread], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    }

    //bool para checar se operações estão corretas
    int isCorrect = 1;

    //itera pelo vetor comparando cada elemento do vetor original elevado ao quadrado com o vetor modificado
    //caso algum não seja igual muda a variavel
    for(int i = 0; i < NELEMENTS; i++) {
        if(vector[i] * vector[i] != g_pwrVector[i]) {
            isCorrect = 0;
            break;
        }
    }

    //imprime se o resultado esta correto ou não
    printf("%s\n", isCorrect ? "Calculos corretos" : "Calculos errados");

    pthread_exit(NULL);
}
