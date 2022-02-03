/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 5 */
/* Codigo: Impressão condicionada */

/***** Condicao logica da aplicacao: as threads A so podem imprimir "Bye" depois que a thread B imprimir  "Hello"  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREADS  5

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
sem_t condt1, condt2;

/* Thread 1 */
void *volte_sempre (void *t) {
    sem_wait(&condt2); // espera todas as outras executar

    printf("Volte sempre!\n");

  pthread_exit(NULL);
}

/* Thread 2 */
void *fique_a_vontade (void *t) {
    sem_wait(&condt1); // espera bem_vindo executar
    printf("Fique a vontade.\n");
    
    x++;

    if(x==3){
        sem_post(&condt2);
    }

    sem_post(&condt1); //permite que todas as outras executem
    
    pthread_exit(NULL);
}

/* Thread 3 */
void *sente_se (void *t) {
    sem_wait(&condt1); // espera bem_vindo executar
    printf("Sente-se por favor.\n");

    x++;

    if(x==3){
        sem_post(&condt2);
    }

    sem_post(&condt1);
    
    pthread_exit(NULL);

}

/* Thread 4 */
void *aceita_agua (void *t) {
    sem_wait(&condt1); // espera bem_vindo executar
    printf("Aceita um copo d’agua?\n");

    x++;
    if(x==3){
        sem_post(&condt2);
    }

    sem_post(&condt1);
    
    pthread_exit(NULL);
}

/* Thread 5 */
void *bem_vindo (void *t) {

    printf("Seja bem-vindo!\n");
    sem_post(&condt1); //permite que todas as outras executem

    pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* inicia os semaforos*/
  sem_init(&condt1, 0, 0);
  sem_init(&condt2, 0, 0);

  /* Cria as threads */
  pthread_create(&threads[4], NULL, bem_vindo, NULL);
  pthread_create(&threads[1], NULL, fique_a_vontade, NULL);
  pthread_create(&threads[2], NULL, sente_se, NULL);
  pthread_create(&threads[3], NULL, aceita_agua, NULL);
  pthread_create(&threads[0], NULL, volte_sempre, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}
