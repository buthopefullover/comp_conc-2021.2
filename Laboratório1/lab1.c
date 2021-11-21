/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: "Hello World" usando threads em C com passagem de um argumento */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  2 //total de threads a serem criadas
#define NELEMENTS 1000
#define max_number 10
#define min_number 0

int vector[NELEMENTS];
int pwrvector[NELEMENTS];
int divisions = (NELEMENTS/NTHREADS)-1;

void initVector(int* vector) {
    srand(time(0));
    for(int i = 0; i < NELEMENTS; i++) {
        vector[i] = rand() % (max_number + 1 - min_number) + min_number;
        pwrvector[i] = vector[i];
    }
}

//funcao executada pelas threads
void *Pwr2 (void* arg) {
    int idThread = * (int*) arg;

    //printf("aa %d %d aa", idThread, idThread*divisions);

    if (idThread+1 == NTHREADS){
	    for (int i = idThread*divisions; i<NELEMENTS; i++){
            pwrvector[i] = pwrvector[i] * pwrvector[i];
        }
    }
    else{
        for (int i = idThread*divisions; i <(idThread*divisions)+divisions; i++){
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

    //int divisions = (N_ELEMENTS/NTHREADS)-1;
    
    initVector(vector);

    for(int loop = 0; loop < 10; loop++)
        printf("%d ", vector[loop]);

    for(thread=0; thread<NTHREADS; thread++) {
        tid_local[thread] = thread;
        if (pthread_create(&tid_sistema[thread], NULL, Pwr2,  (void*) &tid_local[thread])) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }
      //--espera todas as threads terminarem
    for (thread=0; thread<NTHREADS; thread++) {
        if (pthread_join(tid_sistema[thread], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    }

    for(int loop = 0; loop < 10; loop++)
        printf("%d ", pwrvector[loop]);

    // Verifica se os elementos do vetor utilizado pelos cálculos multi-thread estão iguais
    // ao quadrado elementos do vetor referência.
    int isCorrect = 1;
    for(int i = 0; i < NELEMENTS; i++) {
        if(vector[i] * vector[i] != pwrvector[i]) {
        isCorrect = 0;
        break;
        }
    }
    printf("%s\n", isCorrect ? "Calculos certos" : "Calculos errados");
    printf("--Thread principal terminou\n");
    pthread_exit(NULL);
}
