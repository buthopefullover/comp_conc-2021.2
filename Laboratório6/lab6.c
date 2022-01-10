/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Implementação e uso de sincronização por barreira */

/***** Condicao logica da aplicacao: a cada iteracao, as threads somam todos os elementos do vetor e modificam cada posição com um valor aleatório
 só podendo continuar depois que todas as threads completaram cada passo da iteração. ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_RND 9 // máximo elemento possível do vetor
#define MIN_RND 0 // mínimo elemento possível do vetor

// variaveis globais
int *g_vector; // vetor que as threads irão modificar
long long int g_nElements; // quantidade de elementos e consequentemente quantidade de threads
int g_bloqueadas = 0; // variável auxiliar para saber quantas threads estão bloqueadas
pthread_mutex_t x_mutex; 
pthread_cond_t x_cond;

// funcao para inicializar o vetor de forma aleatória com inteiro, nesse caso, de 0 até 9
void init_vector(int* vector, int dim) {
    srand(time(0));
    for(int i = 0; i < dim; i++) {
        vector[i] = rand() % ((MAX_RND + 1 - MIN_RND) + MIN_RND);
    }
}

// funcao para tratar erros com o malloc
void *errorcheck_malloc(int size) {
    void *ptr = malloc(size);
    if(ptr == NULL) {
        printf("--ERRO: malloc()\n");
        exit(-1);
    }
    return ptr;
}

// funcao barreira
void barreira(int nthreads) {
    pthread_mutex_lock(&x_mutex); // inicio secao critica
    if (g_bloqueadas == (nthreads-1)) { 
      // ultima thread a chegar na barreira
      pthread_cond_broadcast(&x_cond);
      g_bloqueadas=0; // reseta variavel auxiliar já que todas as threads foram desbloquadas
    } else {
      g_bloqueadas++;
      pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); // fim secao critica
    return;
}

// funcao das threads
void *tarefa (void *arg) {
  int id = *(int*)arg; // id da thread
  int *sum; // variável para armazenar a soma

  sum = (int *) errorcheck_malloc(sizeof(int)); // armazena memória do ponteiro que será retornado
  
  for (int i = 0; i<g_nElements; i++){ // iterações N
    for (int j = 0; j<g_nElements; j++){ // percorre pelo vetor de tamanhao N somando os elementos
      *sum +=g_vector[i];
    }

    barreira(g_nElements); // barra as threads até todas terminarem a soma

    g_vector[id] = rand() % ((MAX_RND + 1 - MIN_RND) + MIN_RND); // muda a posição do vetor correspondente ao id da thread

    barreira(g_nElements); // barra as threads até todas terminarem a alteração
    
  }

  pthread_exit((void *) sum); // retorna a soma
}

// funcao principal
int main(int argc, char *argv[]) {
  int *results; // vetor para armazenar os resultados das somas
  pthread_t *tidSistema; // identificadores das threads no sistema
  int *tidLocal; // identificadores locais das threads
  int *ret; // ponteiro para pegar o retorno das threads

  // inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  // checa se recebeu todos os argumentos necessários
  if(argc < 2) {
      printf("Digite: %s <número de threads>\n", argv[0]);
      return 1;
  }
  // pega o parametro da linha de comando
  g_nElements = atoll(argv[1]);

  // alocacao de memoria de variaveis auxiliares na concorrencia
  tidSistema  = (pthread_t *) errorcheck_malloc(sizeof(pthread_t) * g_nElements);
  tidLocal  = (int *) errorcheck_malloc(sizeof(int) * g_nElements);

  // alocacao de memoria para o vetor
  g_vector = (int *) errorcheck_malloc(sizeof(int) * g_nElements);
  results = (int *) errorcheck_malloc(sizeof(int) * g_nElements);

  // inicializa o vetor com valores aleatorios
  init_vector(g_vector, g_nElements);

  // cria as threads
  for(int thread=0; thread<g_nElements; thread++) {
    tidLocal[thread] = thread;

    if (pthread_create(&tidSistema[thread], NULL, tarefa, (void*)  &tidLocal[thread])) {
      printf("ERRO: pthread_create()\n"); exit(-1);
    }
  }

  // espera todas as threads terminarem
  for (int thread=0; thread<g_nElements; thread++) {
    if (pthread_join(tidSistema[thread], (void**) &ret)) {
        printf("ERRO: pthread_join() \n"); exit(-1); 
    }
    results[thread] = *ret;

    printf("Somatório da Thread %d: %d \n", thread+1, *ret);

    free(ret);
  }

  // checa se os resultados estão corretos
  for (int i = 1; i<g_nElements; i++){
    if (results[i-1] != results[i]){
      printf("ERRO: Resultados não iguais.");
      return 1;
    }
  }

  printf ("FIM.\n");

  // desaloca variaveis e termina
  free(results);
  free(tidSistema);
  free(tidLocal);
  free(g_vector);
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
  return 0;
}
