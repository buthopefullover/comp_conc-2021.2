#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include <ctype.h>
#include "timer.h"
#include "CheckAlloc.c"
#include "Merge.c"
#include "Palavra.h"


Palavra *Livro; // Variável global que armazena todas as palavras do livro

// variaveis de condicao das threads
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

// quantidade de structs (Palavra) alocadas
long int numStructs=10;
// quantidade de structs (Palavra) armazenadas
long int storedStructs=0;

// número de threads
int nthreads =9;

// contador de broadcasts emitidos
int nBroad = 0;


/**
 * Função que percorre o arquivo e faz uma estimativa
 * para cima de quantas palavras possuem.
 * 
 * @param file arquivo de texto que será contado as palavras
 * @return estimativa de número de palavras
 **/
long int wordCounter(FILE * file){
    long int spaceCounter = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF) // percorre o arquivo todo
    {
        if (isspace(ch)) // caso o caracter seja um espaço incrementa o contador
            ++spaceCounter;
    }
    return spaceCounter;
}


/**
 * Função que le um arquivo sequencialmente parseando as palvras que acha
 * e armazenando na struct global Livro.
 * 
 * @param arg nome do arquivo a ser lido
 **/
void readFileSeq(char * arg)
{
    char* fileName = arg;
    FILE *file;
    char lookBehind; // variavel que armazena o caracter lido anteriormente
    char ch; // variavel que armazena o caracter lido atualmente

    int l = 0; // contador que acompanha a letra lida 
    int p = 0; // contador que acompanha a palavra lida 

    file = fopen(fileName, "r");
    if (file == NULL){ // checa se houve falha na abertura do arquivo
        printf("Falha ao abrir o arquivo!");
        exit(-1); 
    }

    // calcula e armazena uma estima das palavras que existem no arquivo
    numStructs = wordCounter(file);

    // volta a leitura para o inicio do arquivo pois a chamada 
    //do wordCounter mexeu no pontero de leitura
    rewind(file);

    // aloca espaço para todas as palavras baseando-se na estimativa calculada
    Livro = (Palavra*) errorcheck_malloc(sizeof(Palavra)*numStructs);
    for (int i = 0; i < numStructs; i++){
        Livro[i].letras = (char*) errorcheck_malloc(sizeof(char)*TAMANHOPALAVRAS);
    }

    // percorre o arquivo
    while ((ch = fgetc(file)) != EOF)
    {
        // se for um caracter alfanumerico incluimos na palavra que estamos armazenando
        if(isalnum(ch)){
            Livro[p].letras[l] = ch;
            l++; // incrementa o contador de letras para irmos para a próxima posição
        }

        // checa se o caracter que estamos lendo é um acento, checando o anterior e o atual
        // pois um acento é composto de dois caracteres. depois armazena ambos os caracteres
        // na palavra
        if(isAcento(ch, lookBehind)){
            Livro[p].letras[l] = lookBehind; 
            l++;
            Livro[p].letras[l] = ch;
            l++;
            
        }

        // checa se a palavra que estava sendo lida foi terminada, checando primeiro se o caracter 
        // atual é um breakpoint (espaço, hífen, quebra de linha, pontuação) e depois checando se o anterior
        // é um acento ou um caracter alfanumérico
        if((isspace(ch)||ch == '-'||ispunct(ch)) && (isalnum(lookBehind)||isAcento(Livro[p].letras[l-1], Livro[p].letras[l-2]))){
            p++; // incrementa o contador de palavras para irmos para a próxima posição
            l = 0; // zera o contador já que esta palavra foi terminada
        }

        // armazena o caracter que vai ser o anterior na próxima iteração
        lookBehind = ch;
    }

    storedStructs = p; // armazena a quantidade de palavras lidas na variavel global de palavras armazenadas

    fclose(file);  // fecha o arquivo

}

/**
 * Função que le um arquivo concorrentemente parseando as palvras que acha
 * e armazenando na struct global Livro.
 * 
 * @param arg nome do arquivo a ser lido
 **/
void* readFileConc(void * arg)
{
    char* fileName = (char*) arg; 
    FILE *file;
    char lookBehind; // variavel que armazena o caracter lido anteriormente
    char ch; // variavel que armazena o caracter lido atualmente
    long int razao; // armazena a razao usada na divisão da threads de ordenação
    long int limit; // armazena o limite da nova thread que será liberada

    int l = 0; // contador que acompanha a letra lida 
    int p = 0; // contador que acompanha a palavra lida 

    file = fopen(fileName, "r");
    if (file == NULL){ // falha ao abrir o arquivo
        printf("Falha ao abrir o arquivo!");
        exit(-1);
    }

    // calcula e armazena uma estima das palavras que existem no arquivo
    numStructs = wordCounter(file);

    // calcula a razao usada na divisão da threads de ordenação
    razao = numStructs/(nthreads-1);

    //inicia o limite da primeira thread que será liberada
    limit = razao;

    // volta a leitura para o inicio do arquivo pois a chamada 
    //do wordCounter mexeu no pontero de leitura
    rewind(file);

    // aloca espaço para todas as palavras baseando-se na estimativa calculada
    Livro = (Palavra*) errorcheck_malloc(sizeof(Palavra)*numStructs);
    for (int i = 0; i < numStructs; i++){
        Livro[i].letras = (char*) errorcheck_malloc(sizeof(char)*TAMANHOPALAVRAS);
    }

    // percorre o arquivo
    while ((ch = fgetc(file)) != EOF)
    {
        // se for um caracter alfanumerico incluimos na palavra que estamos armazenando
        if(isalnum(ch)){
            Livro[p].letras[l] = ch;
            l++; // incrementa o contador de letras para irmos para a próxima posição

        }

        // checa se o caracter que estamos lendo é um acento, checando o anterior e o atual
        // pois um acento é composto de dois caracteres. depois armazena ambos os caracteres
        // na palavra
        if(isAcento(ch, lookBehind)){
            Livro[p].letras[l] = lookBehind;
            l++;
            Livro[p].letras[l] = ch;
            l++;
            
        }

        // checa se a palavra que estava sendo lida foi terminada, checando primeiro se o caracter 
        // atual é um breakpoint (espaço, hífen, quebra de linha, pontuação) e depois checando se o anterior
        // é um acento ou um caracter alfanumérico
        if((isspace(ch)||ch == '-'||ispunct(ch)) && (isalnum(lookBehind)||isAcento(Livro[p].letras[l-1], Livro[p].letras[l-2]))){
            p++; // incrementa o contador de palavras para irmos para a próxima posição
            storedStructs++; // incrementa as palavras que foram armazenadas
            l = 0; // zera o contador já que esta palavra foi terminada

            // se a palavra é a última do limite que a thread irá ordenar libera ela
            if (p == limit){
                // bloqueia as threads de executarem pois iremos alterar uma váriavel global que elas utilizam
                pthread_mutex_lock(&x_mutex); 
                nBroad++; // incrementa o contador de broadcast para não se perder           
                pthread_mutex_unlock(&x_mutex);

                pthread_cond_broadcast(&x_cond); // libera as threads
                
                
                limit += razao; // calcula o limite da próxima thread a ser liberada
                
            }
        }
        lookBehind = ch;
    }
    // libera a ultima thread de ordenação pois terminamos de ler todas as palavras
    pthread_mutex_lock(&x_mutex);
    nBroad++;
    pthread_mutex_unlock(&x_mutex);

    pthread_cond_broadcast(&x_cond);                

        
    fclose(file);  

    printf("Quantidade de palavras: %ld\n", storedStructs);

    pthread_exit(NULL);


}

/**
 * Função que escreve cada palavra da variável global Livro
 * em um arquivo
 * 
 * @param fileName nome do arquivo que será escrito
 **/
void writeFile (char *fileName){
    FILE *file = fopen(fileName, "w");

    if(file == NULL)
    {
        printf("Erro na criação do arquivo.\n");
        exit(EXIT_FAILURE);
    }

    for(int i =(numStructs-storedStructs) ; i<numStructs; i++)
    {
        fprintf(file, "%s ", Livro[i].letras);
    } 
    fclose(file);

}







void * executaThread(void * arg){
    int ident = *(int *)arg; // id da thread
    
    // bloqueia a as outras thread para que ela ou entre no wait
    // para esperar o broadcast ou checar se os broadcast que já mandaram
    // incluem ela
    pthread_mutex_lock(&x_mutex);
    while(ident > nBroad){ 
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    
    pthread_mutex_unlock(&x_mutex);
    
    int razao = numStructs/(nthreads-1);
    int limite = ident * razao;

    if(ident == nthreads-1){
        MergeSort(Livro, (ident-1)* razao, numStructs-1);
    }
    else{
        MergeSort(Livro, (ident-1)* razao, limite-1);
    }

    
    pthread_exit(NULL);
}





int main(int argc, char *argv[]) {
    double start, finish, tempSeq, tempConc;
    
    nthreads++;

    //cria duas threads
    pthread_t *tid = (pthread_t *) errorcheck_malloc(sizeof(pthread_t) * nthreads);
    int *ident = (int *) errorcheck_malloc(sizeof(int) * nthreads);
    
    //inicializa o mutex e a condição
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);


    // Executando concorrentemente
    GET_TIME(start);
    for(int i=0; i<nthreads;i++){
        ident[i]=i;
        char* fileName = "O-Cortiço-Aluísio-de-Azevedo.txt";
        // caso seja a primeira thread, executa a leitura do arquivo
        if(i == 0){ 
            pthread_create(&tid[i], NULL, readFileConc, (void *)fileName);
        }
        else{
            pthread_create(&tid[i], NULL, executaThread, (void *)&ident[i]);
        }
    }

    //aguarda retorno das threads
    for(int i=0; i<nthreads;i++){
      if(pthread_join(tid[i], NULL))
        printf("Erro: pthread create\n");
        
    }

    // executa a última etapa do merge, juntando todas as partes do vetor 
    // que já foram ordenadas
    for(int i=1; i<nthreads-1; i++){  
        int razao = numStructs/(nthreads-1);
        int limite = i * razao; 
    
        if(i == nthreads-2){
            Merge(Livro, 0, limite -1, numStructs-1);
        }
        else{
            Merge(Livro, 0, limite -1, ((i+1)*razao)-1);
        }
    } 

    GET_TIME(finish);

    tempConc = finish - start;

    writeFile("sortedConc.txt");

    // libera o espaço do vetor para a execução sequencial
    free(Livro);

    // reseta o valor de palavras para a execução sequencial
    storedStructs = 0;

    // Agora executando sequencialmente

    GET_TIME(start);
    char* fileName = "O-Cortiço-Aluísio-de-Azevedo.txt";

    readFileSeq(fileName);

    MergeSort(Livro, 0, numStructs-1);
    
    GET_TIME(finish);

    tempSeq = finish - start;

    writeFile("sortedSeq.txt");

    printf("1 thread de leitura e %d threads de ordenação\n", nthreads-1);
    printf("Tempo de execução concorrente: %f\n", tempConc);
    printf("Tempo de execução sequencial: %f\n", tempSeq);

    return 0;

}