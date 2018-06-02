/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

compactador.c: implementações para compactador
********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lista.h"
#include "bitmap.h"
#include "compactador.h"

#define ASCII_TAM 256

int i; // Variavel global de incrementação

/**
 * Função auxiliar que converte um caracter sem sinal para binário:
 * Input: bitmap que armazenará o binário e um caracter sem sinal;
 * Output: bitmap contendo o caracter convertido (posição 0 mais significativa);
 * Condições: bitmap existe e tem tamanho máximo >= 8;
 * Efeitos Colaterais: bitmap contem o binário de 8 bits;
*/
static void ConverteParaBinario(bitmap* map, unsigned char a)
{
    unsigned int num; // armazena o valor do caracter em inteiro decimal

    // Realizando divisões por 2 e armazenado os restos no bitmap
    for(i = 7, num = (unsigned int) a ; i >= 0 ; num /= 2, i--)
    {
        bitmapSetBit(map,i,num % 2);
    }
}

/**
 * Função auxiliar que converte um binário para caracter sem sinal:
 * Input: bitmap contendo um binário de 8 bits;
 * Output: o caracter convertido;
 * Condições: bitmap existente e válido;
 * Efeitos Colaterais: nenhum;
*/
static unsigned char ConverteParaCharacter(bitmap* map)
{
    unsigned int num; // armazena o valor do caracter em inteiro decimal

    // Percorrendo os 8 primeiros bits do bitmap
    for(i = 0, num = 0 ; i < 8 ; i++)
    {
        if(bitmapGetBit(*map, i) == 1) // se o bit for 1
        {
            num += pow(2.0, 7 - i); // soma à conversão a respectiva potência de 2
        }
    }

    return (unsigned char) num; // retorna caracter sem sinal
}

/**
 * Função auxiliar que identifica os pesos de cada caracter do arquivo:
 * Input: nome do arquivo;
 * Output: vetor com os pesos de cada caracter (identificados pela sua posição no vetor);
 * Condições: arquivo existe;
 * Efeitos Colaterais: nenhum;
*/
static int* MontaVetorPesos(char* arquivo)
{
    int pesos[ASCII_TAM]; // vetor que guarda o peso do caracter na masma posição de sua posição da tabela ASCII
    unsigned char c; // auxiliar que guarda o caracter

    // Inicializando ocorrencias
    for(i = 0 ; i < ASCII_TAM ; i++)
    {
        pesos[i] = 0;
    }

    FILE *input = fopen(arquivo, "r"); // abrindo arquivo

    c = fgetc(input); // selecionando primeiro caracter
    while(c != EOF) // percorrendo todo arquivo
    {
        pesos[(int) c]++; // contando as ocorrencias de cada caracter
        c = fgetc(input); // atualizando caracter
    }

    fclose(input); // fechando arquivo
    return pesos;
}

/**
 * Função auxiliar que insere ordenadamente uma árvore numa lista:
 * Input: lista e árvore;
 * Output: nenhum;
 * Condições: lista e árvore existem;
 * Efeitos Colaterais: árvore inclusa à lista mantendo-a ordenada;
*/
static void InsereArvoreOrdenado(Lista* l, Arvore* arvore)
{
    for(i = 0 ; i < Lista_TamanhoLista(l) ; i++) // Varrendo toda a lista
    {
        // Interrompendo o loop ao encontrar uma árvore com campo 'ocorrencias' maior do que o da árvore a ser inserida
        if(Arvore_Ocorrencias((Arvore*) Lista_AchaItem(l,i)) > Arvore_Ocorrencias(arvore))
        {
            break;
        }
    } // Ao fim do loop, 'i' será a posição onde deve ser inserida a árvore

    Lista_ListaAdd(l, Lista_NovoItem("Arvore", arvore), i); // Inserindo árvore em sua devida posição
}

/**
 * Função auxiliar que imprime a árvore de Huffman no arquivo especificado:
 * Input: árvore e arquivo;
 * Output: nenhum;
 * Condições: árvore e arquivo existem;
 * Efeitos Colaterais: árvore é codificada e impressa no arquivo;
*/
static void ImprimeArvore(Arvore* arvore, FILE* output)
{
    if(Arvore_EhFolha(arvore))
    {
        fprintf(output, "0");
        for(i = 0 ; i < 8 ; i++)
        {
            fprintf(output, "%u", );
        }
    }
}

// Lendo arquivo e montando a árvore de Huffman:
Arvore* Compactador_MontaArvoreHuffman(char* arquivo)
{
    // Variáveis auxiliares para criação da árvore
    Arvore *t1;
    Arvore *t2;
    Arvore *tr;

    int *pesos = MontaVetorPesos(arquivo); // Encontrando o numero de ocorrencias de cada caracter no arquivo

    Lista *listaArvores = Lista_NovaLista("Arvore"); // Inicializando a lista de árvores

    for(i = 0 ; i < ASCII_TAM ; i++) // Varrendo todo vetor de caracteres
    {
        if(pesos[i] > 0) // Encontrando os caracteres presentes no arquivo
        {
            // Inserindo esses caracteres numa folha e a folha na lista de árvores (ordenada segundo os pesos)
            InsereArvoreOrdenado(listaArvores, Arvore_CriaFolha(i,pesos[i]));
        }
    }

    // Criando a árvore de Huffman
    while(Lista_TamanhoLista(listaArvores) > 1) // inicio do loop
    {
        t1 = (Arvore*) Lista_AchaItem(listaArvores, 0); // t1 recebe a primeira árvore da lista
        t2 = (Arvore*) Lista_AchaItem(listaArvores, 1); // t2 recebe a segunda árvore da lista

        // criando nova árvore cujo peso é a soma dos pesos de t1 e t2 e suas árvores esquerda e direita são t1 e t2, respectivamente
        tr = Arvore_CriaArvore('\0', Arvore_Ocorrencias(t1) + Arvore_Ocorrencias(t2) ,t1,t2);

        Lista_ListaRemove(listaArvores,0,NULL); // removendo t1 da lista
        Lista_ListaRemove(listaArvores,1,NULL); // removendo t2 da lista

        InsereArvoreOrdenado(listaArvores,tr); // inserindo tr na lista ordenadamente
    } // loop encerra quando só houver uma árvore na lista. Essa é a árvore de Huffman

    tr = (Arvore*) Lista_AchaItem(listaArvores,0); // tr recebe a árvore de Huffman
    Lista_DestroiLista(listaArvores,NULL); // liberando memória da lista

    return tr;
}

/**
 * Função de impressão do arquivo compactado:
 * Input: arvore de Huffman para o arquivo e nome do arquivo a ser compactado (com extensão .txt);
 * Output: arquivo compactado;
 * Condições: arvore válida e arquivo existe;
 * Efeitos Colaterais: nenhum;
*/
void Compactador_Compacta(Arvore* arvoreHuffman, char* arquivo);

/**
 * Função de descompactação de um arquivo:
 * Input: nome do arquivo (com extensão .comp);
 * Output: arquivo descompactado;
 * Condições: arquivo existe;
 * Efeitos Colaterais: nenhum;
*/
void Compactador_Descompacta(char* arquivo);