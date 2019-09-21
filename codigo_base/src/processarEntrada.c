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
void debug(Token* tokens[],int controle){
    char *tokenNames[] = {
        "Instrucao",
        "Diretiva",
        "Definicao Rotulo",
        "Hexadecimal",
        "Decimal",
        "Nome"};
    printf("======================DEBUG==========================\n");
    for (int i = 0; i < controle; i++){
        printf("Token %d\n", i);
        printf("\tTipo: %d (%s)\n", (int)tokens[i]->tipo, tokenNames[(int)tokens[i]->tipo - (int)Instrucao]);
        printf("\tPalavra: \"%s\"\n", tokens[i]->palavra);
        printf("\tLinha: %u\n", tokens[i]->linha);
    }
    printf("======================DEBUG==========================\n");
}


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

int stringToInt(char palavra[]){
    int res = 0;
    int uni = 1;
    for (int i = len(palavra)-1; i > 0; i--){
        res += (palavra[i] - '0') * uni;
        uni *= 10;
    }
    return res;
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
    if(count2p != 1){
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
Boolean erro(Token* linha[],int controle);

int processarEntrada(char* entrada, unsigned tamanho){
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
    Token *linhaTokenAtual[10];
    Token *token;
    int controle;
    int linhaAtual = 1;
    while (linhaAtual < countLinha){
        controle = 0;
        for (int i = 0; i < getNumberOfTokens(); i++){
            token = recuperaToken(i);
            if(token->linha == linhaAtual){
                linhaTokenAtual[controle] = token;
                controle++;
            }
        }
        if(erro(linhaTokenAtual,controle))
            debug(linhaTokenAtual,controle);
        linhaAtual++;
    }
    return 0;
}


Boolean erro(Token* linha[],int controle){
    int erro = false;
    if(linha[0]->tipo != Diretiva || linha[0]->tipo != DefRotulo || linha[0]->tipo != Instrucao)
        erro = true;
    if(controle > 4)
        erro = true;
    for (int i = 0; i < controle; i++){
        switch (linha[i]->tipo){
            case DefRotulo:
                if(linha[i+1]->tipo != Diretiva || linha[i+1]->tipo != Instrucao)
                    erro = true;
                break;
            case Instrucao:
                char semParametro[][8] = {"LSH","RSH","LDMQMX","lsh","rsh","ldmqmx"};
                int achei = -1;
                for (int i = 0; i < 6; i++){
                    if(!strcmp(linha[i],semParametro[i])) 
                        achei = i;
                }
                if(achei == -1){
                // com parametro
                }else{

                }
                break;
            case Diretiva:
                switch(linha[i]->palavra[2]){
                    // .set
                    case 'e':
                        break;
                    // .wfill
                    case 'f':
                        break;
                    // .org
                    case 'r':
                        break;
                    // .align
                    case 'l':
                        break;
                    // .word
                    case 'o':
                        break;
                }
                break;
            default:
                break;
            }
    }
    // fazer contagem se tem tipo repetido (rotulo,diretiva ou instrucao)

    return erro;
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