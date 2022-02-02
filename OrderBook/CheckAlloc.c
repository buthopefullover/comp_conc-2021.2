

/**
 * Função para checar erros no realloc
 * 
 * @param size tamanho da realocação a ser feita
 * @param ptr vetor que será realocado
 **/
void *errorcheck_realloc(int size, void *ptr) {
    void* temp = realloc(ptr, size);
    if(temp == NULL) {
        printf("--ERRO: realloc()\n");
        exit(-1);
    }
    return temp;
    
}

/**
 * Função para checar erros no malloc
 * 
 * @param size tamanho da realocação a ser feita
 **/
void *errorcheck_malloc(int size) {
    void *ptr = malloc(size);
    if(ptr == NULL) {
        printf("--ERRO: malloc()\n");
        exit(-1);
    }
    return ptr;
}