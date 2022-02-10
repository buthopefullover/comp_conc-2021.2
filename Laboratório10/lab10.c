#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NELEMENTS 10 // quantidade de elementos do buffer


int nProd; // quantidade de threads produtoras
int nCons; // quantidade de threads consumidoras

sem_t semProd, semCons; // semaforos para exclusao mutua
sem_t bufferVazio, slotCheio; // semaforos condicionais
int buffer[NELEMENTS]; // buffer de elementos

// funcao para tratar erros com o malloc
void *errorcheck_malloc(int size) {
    void *ptr = malloc(size);
    if(ptr == NULL) {
        printf("--ERRO: malloc()\n");
        exit(-1);
    }
    return ptr;
}

// funcao que retira um elemento do buffer
void Remove (int id) {
    static int out = 0; // indice no buffer que tera seu valor removido, precisa ser static, pois os elementos são retirados um de cada vez
    // aguarda slot estar cheio
    sem_wait(&slotCheio);

    // exclusão mútua entre consumidores
    sem_wait(&semCons);
    printf("Consumidor %d consumindo o valor %d\n", id, buffer[out]);
    buffer[out] = 0;
    out++;
    sem_post(&semCons);

    // sinaliza que o buffer esta vazio
    if(out == NELEMENTS) {
        out = 0;
        sem_post(&bufferVazio);
    }
}

// funcao que insere todos os elementos no buffer
void Insert (int id) {
    // espera esvaziar o buffer
    sem_wait(&bufferVazio);
    // exclusão mutua entre produtores
    sem_wait(&semProd);
    // preenche todo o buffer
    for(int i = 0; i < NELEMENTS; i++) {    
        buffer[i] = i+id;
    }
    sem_post(&semProd);

    // imprime todos os valores inseridos
    printf("Produtor %d inserindo\n", id);
    printf("Elementos inseridos: ");
    for(int i = 0; i < NELEMENTS; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    // sinaliza que o buffer esta cheio
    for(int i = 0; i < NELEMENTS; i++) {
        sem_post(&slotCheio);
    }
}

// funcao executada pelas threads produtoras, inserindo todos os elementos de uma vez só
void *produtor(void * arg) {
    int id = *(int*)arg; // id da thread

    while(1) {
        Insert(id);
    }
    pthread_exit(NULL);
}

// funcao executada pelas threads consumidoras, consumindo um elemento de cada vez
void *consumidor(void * arg) {
    int id = *(int*)arg; // id da thread
    
    while(1) {
        Remove(id);
    } 
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    int nThreads; // quantidade de threads é a soma de threads produtoras e consumidoras
    pthread_t *tidSistema; // identificadores das threads no sistema
    int *tidLocal; // identificadores locais das threads

    // checa se recebeu todos os argumentos necessários
    if(argc < 3) {
        printf("Digite: %s <número de threads consumidoras> <número de threads produtoras>\n", argv[0]);
        return 1;
    }

    // pega o parametro da linha de comando
    nCons = atoll(argv[1]);
    nProd = atoll(argv[1]);

    // calcula a quantidade de threads totais
    nThreads = nProd + nCons;

    // alocacao de memoria de variaveis auxiliares na concorrencia
    tidSistema  = (pthread_t *) errorcheck_malloc(sizeof(pthread_t) * nThreads);
    tidLocal  = (int *) errorcheck_malloc(sizeof(int) * nThreads);



    // inicia os semaforos
    sem_init(&semCons, 0, 1);  
    sem_init(&semProd, 0, 1);
    sem_init(&bufferVazio, 0, 0); 
    sem_init(&slotCheio, 0, NELEMENTS); 

    // cria as threads produtoras 
    for(int thread = 0; thread < nProd; thread++) {
        tidLocal[thread] = thread;
        if (pthread_create(&tidSistema[thread], NULL, produtor, (void*)  &tidLocal[thread])) { 
            printf("ERRO: pthread_create()\n"); exit(-1); 
        }
    }

    // cria as consumidoras threads
    for(int thread; thread < nCons; thread++) {
        tidLocal[thread] = thread;
        if (pthread_create(&tidSistema[thread], NULL, consumidor, (void*)  &tidLocal[thread])) { 
            printf("ERRO: pthread_create()\n"); exit(-1); 
        }
    }

    // espera todas as threads terminarem
    for (int thread=0; thread<nThreads; thread++) {
        if (pthread_join(tidSistema[thread], NULL)) { 
            printf("ERRO: pthread_create()\n"); exit(-1); 
        } 
    } 

    // desaloca variaveis e termina
    free(tidSistema);
    free(tidLocal);
    sem_destroy(&semCons);
    sem_destroy(&semProd);
    sem_destroy(&bufferVazio);
    sem_destroy(&slotCheio);

    return 0;
}