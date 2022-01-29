#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>

typedef struct{
    char *letras;
}Palavra;

Palavra *Livro;
int TamanhoPalavras=50;
char Acentos [] = {"áéíóúàèìòùâêîôûãõç"};//acentos a ser incluidos


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
        if (ch == ' ' || ch == '-')
            ++spaceCounter;
    }
    return spaceCounter;
}

// le o aquivo pegando as palavras e armazenando-as no vetor de struct global (Palavras)
void readFile(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    char lookBehind;
    char ch; //
    long int wordCount;

    int l = 0;
    int p = 0;

    if (file == NULL)
        return; // não consegui abrir o arquivo

    
    wordCount = wordCounter(file);

    rewind(file);

    printf("%ld", wordCount);

    Livro = (Palavra*) errorcheck_malloc(sizeof(Palavra)*wordCount);
    for (int i = 0; i < wordCount; i++){
        Livro[i].letras = (char*) errorcheck_malloc(sizeof(char)*TamanhoPalavras);
    }

    while ((ch = fgetc(file)) != EOF)
    {

        if(strchr(Acentos, ch) || isalnum(ch)){
            Livro[p].letras[l] = ch;
            l++;

        }

        if((isspace(ch)||ch == '-') && (isalnum(lookBehind)||ispunct(lookBehind))){
            //printf("%c ispunc: %d\n", ch, ispunct(ch));

            //printf("%s\n", Livro[p].letras);
            p++;
            l = 0;

        }
        lookBehind = ch;

    fclose(file);       

}

int main(int argc, char *argv[]) {

    readFile("Quem e voce, Alasca_ - John Green (2).txt");

    for(int i =0; i<200; i++)//DEUS É BOM O TEMPO TODO
    {
        printf("palavra %d: %s\n", i, Livro[i].letras);
    } 

    return 0;

}