/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

main.c: ponto de entrada do programa
********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compactador.h"

static int VerificaTXT(char*);
static void TrocaTXT(char*,char*);

int main(int argc, char *argv[])
{
    // Verificando entrada
    if(argv[1] == '0') // compactar
    {
        // Verificando se o nome do arquivo termina em .txt
        if(!VerificaTXT(argv[2]))
        {
            printf("ERRO: arquivo deve ser tipo txt"); // caso não, exibe mensagem de erro
            return 0;                                  // e aborta o programa
        }

        Arvore* arvHuff = Compactador_MontaArvoreHuffman(argv[2]); // criando a árvore de Huffman

        // Preparando o nome do arquivo de escrita
        char *novoArquivo = (char*) malloc(strlen(argv[2]) +2); // alocando espaço para o nome
        TrocaTXT(novoArquivo, argv[2]); // trocando .txt por .comp

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

// Auxiliar que verifica se uma string termina em .txt
static int VerificaTXT(char* string)
{
    int tamanho = strlen(string); // numero de caracteres na string

    if(tamanho <= 4) // se houver 4 ou menos caracteres, .txt não eh sufixo
    {
        return 0; // retorno de caráter booleano
    }

    // verifica se os 4 últimos caracteres são .txt
    if(strcmp( string + tamanho -4 ,".txt") == 0)
    {
        return 1; // caso sim, retorno afirmativo
    }

    return 0; // caso contrário o sufixo não eh .txt
}

// Auxiliar que troca terminação .txt por .comp
static void TrocaTXT(char* novaString, char* string)
{
    strcpy(novaString,string); // copiando string
    novaString[strlen(string) -3] = '\0'; // invalidando três ultimos caracteres (txt)
    strcat(novaString,"comp"); // acrescentando "comp"
}