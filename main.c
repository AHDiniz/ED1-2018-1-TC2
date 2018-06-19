/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

main.c: ponto de entrada do programa
********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compactador.h"

static char* AddComp(char* string);
static char* RemComp(char* string);

int main(int argc, char *argv[])
{
    // Verificando entrada
    if(!strcmp(argv[1],"-c")) // compactar
    {
        // Criando a árvore de Huffman
        printf("Arvore...\n");
        Arvore* arvHuff = Compactador_MontaArvoreHuffman(argv[2]);

        // Preparando o nome do arquivo de escrita
        char *novoArquivo = AddComp(argv[2]); // acrescentando terminação .comp

        printf("Compactando...\n");
        Compactador_Compacta(arvHuff, argv[2], novoArquivo); // compactando e impimindo num arquivo com o novo nome

        free(novoArquivo); // liberando nome
        Arvore_DestroiArvore(arvHuff); // liberando árvore
    }
    else
    {
        if(!strcmp(argv[1], "-d")) // descompactar
        {
            char *novoArquivo = RemComp(argv[2]); // removendo terminação .comp
            printf("Descompactando...\n");
            Compactador_Descompacta(argv[2], novoArquivo); // descompactando e imprimindo o arquivo com o novo nome
            printf("Descompact fim.\n");
            free(novoArquivo); // liberando nome
        }
        else // caso entrada seja diferente imprime mensagem de erro
        {
            printf("ERRO: Formato incorreto\nPrimeiro parametro deve ser -c(compactar) ou -d(descompactar), seguido do nome do arquivo\n");
        }
    }

    return 0; // fim do programa
}

// Auxiliar que prepara o nome do arquivo compactado
static char* AddComp(char* string)
{
    int pos; // variável de incrementação
    int len = strlen(string); // tamanho da string fonte
    char *novaString; // string retorno

    //  Encontra a posição do ultimo ponto na string
    for(pos = len -1 ; pos >= 0 ; pos--)
    {
        if(string[pos] == '.')
        {
            break;
        }
    }

    novaString = (char*) malloc(len +6); // alocando novaString
    strcpy(novaString,string); // copiando string
    novaString[pos] = '_'; // substituindo '.' por '_'
    strcat(novaString,".comp"); // acrescentando .comp a novaString

    return novaString;
}

// Auxiliar que recupera o nome do arquivo
static char* RemComp(char* string)
{
    int pos; // variável de incrementação
    int len = strlen(string); // tamanho da string fonte
    char *novaString = (char*) malloc(len +1); // string retorno

    //  Encontra a posição do ultimo '_' na string
    for(pos = len -1 ; pos >= 0 ; pos--)
    {
        if(string[pos] == '_')
        {
            break;
        }
    }

    strcpy(novaString,string); // copiando string
    novaString[pos] = '.'; // substituindo '.' por '_'
    novaString[len -5] = '\0'; // removendo .comp de novaString

    return novaString;
}