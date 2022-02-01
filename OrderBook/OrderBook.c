#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include <ctype.h>
#include <locale.h>

typedef struct{
    char *letras;
}Palavra;

Palavra *Livro;
int TamanhoPalavras=100;

//variaveis de condicao das threads
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;
int liberaThread = 0;//validador 0 ou 1
int contaThread=0;
int bloqueadas = 0;

//acentos a ser incluidos
char acentoA[] = {"áÁãÃàÀâÂ"};
char acentoE[] = {"éÉèÈêÊ"};
char acentoI[] = {"íÍìÌ"};
char acentoO[] = {"óÓòÒôÔõÕ"};
char acentoU[] = {"úÚùÙûÛ"};
char acentoC[] = {"çÇ"};

//numStucts alocadas
long int numStructs=10;
//numStructs utilizadas
long int storedStructs=0;

//num threads
int nthreads =3;

int nBroad = 0;


// funcao para tratar erros com o malloc
void *errorcheck_realloc(int SIZEARR, void *ptr) {
    void* temp = realloc(ptr, SIZEARR);
    if(temp == NULL) {
        printf("--ERRO: realloc()\n");
        exit(-1);
    }
    return temp;
    
}

// funcao para tratar erros com o malloc
void *errorcheck_malloc(int SIZEARR) {
    void *ptr = malloc(SIZEARR);
    if(ptr == NULL) {
        printf("--ERRO: malloc()\n");
        exit(-1);
    }
    return ptr;
}

long int wordCounter(FILE * file){
    long int spaceCounter = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF) 
    {
        if (isspace(ch))
            ++spaceCounter;
    }
    return spaceCounter;
}

int isAcento(const char *string, char character)
{
    const char *pointer;

    pointer = strchr(string, character);

    if(pointer)
        return 1;
    else
        return 0;
}

int isAcento2(char ch, char lb){
    if(lb == -61){
        if(isAcento(acentoA, ch)){
            return 1;
        }
        else if(isAcento(acentoE, ch)){
            return 1;
        }
        else if(isAcento(acentoI,ch)){
            return 1;
        }
        else if(isAcento(acentoO,ch)){
            return 1;
        }
        else if(isAcento(acentoU,ch)){
            return 1;
        }
        else if(isAcento(acentoC,ch)){
            return 1;
        }
    }
    return 0;
}


// le o aquivo pegando as palavras e armazenando-as no vetor de struct global (Palavras)
void* readFile(void * arg)
{
    char* fileName = (char*) arg;
    FILE *file;
    char lookBehind;
    char ch; 
    long int razao;
    long int limit;

    int l = 0;
    int p = 0;

    file = fopen(fileName, "r");
    if (file == NULL){
        printf("Falha ao abrir o arquivo!");
        exit(-1); // não consegui abrir o arquivo
    }

    
    numStructs = wordCounter(file);
    razao = numStructs/(nthreads-1);
    limit = razao;

    rewind(file);


    Livro = (Palavra*) errorcheck_malloc(sizeof(Palavra)*numStructs);
    for (int i = 0; i < numStructs; i++){
        Livro[i].letras = (char*) errorcheck_malloc(sizeof(char)*TamanhoPalavras);
    }

    while ((ch = fgetc(file)) != EOF)
    {

        if(isalnum(ch)){
            Livro[p].letras[l] = ch;
            l++;

        }

        if(isAcento2(ch, lookBehind)){
            Livro[p].letras[l] = lookBehind;
            l++;
            Livro[p].letras[l] = ch;
            l++;
            
        }

        if((isspace(ch)||ch == '-'||ispunct(ch)) && (isalnum(lookBehind)||isAcento2(Livro[p].letras[l-1], Livro[p].letras[l-2]))){
            p++;
            storedStructs++; // posso só igualar com p antes de finalizar
            
            l = 0;

            if (p == limit){
                pthread_mutex_lock(&x_mutex);
                nBroad++;
                
                pthread_mutex_unlock(&x_mutex);
                pthread_cond_broadcast(&x_cond);
                
                
                limit += razao;
                
            }

        }
        lookBehind = ch;
    }
    if (ch == EOF){
        pthread_mutex_lock(&x_mutex);
        nBroad++;

        pthread_mutex_unlock(&x_mutex);

        pthread_cond_broadcast(&x_cond);                
    }
        
    fclose(file);  

    pthread_exit(NULL);


}

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

int substituiAcento(char a){
    if(isAcento(acentoA, a)){
        return 97;//retorna o codigo de a
    
    }        
    else if(isAcento(acentoE, a)){
        return 101;//retorna o codigo de e
    
    }
    else if(isAcento(acentoI,a)){
        return 105;//retorna o codigo de i
    
    }
    else if(isAcento(acentoO,a)){
        return 111;//retorna o codigo de o
    
    }
    else if(isAcento(acentoU,a)){
        return 117;//retorna o codigo de u
    
    }
    else if(isAcento(acentoC,a)){
        return 99;//retorna o codigo de c
        
    }
    return 0;
}

/**
 * 1 - palavra2 antes palavra1
 * 0 - iguais
 * -1 - palavra1 antes palavra2 
 */ 
int stringcomp(char *palavra1, char *palavra2){
    char* temp1 = (char*) errorcheck_malloc(sizeof(char)*strlen(palavra1));
    char* temp2 = (char*) errorcheck_malloc(sizeof(char)*strlen(palavra2));

    strcpy(temp1, palavra1);
    strcpy(temp2, palavra2);
    
    int achouAcentoI =0;
    int achouAcentoK =0;
    int achouAcento =0;

    for(int i =0, k=0; i<strlen(palavra1) && k<strlen(palavra2); i++, k++){
        achouAcentoI =0;
        achouAcentoK =0;
        achouAcento =0;
        
        if(temp1[i]==-61||temp2[i]==-61){ // tratamos acento na palavra 1
            
            if(temp1[i]==-61){
                temp1[i] = substituiAcento(temp1[i+1]);
                i++;
                achouAcentoI++;
                achouAcento++;
            }
            if(temp2[k]==-61){
                temp2[k] = substituiAcento(temp2[k+1]);
                k++;
                achouAcentoK++;
                achouAcento++;
            }
            
            if(achouAcentoI){
                if(achouAcentoK){
                    if(tolower(temp1[i-1])>tolower(temp2[k-1]))
                        return 1;
                    else if(tolower(temp1[i-1])<tolower(temp2[k-1]))
                        return -1;
                }
                else{
                    if(tolower(temp1[i-1])>tolower(temp2[k]))
                        return 1;
                    else if(tolower(temp1[i-1])<tolower(temp2[k]))
                        return -1;
                }
            }
            if(achouAcentoK && !achouAcentoI){
                if(tolower(temp1[i])>tolower(temp2[k-1]))
                        return 1;
                else if(tolower(temp1[i])<tolower(temp2[k-1]))
                    return -1;
            }

            if(!achouAcento){
                if(tolower(temp1[i])>tolower(temp2[k]))
                    return 1;
                else if(tolower(temp1[i])<tolower(temp2[k]))
                    return-1;
            }
            continue;
        }
        
        if ( tolower(temp1[i]) > tolower(temp2[k]) )
            return 1;
        
        else if( tolower(temp1[i]) < tolower(temp2[k]) )
            return -1;
        
            
    }

    if((strlen(palavra1)-achouAcentoI)>(strlen(palavra2)-achouAcentoK))
        return 1;
    else if((strlen(palavra1)-achouAcentoI)<(strlen(palavra2)-achouAcentoK))
        return -1; 
    
    return strcmp(palavra1, palavra2);
    
    
}





void Merge(Palavra *Lista, int inicio, int meio, int fim)//passar tamanho da struct no fim
{
    Palavra *listaTemporaria; int aux1, aux2, fimAux1, fimAux2, tamanho;  
    tamanho = fim - inicio +1; // admitindo que o inicio pode ser o indice 0 somamos 1;
    aux1 = inicio; aux2 = meio+1; //aux1: indice de inicio da primeira divisao //aux2: indice de inicio da segunda divisao
    fimAux1 = 0; fimAux2 = 0; //validadores de fim das partes divididas
    listaTemporaria = (Palavra *) errorcheck_malloc(tamanho*sizeof(Palavra)); //Lista temporaria com o tamanho do vetor a ser ordenado
    for (int i = 0; i < tamanho; i++){
        listaTemporaria[i].letras = (char*) errorcheck_malloc(sizeof(char)*TamanhoPalavras);
    }

    for(int i=0; i<tamanho; i++)
    {
        if(!fimAux1 && !fimAux2)
        {
            if(stringcomp(Lista[aux1].letras, Lista[aux2].letras)<0)
                listaTemporaria[i] = Lista[aux1++];
            else 
                listaTemporaria[i]= Lista[aux2++];
            if(aux1 > meio)
                fimAux1 = 1;
            if(aux2 > fim)
                fimAux2 = 1;
        }
        else 
        {
            if (!fimAux1)
                listaTemporaria[i] = Lista[aux1++];
            else 
                listaTemporaria[i] = Lista[aux2++];
        }
    }
    for(int j=0 , k = inicio; j<tamanho; j++, k++)
        Lista[k]= listaTemporaria[j];
    
    free(listaTemporaria);
    return;
}




void MergeSort(Palavra *Lista, int inicio, int fim)
{
    int meio = (fim+inicio)/2;
    if(inicio < fim){ 
        MergeSort(Lista, inicio, meio);
        MergeSort(Lista, meio+1, fim); 
        Merge(Lista, inicio, meio, fim); 
    }
    return;
}



void * executaThread(void * arg){
    int ident = *(int *)arg;
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
    
    nthreads++;

    //cria duas threads
    pthread_t *tid = (pthread_t *) errorcheck_malloc(sizeof(pthread_t) * nthreads);
    int *ident = (int *) errorcheck_malloc(sizeof(int) * nthreads);
    
    //inicializa o mutex e a condição
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);


    for(int i=0; i<nthreads;i++){
        ident[i]=i;//ident[i]=i+1;
        char* fileName = "Quem e voce, Alasca_ - John Green (2).txt";
        if(i == 0){ 
            pthread_create(&tid[i], NULL, readFile, (void *)fileName);
        }
        else{
            pthread_create(&tid[i], NULL, executaThread, (void *)&ident[i]);
        }
    }

    //aguarda retornoi das threads
    for(int i=0; i<nthreads;i++){
      if(pthread_join(tid[i], NULL))
        printf("Erro: pthread create\n");
        
    }

    for(int i=1; i<nthreads-1; i++){  
        int razao = numStructs/(nthreads-1);
        int limite = i * razao; //id da thread * razao
        //int meio = (fim+inicio)/2;
    
        if(i == nthreads-2){
            Merge(Livro, 0, limite -1, numStructs-1);
        }
        else{
            Merge(Livro, 0, limite -1, ((i+1)*razao)-1);//alterar fim e meio
        }
    } 
    writeFile("sorted.txt");


    return 0;

}