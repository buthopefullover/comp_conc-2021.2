
//acentos a ser incluidos
char acentoA[] = {"áÁãÃàÀâÂ"};
char acentoE[] = {"éÉèÈêÊ"};
char acentoI[] = {"íÍìÌ"};
char acentoO[] = {"óÓòÒôÔõÕ"};
char acentoU[] = {"úÚùÙûÛ"};
char acentoC[] = {"çÇ"};


/**
 * retorna se possui ou nao acento
 * @param string string com acentos
 * @param character caracter a ser verificado
 * @return (1-True | 0-False)
 */
int searchAcento(const char *string, char character)
{
    const char *pointer;

    pointer = strchr(string, character);

    if(pointer)
        return 1;
    else
        return 0;
}
/**
 * Diz se possui ou nao acento
 * @param ch caracter com acento
 * @param lb identificacao do acento
 * @return (1-True | 0-False)
 */
int isAcento(char ch, char lb){
    if(lb == -61){
        if(searchAcento(acentoA, ch)){
            return 1;
        }
        else if(searchAcento(acentoE, ch)){
            return 1;
        }
        else if(searchAcento(acentoI,ch)){
            return 1;
        }
        else if(searchAcento(acentoO,ch)){
            return 1;
        }
        else if(searchAcento(acentoU,ch)){
            return 1;
        }
        else if(searchAcento(acentoC,ch)){
            return 1;
        }
    }
    return 0;
}

/**
 * Substitui caracter com acento pelo caracter sem acento
 * @param a letra com acento a ser substituida
 * @return codigo da letra minuscula sem acento na tabela ascii
 **/
int substituiAcento(char a){
    if(searchAcento(acentoA, a)){
        return 97;//retorna o codigo de a
    
    }        
    else if(searchAcento(acentoE, a)){
        return 101;//retorna o codigo de e
    
    }
    else if(searchAcento(acentoI,a)){
        return 105;//retorna o codigo de i
    
    }
    else if(searchAcento(acentoO,a)){
        return 111;//retorna o codigo de o
    
    }
    else if(searchAcento(acentoU,a)){
        return 117;//retorna o codigo de u
    
    }
    else if(searchAcento(acentoC,a)){
        return 99;//retorna o codigo de c
        
    }
    return 0;
}

/**
 * Compara strings e retorna a que vem antes alfabeticamente
 * @param palavra1 primeira string a ser comparada
 * @param palavra2 segunda string a ser comparada
 * @return (1 - palavra2 antes palavra1 | 0 - iguais | -1 - palavra1 antes palavra2 )
 */ 
int stringcomp(char *palavra1, char *palavra2){
    char* temp1 = (char*) errorcheck_malloc(sizeof(char)*strlen(palavra1));
    char* temp2 = (char*) errorcheck_malloc(sizeof(char)*strlen(palavra2));

    strcpy(temp1, palavra1);
    strcpy(temp2, palavra2);
    
    //contador de acentos encontrados
    int achouAcentoI;
    int achouAcentoK;
    int achouAcento;

    for(int i =0, k=0; i<strlen(palavra1) && k<strlen(palavra2); i++, k++){
        achouAcentoI =0;
        achouAcentoK =0;
        achouAcento =0;
        
        if(temp1[i]==-61||temp2[i]==-61){ // tratamos acento na palavra 1
            
            //achou acento na palavra1
            if(temp1[i]==-61){
                temp1[i] = substituiAcento(temp1[i+1]);
                i++;
                achouAcentoI++;
                achouAcento++;
            }
            
            //achou acento na palavra2
            if(temp2[k]==-61){
                temp2[k] = substituiAcento(temp2[k+1]);
                k++;
                achouAcentoK++;
                achouAcento++;
            }
            
            //Se achou acento na palavra1
            if(achouAcentoI){
                //Se achou acento na palavra2
                if(achouAcentoK){
                    
                    //Se palavra2 vier antes de palavra1
                    if(tolower(temp1[i-1])>tolower(temp2[k-1]))
                        return 1;
                    //Se palavra1 vier antes de palavra2
                    else if(tolower(temp1[i-1])<tolower(temp2[k-1]))
                        return -1;
                }
                
                //Nao achou acento em palavra2
                else{
                    //Se palavra2 vier antes de palavra1
                    if(tolower(temp1[i-1])>tolower(temp2[k]))
                        return 1;
                    //Se palavra1 vier antes de palavra2
                    else if(tolower(temp1[i-1])<tolower(temp2[k]))
                        return -1;
                }
            }
            
            //Se achou acento em palavra2 e não achou em palavra1
            if(achouAcentoK && !achouAcentoI){
                //Se palavra2 vier antes de palavra1
                if(tolower(temp1[i])>tolower(temp2[k-1]))
                        return 1;
                //Se palavra1 vier antes de palavra2
                else if(tolower(temp1[i])<tolower(temp2[k-1]))
                    return -1;
            }

            //Se não achou acento
            if(!achouAcento){
                //Se palavra2 for menor que palavra1
                if(tolower(temp1[i])>tolower(temp2[k]))
                    return 1;
                //Se palavra1 vier antes de palavra2
                else if(tolower(temp1[i])<tolower(temp2[k]))
                    return-1;
            }
            continue;
        }
        //caso palavra2 venha antes de palavra1
        if ( tolower(temp1[i]) > tolower(temp2[k]) )
            return 1;
        
        //caso palavra1 venha antes de palavra2
        else if( tolower(temp1[i]) < tolower(temp2[k]) )
            return -1;
            
        
            
    }
    //Caso palavra2 seja menor do que palavra1 
    if((strlen(palavra1)-achouAcentoI)>(strlen(palavra2)-achouAcentoK))
        return 1;
    //Caso palavra1 seja menor do que palavra2
    else if((strlen(palavra1)-achouAcentoI)<(strlen(palavra2)-achouAcentoK))
        return -1; 
    
    //retorna strings iguais
    return strcmp(palavra1, palavra2);
    
    
}
