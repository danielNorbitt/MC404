#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*
Exemplo de erros:
const char* get_error_string (enum errors code) {
    switch (code) {
        case ERR_HEXADECIMAL_NAO_VALIDO:
            return "LEXICO: Numero Hexadecimal Inválido!";
        case ERRO_ROTULO_JA_DEFINIDO:
            return "GRAMATICAL: ROTULO JA FOI DEFINIDO!";
        case ERR_DIRETIVA_NAO_VALIDA:
            return "LEXICO: Diretiva não válida";
*/

/*
    ---- Você Deve implementar esta função para a parte 1.  ----
    Essa função é chamada pela main em main.c
    Entrada da função: arquivo de texto lido e seu tamanho
    Retorna:
        * 1 caso haja erro na montagem; (imprima o erro em stderr)
        * 0 caso não haja erro.         (Caso não haja erro, na parte 1, ao retornar desta função, a lista de Tokens (adicionados utilizando a função adicionarToken()) é impressa)
*/

// tipo booleano para facilitar a compreensao
typedef enum Boolean {
    false = 0,true
}Boolean;

// Calcula o tamanho de uma palavra, sem contar o '\0'
int len(char palavra[]){
    int tamanho = 0;
    for (int i = 0; palavra[i] != '\0'; i++){
       tamanho++;
    }
    return tamanho;
}

Boolean isIntrucaoValido(char palavra[]){
    Boolean valido = true;
    char funcoesMax[][8] = {"LD","LDINV","LDABS","LDMQ","LDMQMX","STORE","JUMP","JGE","ADD","ADDABS","SUB","SUBABS","MULT","DIV","LSH","RSH","STOREND"};
    char funcoesMin[][8] = {"ld","ldinv","ldabs","ldmq","ldmqmx","store","jump","jge","add","addabs","sub","subabs","mult","div","lsh","rsh","storend"};

    int achei = -1;
    for (int i = 0; i < 17; i++){
        if(!strcmp(palavra,funcoesMax[i]) || !strcmp(palavra,funcoesMin[i])) 
            achei = i;
    }
    switch (achei){
    case -1: //TODO MUDAR ISSO PARA O DEFAULT
        valido = false;
        break;
    default:
        break;
    }
    return valido;
}
Boolean isDiretivaValido(char palavra[] ){
    Boolean valido = true;
    int achei = -1;
    char diretivasValidasMin[][7] = {".set",".org",".align",".wfill",".word"};
    char diretivasValidasMax[][7] = {".SET",".ORG",".ALIGN",".WFILL",".WORD"};
    for (int i = 0; i < 5; i++){
        if(!strcmp(palavra,diretivasValidasMin[i]) || !strcmp(palavra,diretivasValidasMax[i])) 
            achei = i;
    }
    switch (achei){
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    default:
        valido = false;
        break;
    }
    return valido;
}
Boolean isNomeValido(char palavra[] ){
    Boolean valido = true;

    for (int i = 0; i < len(palavra); i++){
        if (!isalnum(palavra[i]))
            if(palavra[i] != '_')
                valido = false;
    }
    if (!isalpha(palavra[0]) && palavra[0] != '_'){
        valido = false;
    }
    return valido;
}
Boolean isRotuloValido(char palavra[]){
    Boolean valido = true;
    int count2p = 0;
    for (int i = 0; i < len(palavra); i++){
       if(palavra[i] == ':')
            count2p++;
    }
    if(count2p != 1){ // erro 
        valido = false;
    }
    if (palavra[len(palavra)-1] != ':'){
        valido = false;
    }
    if(!isalpha(palavra[0]) && palavra[0] != '_'){
        valido = false;
    }
    for (int i = 0; i < len(palavra)-1; i++){
        if (!isalnum(palavra[i]))
            if(palavra[i] != '_')
                valido = false;      
    }
    return  valido;
}
Boolean isHexadecimalValido(char palavra[]){
    Boolean valido = true;    
    if (len(palavra) > 12 || len(palavra) < 3)
        valido = false;
    if(palavra[0] != '0' && (palavra[1] != 'x' || palavra[1] != 'X'))
        valido = false;
    for (int i = 2; i < len(palavra); i++){
        if(!(palavra[i] >= '0'&& palavra[i] <= '9') && !(palavra[i] >= 'A'&& palavra[i] <= 'F'))
            valido = false;
    }
    return  valido;
}
Boolean isDecimalValido(char palavra[]){
    Boolean valido = true;
    for (int i = 0; i < len(palavra); i++){
       if (!isdigit(palavra[i]))
          valido = false; 
    }
    return valido;
}

int qualToken(char palavra[]){
    if (isDiretivaValido(palavra)){
        return Diretiva;
    }
    if (isIntrucaoValido(palavra)){
        return Instrucao;
    }
    if (isRotuloValido(palavra)){
        return DefRotulo;
    }
    if (isHexadecimalValido(palavra)){
        return Hexadecimal;
    }
    if (isDecimalValido(palavra)){
        return Decimal;
    }
    if (isNomeValido(palavra)){
        return Nome;
    }
    return false;
}

int processarEntrada(char* entrada, unsigned tamanho){
    // contador de linhas
    int countLinha = 1;
    // palavra que esta sendo analizada
    char palavra[65];
    char *novaPalavra;
    // boolean que  controla oq é um comentario
    Boolean isComentario = false;
    TipoDoToken  token;
    for (int i = 0; i < tamanho; i++){
        if(entrada[i] == '\n')
            isComentario = false;  
        if (entrada[i] != ' ' && entrada[i] != '\n'){
            for (int j = i; entrada[j] != '\0'; j++){
                if(entrada[j] == '#')
                    isComentario = true;
                if ((entrada[j] == ' ' || entrada[j] == '\n' ) && !isComentario){
                    for (int k = i; k <= j; k++)
                        // pega a palavra separada por espaço
                        palavra[k-i] = k != j ? entrada[k] : '\0' ;
                    token = qualToken(palavra);   
                    if(token){
                        novaPalavra = malloc(sizeof(char)*65);
                        strcpy(novaPalavra, palavra);
                        adicionarToken(token,novaPalavra,countLinha);
                    }else{
                        fprintf(stderr,"ERRO LEXICO: palavra inválida na linha %d!\n",countLinha);
                        return 1;
                    }
                    i = j;
                    break;
                }
            }
        }
        if (entrada[i] == '\n') 
            countLinha++;
    }

    return 0;
}
// uma linha -> começa diretiva ou rotulo ou instrucao
// se for rotulo -> direritva ou instrucao
// se for instrucao 
    // rsh / lsh / ldmqmx -> nao pedem nada 
    // resto pede hex / decimal (0:2^31-1) -> passar do range ERRO GRAMATICAL / nome
// diretiva ver cada caso
// else erro gramatical