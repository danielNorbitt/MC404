#include <stdio.h>
#include<string.h>

int len(char palavra[])
{
    int tamanho = 0;
    for (int i = 0; palavra[i] != '\0'; i++)
    {
        tamanho++;
    }
    return tamanho-1;
}

int stringToInt(char palavra[]){
    int res = 0;
    int uni = 1;
    for (int i = len(palavra); i > 0; i--){
        res += (palavra[i]-'0') * uni;
        uni *= 10;
    }
    return res;
}
int  main(){
    
    char aa[] = "2323222";
    printf("%d\n",stringToInt(aa));
    return 0;
}