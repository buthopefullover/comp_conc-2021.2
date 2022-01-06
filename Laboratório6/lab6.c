/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Implementação e uso de sincronização por barreira */

/***** Condicao logica da aplicacao: a cada iteracao, as threads fazem uma parte do processamento e só podem continuar depois que todas as threads completaram essa iteração. ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_RND 9 //máximo elemento possível do vetor
#define MIN_RND 0 //mínimo elemento possível do vetor

/* Variaveis globais */
int *g_vector; //vetor
long long int g_nElements; //quantidade de elementos
int bloqueadas = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

//funcao para inicializar o vetor de forma aleatória com inteiro, nesse caso, de 0 até 10
void init_vector(int* vector, int dim) {
    srand(time(0));
    for(int i = 0; i < dim; i++) {
        vector[i] = rand() % ((MAX_RND + 1 - MIN_RND) + MIN_RND);
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

//funcao barreira
void barreira(int nthreads) {
    pthread_mutex_lock(&x_mutex); //inicio secao critica
    if (bloqueadas == (nthreads-1)) { 
      //ultima thread a chegar na barreira
      pthread_cond_broadcast(&x_cond);
      bloqueadas=0;
    } else {
      bloqueadas++;
      pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); //fim secao critica
    return;
}

//funcao das threads
void *tarefa (void *arg) {
  int id = *(int*)arg;
  int *sum;

  sum = (int *) errorcheck_malloc(sizeof(int));
  
  for (int i = 0; i<g_nElements; i++){
    for (int j = 0; j<g_nElements; j++){
      *sum +=g_vector[i];
    }
    printf("%d ", *sum);
    barreira(g_nElements);

    g_vector[id] = rand() % ((MAX_RND + 1 - MIN_RND) + MIN_RND);

    barreira(g_nElements);
    
  }

  pthread_exit((void *) sum);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int vector [rand() % ((MAX_RND + 1 - MIN_RND) + MIN_RND)];
  pthread_t threads[g_nElements];
  int results [g_nElements];
  pthread_t *tidSistema; //identificadores das threads no sistema
  int *tidLocal; //identificadores locais das threads
  int *ret; //ponteiro para pegar o retorno das threads

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  //checa se recebeu todos os argumentos necessários
  if(argc < 2) {
      printf("Digite: %s <número de threads>\n", argv[0]);
      return 1;
  }
  //pega o parametro da linha de comando
  g_nElements = atoll(argv[1]);

  //alocacao de memoria de variaveis auxiliares na concorrencia
  tidSistema  = (pthread_t *) errorcheck_malloc(sizeof(pthread_t) * g_nElements);
  tidLocal  = (int *) errorcheck_malloc(sizeof(int) * g_nElements);

  //alocacao de memoria para o vetor
  g_vector = (int *) errorcheck_malloc(sizeof(int) * g_nElements);

  //inicializa o vetor com valores aleatorios
  init_vector(g_vector, g_nElements);

  for (int i = 0; i<g_nElements; i++){
    printf("%d, ", g_vector[i]);
  }

  /* Cria as threads */
  for(int thread=0; thread<g_nElements; thread++) {
    tidLocal[thread] = thread;

    if (pthread_create(&tidSistema[thread], NULL, tarefa, (void*)  &tidLocal[thread])) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  // espera todas as threads terminarem
  for (int thread=0; thread<g_nElements; thread++) {
    if (pthread_join(tidSistema[thread], (void**) &ret)) {
        printf("--ERRO: pthread_join() \n"); exit(-1); 
    }
    results[thread] = *ret;

    //printf("Somatório da Thread %d: %d \n", thread, *ret);

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

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
  return 0;
}
