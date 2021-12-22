/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 5 */
/* Codigo: Impressão condicionada */

/***** Condicao logica da aplicacao: as threads A so podem imprimir "Bye" depois que a thread B imprimir  "Hello"  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  5

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond1, x_cond2;

/* Thread 1 */
void *volte_sempre (void *t) {
  pthread_mutex_lock(&x_mutex);
  if(x < 3)
    pthread_cond_wait(&x_cond2, &x_mutex); 
  printf("Volte sempre!\n");
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Thread 2 */
void *fique_a_vontade (void *t) {
  pthread_mutex_lock(&x_mutex);
  if(x == 0)
  pthread_cond_wait(&x_cond1, &x_mutex); 
  x++; 
  printf("Fique a vontade.\n");
  if(x == 4) {
    pthread_cond_signal(&x_cond2);
  }
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Thread 3 */
void *sente_se (void *t) {
  pthread_mutex_lock(&x_mutex);
  if(x == 0)
  pthread_cond_wait(&x_cond1, &x_mutex); 
  x++; 
  printf("Sente-se por favor.\n");
  if(x == 4) {
    pthread_cond_signal(&x_cond2);
  }
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Thread 4 */
void *aceita_agua (void *t) {
  pthread_mutex_lock(&x_mutex);
  if(x == 0)
  pthread_cond_wait(&x_cond1, &x_mutex); 
  x++; 
  printf("Aceita um copo d’agua?\n");
  if(x == 4) {
    pthread_cond_signal(&x_cond2);
  }
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Thread 5 */
void *bem_vindo (void *t) {
  int boba1, boba2;

  printf("Seja bem-vindo!\n");
  boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;
  pthread_mutex_lock(&x_mutex);
  x++;
  pthread_cond_broadcast(&x_cond1);
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond1, NULL);
  pthread_cond_init (&x_cond2, NULL);

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

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond1);
  pthread_cond_destroy(&x_cond2);
  return 0;
}
