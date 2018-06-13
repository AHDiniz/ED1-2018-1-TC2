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
    if(!strcmp(argv[1],"-c")) // compactar
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
        if(strcmp(argv[1], "-d")) // descompactar
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
    int pos; // variável de incrementação
    int len = strlen(string); // tamanho da string fonte
    char *aux; // string auxíliar

    //  Encontra a posição do ultimo ponto na string
    for(pos = len -1 ; pos >= 0 ; pos--)
    {
        if(string[pos] == '.')
        {
            break;
        }
    }

    aux = (char*) malloc(pos +6); // alocando aux
    strcpy(aux,string); // copiando string
    aux[pos] = '\0'; // encurtando aux
    strcat(aux,".comp"); // acrescentando ".comp" a aux

    return aux;
}