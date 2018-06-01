/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

compacta.c: implementações para compactador
********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "bitmap.h"
#include "compacta.h"

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
    bitmap map = bitmapInit(8);
    int num;

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
    unsigned char pesos[ASCII_TAM];
    unsigned char c;

    for(i = 0 ; i < ASCII_TAM ; i++)
    {
        pesos[i] = 0;
    }

    FILE *input = fopen(arquivo, "r");

    c = fgetc(input);
    while(c != EOF)
    {
        pesos[(int) c]++;
        c = fgetc(input);
    }

    fclose(input);
    return pesos;
}

/**
 * Função auxiliar que identifica os pesos de cada caracter do arquivo:
 * Input: nome do arquivo;
 * Output: vetor com os pesos de cada caracter (identificados pela sua posição no vetor);
 * Condições: arquivo existe;
 * Efeitos Colaterais: nenhum;
*/
void InsereArvoreOrdenado(Lista* l, Arvore* arvore)
{
    Arvore *p;
    Arvore *ant;

    if(Lista_TamanhoLista(l) == 0)
    {
        i = 0;
    }
    else
    {
        if(Arvore_Ocorrencias(arvore) < Arvore_Ocorrencias((Arvore*) Lista_AchaItem(l,0)))
        {
            
        }
    }
}

/**
 * Função de criação da árvore de Huffman:
 * Input: nome do arquivo;
 * Output: ponteiro para a árvore;
 * Condições: arquivo existe;
 * Efeitos Colaterais: nenhum;
*/
Arvore* Compacta_MontaArvoreHuffman(char* arquivo)
{
    unsigned char *pesos = MontaVetorPesos(arquivo);

    Lista *listaArvores = Lista_NovaLista("Arvore");

    for(i = 0 ; i < ASCII_TAM ; i++)
    {
        if(pesos[i] > 0)
        {
            InsereArvoreOrdenado(listaArvores, Arvore_CriaFolha(i,pesos[i]));
        }
    }

    return ;
}

/**
 * Função de impressão do arquivo compactado:
 * Input: arvore de Huffman para o arquivo e nome do arquivo a ser compactado (com extensão .txt);
 * Output: arquivo compactado;
 * Condições: arvore válida e arquivo existe;
 * Efeitos Colaterais: nenhum;
*/
void Compacta_ImprimeArquivo(Arvore* arvorHuffman, char* arquivo);