/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

compactador.c: implementações para compactador
********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "bitmap.h"
#include "compactador.h"

#define ASCII_TAM 256

int i; // Variavel global de incrementação

/**
 * Função auxiliar que converte um caracter sem sinal para binário:
 * Input: caracter sem sinal;
 * Output: bitmap contendo o caracter convertido;
 * Condições: nenhuma;
 * Efeitos Colaterais: nenhum;
*/
bitmap ConverteParaBinario(unsigned char a)
{
    bitmap map = bitmapInit(8); // inicializando bitmap
    int num; // armazena o valor do caracter em inteiro decimal

    // Realizando divisões por 2 e armazenado o resto no bitmap
    for(i = 8, num = (int) a ; num > 0 ; num / 2, i--)
    {
        bitmapSetBit(&map,i,num % 2);
    }

    return map;
}

/**
 * Função auxiliar que identifica os pesos de cada caracter do arquivo:
 * Input: nome do arquivo;
 * Output: vetor com os pesos de cada caracter (identificados pela sua posição no vetor);
 * Condições: arquivo existe;
 * Efeitos Colaterais: nenhum;
*/
unsigned char* MontaVetorPesos(char* arquivo)
{
    unsigned char pesos[ASCII_TAM]; // vetor que guarda o peso do caracter na masma posição de sua posição da tabela ASCII
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
void InsereArvoreOrdenado(Lista* l, Arvore* arvore)
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

// Criando a árvore de Huffman:
Arvore* Compactador_MontaArvoreHuffman(char* arquivo)
{
    // Variáveis auxiliares para criação da árvore de Huffman
    Arvore *t1;
    Arvore *t2;
    Arvore *tr;

    unsigned char *pesos = MontaVetorPesos(arquivo); // Encontrando o numero de ocorrencias de cada caracter no arquivo

    Lista *listaArvores = Lista_NovaLista("Arvore"); // Preparando a lista de árvores

    for(i = 0 ; i < ASCII_TAM ; i++) // Varrendo todo vetor de caracteres
    {
        if(pesos[i] > 0) // Encontrando os caracteres presentes no arquivo
        {
            // Inserindo esses caracteres numa folha e a folha na lista de árvores (ordenada segundo os pesos)
            InsereArvoreOrdenado(listaArvores, Arvore_CriaFolha(i,pesos[i]));
        }
    }

    // Criando a árvore de Huffman
    while(Lista_TamanhoLista(listaArvores) > 1)
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
void Compactador_Compacta(Arvore* arvorHuffman, char* arquivo);

/**
 * Função de descompactação de um arquivo:
 * Input: nome do arquivo (com extensão .comp);
 * Output: arquivo descompactado;
 * Condições: arquivo existe;
 * Efeitos Colaterais: nenhum;
*/
void Compactador_Descompacta(char* arquivo);