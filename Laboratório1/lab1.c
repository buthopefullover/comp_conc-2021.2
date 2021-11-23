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
#define max_number 10 //máximo elemento possível do vetor
#define min_number 0 //mínimo elemento possível do vetor

int pwrvector[NELEMENTS]; //vetor que será modificado, inicialmente identico ao original

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread, limit;
} t_Args;


//funcao para inicializar o vetor de forma aleatória com inteiro, nesse caso, de 0 até 10
void initVector(int* vector) {
    srand(time(0));
    for(int i = 0; i < NELEMENTS; i++) {
        vector[i] = rand() % (max_number + 1 - min_number) + min_number;
        pwrvector[i] = vector[i]; //copia cada elemento do vetor original no vetor que modificaremos
    }
}

//funcao executada pelas threads que efetua a elevação ao quadrado dos elementos do vetor
void *Pwr2 (void* arg) {
    t_Args *args = (t_Args *) arg;

    int start = args->idThread*args->limit; //calcula a posição de inicio da operação, dependendo de qual thread esta sendo executada

    //verifica se estamos na última thread, caso esteja efetua a operação até o final do vetor 
    //(para evitar erros com divisões com resto)
    if (args->idThread+1 == NTHREADS){
	    for (int i = start; i<NELEMENTS; i++){
            pwrvector[i] = pwrvector[i] * pwrvector[i];
        }
    }
    else{
        //caso não esteja na última thread faz as operações nos elementos até atingir a quantidade calculada anteriormente (divisions)
        for (int i = start; i <start+args->limit; i++){
		    pwrvector[i] = pwrvector[i] * pwrvector[i];
	    }
    }
    pthread_exit(NULL);                                                                       
}

//funcao principal do programa
int main() {
    pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
    int thread; //variavel auxiliar
    int tid_local[NTHREADS]; //identificadores locais das threads
    int divisions = (NELEMENTS/NTHREADS)-1; //quantos elementos cada thread operará em
    t_Args *arg; //receberá os argumentos para a thread
    int vector[NELEMENTS]; //vetor original
    
    initVector(vector); //inicializa o vetor

    //cria as  threads
    for(thread=0; thread<NTHREADS; thread++) {
        tid_local[thread] = thread;

        arg = malloc(sizeof(t_Args));
        if (arg == NULL) {
            printf("--ERRO: malloc()\n"); exit(-1);
        }
        arg->idThread = thread; 
        arg->limit = divisions; 

        if (pthread_create(&tid_sistema[thread], NULL, Pwr2,  (void*) (void*) arg)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    //espera todas as threads terminarem
    for (thread=0; thread<NTHREADS; thread++) {
        if (pthread_join(tid_sistema[thread], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    }

    //bool para checar se operações estão corretas
    int isCorrect = 1;

    //itera pelo vetor comparando cada elemento do vetor original elevado ao quadrado com o vetor modificado
    //caso algum não seja igual muda a variavel
    for(int i = 0; i < NELEMENTS; i++) {
        if(vector[i] * vector[i] != pwrvector[i]) {
            isCorrect = 0;
            break;
        }
    }

    //imprime se o resultado esta correto ou não
    printf("%s\n", isCorrect ? "Calculos corretos" : "Calculos errados");

    pthread_exit(NULL);
}
