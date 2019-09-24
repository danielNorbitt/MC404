#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


#define Max (int) (2 * pow(10,31))
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


Boolean isIntrucaoValido(char palavra[]){
    Boolean valido = true;
    char funcoesMax[][8] = {"LD","LDINV","LDABS","LDMQ","LDMQMX","STORE","JUMP","JGE","ADD","ADDABS","SUB","SUBABS","MULT","DIV","LSH","RSH","STOREND"};
    char funcoesMin[][8] = {"ld","ldinv","ldabs","ldmq","ldmqmx","store","jump","jge","add","addabs","sub","subabs","mult","div","lsh","rsh","storend"};

    int achei = -1;
    for (int i = 0; i < 17; i++){
        if(!strcmp(palavra,funcoesMax[i]) || !strcmp(palavra,funcoesMin[i])) 
            achei = i;
    }
    if (achei == -1)
        valido = false;
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
    if (achei == -1)
        valido = false;
    return valido;
}
Boolean isNomeValido(char palavra[] ){
    Boolean valido = true;
    for (int i = 0; i < strlen(palavra); i++){
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
    for (int i = 0; i < strlen(palavra); i++){
       if(palavra[i] == ':')
            count2p++;
    }
    if(count2p != 1){
        valido = false;
    }
    if (palavra[strlen(palavra)-1] != ':'){
        valido = false;
    }
    if(!isalpha(palavra[0]) && palavra[0] != '_'){
        valido = false;
    }
    for (int i = 0; i < strlen(palavra)-1; i++){
        if (!isalnum(palavra[i]))
            if(palavra[i] != '_')
                valido = false;      
    }
    return  valido;
}
Boolean isHexadecimalValido(char palavra[]){
    Boolean valido = true;    
    if (strlen(palavra) > 12 || strlen(palavra) < 3)
        valido = false;
    if(palavra[0] != '0' && (palavra[1] != 'x' || palavra[1] != 'X'))
        valido = false;
    for (int i = 2; i < strlen(palavra); i++){
        if(!(palavra[i] >= '0'&& palavra[i] <= '9') && !(palavra[i] >= 'A'&& palavra[i] <= 'F'))
            valido = false;
    }
    return  valido;
}
Boolean isDecimalValido(char palavra[]){
    Boolean valido = true;
    for (int i = 0; i < strlen(palavra); i++){
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
Boolean erro(Token* linha[],int controle);
Boolean maisDeUm(Token *linha[], int controle){
    int countRotulo = 0;
    int countDiretiva = 0;
    int countInstrucao = 0;
    for (int i = 0; i < controle; i++){
        if (linha[i]->tipo == DefRotulo)
            countRotulo++;
        if (linha[i]->tipo == Instrucao)
            countInstrucao++;
        if (linha[i]->tipo == Diretiva)
            countDiretiva++;
    }
    if (countDiretiva > 1 || countInstrucao > 1 || countRotulo > 1){
        return true;
    }
    return false;
}

int processarEntrada(char *entrada, unsigned tamanho){
    // contador de linhas
    int countLinha = 1;
    // palavra que esta sendo analizada
    char palavra[65];
    char *novaPalavra;
    // boolean que  controla oq é um comentario
    Boolean isComentario = false;
    TipoDoToken  tipoToken;
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
                    tipoToken = qualToken(palavra);   
                    if(tipoToken){
                        novaPalavra = malloc(sizeof(char)*65);
                        strcpy(novaPalavra, palavra);
                        adicionarToken(tipoToken,novaPalavra,countLinha);
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
    Token *linhaTokenAtual[7];
    Token *token;
    int controle;
    int linhaAtual = 1;
    while (linhaAtual < countLinha){
        controle = 0;
        for (int i = 0; i < getNumberOfTokens(); i++){
            token = recuperaToken(i);
            if (token->linha == linhaAtual){
                linhaTokenAtual[controle] = token;
                controle++;
            }
        }

        if(controle != 0 && erro(linhaTokenAtual,controle)){
            fprintf(stderr, "ERRO GRAMATICAL: palavra na linha %d!\n", linhaAtual);
            return 1;
        }
        linhaAtual++;
    }
    return 0;
}


Boolean erro(Token* linha[],int controle){
    if(linha[0]->tipo != Diretiva && linha[0]->tipo != DefRotulo && linha[0]->tipo != Instrucao){
        return true;
    }
    if(controle > 4){
        return true;
    }
    for (int i = 0; i < controle; i++){
        switch (linha[i]->tipo){
            case DefRotulo:
                if(i != 0)
                    return true;
                if (controle != 1)
                    if(linha[i+1]->tipo != Diretiva && linha[i+1]->tipo != Instrucao)
                        return true;
                break;
            case Instrucao:{
                char semParametro[6][8] = {"LSH","RSH","LDMQ","lsh","rsh","ldmq"};
                int achei = -1;
                for (int j = 0; j < 6; j++){
                    if(!strcmp(linha[i]->palavra,semParametro[j])) 
                        achei = i;
                }
                if(achei == -1){
                    if (linha[i + 1]->tipo != Hexadecimal && linha[i + 1]->tipo != Nome && linha[i + 1]->tipo != Decimal ){
                        return true;
                    }
                    if (linha[i + 1]->tipo == Decimal){
                        if (atoi(linha[i + 1]->palavra) < 0 || atoi(linha[i + 1]->palavra)>1023){
                            return true;
                        }
                    }
                }else{
                    if(controle - 1 - i != 0){
                        return true;
                    }
                }
                break;
            }
            case Diretiva:
            // .set
                if (linha[i]->palavra[2] == 'e' || linha[i]->palavra[2] == 'E'){
                    if(controle - 1- i != 2){
                        return true;
                    }
                    if (linha[i + 1]->tipo != Nome){
                        return true;
                    }
                    if (linha[i + 2]->tipo != Hexadecimal && linha[i + 2]->tipo != Decimal ){
                        return true;
                    }
                    if (linha[i + 2]->tipo == Decimal){
                        if (atoi(linha[i + 2]->palavra) < 0 || atoi(linha[i + 2]->palavra) > (Max - 1))
                        {
                            return true;
                        }
                    }

                }
                //.wfill
                else if (linha[i]->palavra[2] == 'f' || linha[i]->palavra[2] == 'F'){
                    if(controle - 1- i != 2){
                        return true;
                    }
                    if (linha[i + 1]->tipo != Decimal){
                        return true;
                    }
                    else{
                        if (atoi(linha[i + 1]->palavra) < 1 || atoi(linha[i + 1]->palavra) > 1023){
                            return true;
                        }
                    }
                    if (linha[i + 2]->tipo != Hexadecimal && linha[i + 2]->tipo != Decimal && linha[i + 2]->tipo != Nome){
                        return true;
                    }
                    if (linha[i + 2]->tipo == Decimal){
                        if (atoi(linha[i + 2]->palavra) < (-1 * Max) || atoi(linha[i + 2]->palavra) > (Max - 1)){
                            return true;
                        }
                    }
                }
                // //.org
                else if (linha[i]->palavra[2] == 'r' || linha[i]->palavra[2] == 'R'){
                    if(controle - 1- i != 1){
                        return true;
                    }
                    if (linha[i + 1]->tipo != Decimal && linha[i + 1]->tipo != Hexadecimal){
                        return true;
                    }
                    if (linha[i + 1]->tipo == Decimal){
                        if (atoi(linha[i + 1]->palavra) < 0 && atoi(linha[i + 1]->palavra) > 1023){
                            return true;
                        }
                    }
                }
                // //.align
                else if (linha[i]->palavra[2] == 'l' || linha[i]->palavra[2] == 'L'){
                    if(controle - 1- i != 1){
                        return true;
                    }
                    if (linha[i + 1]->tipo != Decimal){
                        return true;
                    }
                    else{
                        if (atoi(linha[i + 1]->palavra) < 0 || atoi(linha[i + 1]->palavra) > 1023){
                            return true;
                        }
                    }
                }
                // // // .word
                else if (linha[i]->palavra[2] == 'o' || linha[i]->palavra[2] == 'O'){
                    if(controle - 1- i != 1){
                        return true;
                    }
                    if (linha[i + 1]->tipo != Decimal && linha[i + 1]->tipo != Hexadecimal && linha[i + 1]->tipo != Nome && linha[i + 1]->tipo != DefRotulo){
                        return true;
                    }
                    else if(linha[i + 1]->tipo == Decimal){
                        if (atoi(linha[i + 1]->palavra) < (-1 * Max) || atoi(linha[i + 1]->palavra) > Max - 1){

                            return true;
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
    if (maisDeUm(linha,controle)){
        return true;
    }
    return false;
}
// uma linha -> começa diretiva ou rotulo ou instrucao
// se for rotulo -> direritva ou instrucao
// se for instrucao 
    // rsh / lsh / ldmqmx -> nao pedem nada 
    // resto pede hex / decimal (0:2^31-1) -> passar do range ERRO GRAMATICAL / nome
// diretiva ver cada caso
    //  2 parametros
        //  .set -> nome && hex | decimal (0:2^31-1) 
        //  .wfill -> decimal (0:1023) && hex | decimal (-2^31:2^31-1)
    // 1 parametro
        //  .org -> hex | decimal (0:1023)
        //  .align -> decimal (0:1023)
        //  .word hex | decimal (-2^31:2^31-1) | rotulo | nome

// else erro gramatical