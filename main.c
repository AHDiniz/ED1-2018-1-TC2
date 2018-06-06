/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

main.c: ponto de entrada do programa
********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compactador.h"

static char* TrocaTXT(char*);

int main(int argc, char *argv[])
{
    // Verificando entrada
    if(!strcmp(argv[1],"0")) // compactar
    {
        Arvore* arvHuff = Compactador_MontaArvoreHuffman(argv[2]); // criando a árvore de Huffman

        // Preparando o nome do arquivo de escrita
        char *novoArquivo = TrocaTXT(argv[2]); // trocando .txt por .comp

        Compactador_Compacta(arvHuff, argv[2], novoArquivo); // compactando e impimindo num arquivo com o novo nome

        free(novoArquivo); // liberando nome
        Arvore_DestroiArvore(arvHuff); // liberando árvore
    }
    else
    {
        if(argv[1] == '1') // descompactar
        {

        }
        else // caso entrada seja diferente imprime mensagem de erro
        {
            printf("ERRO: Formato incorreto\nPrimeiro parametro deve ser 1(compactar) ou 0(descompactar)\n");
        }
    }

    return 0; // fim do programa
}

// Auxiliar que troca terminação .txt por .comp
static char* TrocaTXT(char* string)
{
    int i; // variável de incrementação
    int len = strlen(string); // tamanho da string fonte
    char *aux = (char*) malloc(len +1); // string auxíliar

    // Inverte a string
    for(i = 0 ; i < len ; i++)
    {
        aux[i] = string[len -1 -i];
    }
    aux[len -1] = '\0';

    // Encontra a posição do ultimo ponto na string
    int pos = (int) (strchr(aux,'.') -aux);
    pos = len -pos;

    free(aux); // liberando auxíliar

    string[pos] = '\0'; // encurtando string
    aux = (char*) malloc(pos +5); // realocando aux
    strcpy(aux,string); // copiando string
    strcat(aux,"comp"); // acrescentando "comp" a aux

    return aux;
}