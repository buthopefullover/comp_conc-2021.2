#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include"timer.h"

int nthreads;
long int dim;
int *vetor;

//fluxo das threads
void* tarefa (void* arg) {
   long int id = (long int) arg;
   long int tam_bloco = dim/nthreads;
   long int ini = id*tam_bloco;
   long int fim = ini+tam_bloco;
   long int soma_local=0;
   if (id==nthreads-1) fim=dim; //trata sobra
   for(long int i=ini; i<fim; i++)
      soma_local+=vetor[i];
   pthread_exit((void*)soma_local);
}

//fluxo principal
int main(int argc, char *argv[]) {
   long int soma_seq=0;
   long int soma_conc=0;
   double ini, fim; //tomada de tempo
   pthread_t *tid;
   long int retorno;

   //verifica os parametros
   if(argc<3){
      fprintf(stderr, "Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
      return 1;
   }
   dim = atoll(argv[1]);
   nthreads = atoi(argv[2]);
   //aloca espaco e preenche o vetor
   vetor = (int*) malloc(sizeof(int)*dim);
   if(vetor==NULL){
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   for (int i=0; i<dim; i++)
      vetor[i] = i/1000;

   //solucao sequencial
   GET_TIME(ini);
   for (long int i=0; i<dim; i++)
      soma_seq+=vetor[i];
   GET_TIME(fim);
   printf("Tempo-Seq: %lf\n", fim-ini);

   //solucao concorrente
   GET_TIME(ini);
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL){
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   for(long int i=0; i<nthreads; i++){
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)) {
         fprintf(stderr, "ERRO--pthread_create\n");
         return 2;
      }   
   }
   for(long int i=0; i<nthreads; i++){
      if(pthread_join(*(tid+i), (void**)&retorno)) {
         fprintf(stderr, "ERRO--pthread_join\n");
         return 2;
      }   
      soma_conc+=retorno;
   }
   GET_TIME(fim);
   printf("Tempo-Conc: %lf\n", fim-ini);

   //exibe os resultados
   printf("Soma-Seq:  %ld\n", soma_seq);
   printf("Soma-Conc: %ld\n", soma_conc);

   free(tid);
   free(vetor);

   return 0;
}
