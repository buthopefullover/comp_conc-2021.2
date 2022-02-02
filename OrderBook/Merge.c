#include "Palavra.h"
#include "Compare.c"

/**
 *Mescla dois pedacos de lista ordenadamente 
 * @param Lista Lista a ser ordenada
 * @param inicio ID do inicio a ser ordenado
 * @param meio ID de divisao da lista
 * @param fim ID do ultimo item a ser ordenado
 **/
void Merge(Palavra *Lista, int inicio, int meio, int fim)//passar tamanho da struct no fim
{
    Palavra *listaTemporaria; int aux1, aux2, fimAux1, fimAux2, tamanho;  
    tamanho = fim - inicio +1; // admitindo que o inicio pode ser o indice 0 somamos 1;
    aux1 = inicio; aux2 = meio+1; //aux1: indice de inicio da primeira divisao //aux2: indice de inicio da segunda divisao
    fimAux1 = 0; fimAux2 = 0; //validadores de fim das partes divididas
    listaTemporaria = (Palavra *) errorcheck_malloc(tamanho*sizeof(Palavra)); //Lista temporaria com o tamanho do vetor a ser ordenado
    for (int i = 0; i < tamanho; i++){
        listaTemporaria[i].letras = (char*) errorcheck_malloc(sizeof(char)*TAMANHOPALAVRAS);
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



/**
 * Algoritmo recursivo que divide a lista ate ter somente 1  struct e depois executa o Merge
 * @param inicio ID do primeiro item a ser ordenado
 * @param fim ID do ultimo item a ser ordenado
 **/
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
