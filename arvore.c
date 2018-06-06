/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

arvore.c: implementações para arvore genérica
********************************************/

#include "arvore.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct arvore
{
    Arvore* esq;
    Arvore* dir;
    unsigned char* caracter;
    int ocorrencias;
};

// Criando uma folha (árvore sem nós filhos):
Arvore* Arvore_CriaFolha(unsigned char* caracter, int ocorrencias)
{
    Arvore* a = (Arvore*)malloc(sizeof(Arvore)); // Alocando espaço na memória dinamicamente
    // Populando os campos da estrutura:
    a->caracter = (unsigned char*)malloc(9);
    strcpy(a->caracter,caracter);
    a->ocorrencias = ocorrencias;
    a->dir = a->esq = NULL;
    return a; // Retornando o nó
}

// Criando uma árvore com nós filhos:
Arvore* Arvore_CriaArvore(int ocorrencias, Arvore* esq, Arvore* dir)
{
    Arvore* a = (Arvore*)malloc(sizeof(Arvore)); // Alocando espaço na memória dinamicamente
    // Populando os campos da estrutura:
    a->caracter = NULL;
    a->ocorrencias = ocorrencias;
    a->esq = esq;
    a->dir = dir;
    return a; // Retornando o nó
}

// Retorna o caracter de uma árvore:
unsigned char* Arvore_Caracter(Arvore* arvore)
{
    return arvore->caracter;
}

// Retorna o valor ocorrências de uma árvore:
int Arvore_Ocorrencias(Arvore* arvore)
{
    return arvore->ocorrencias;
}

// Retorna a árvore da direita de uma árvore
Arvore* Arvore_ArvoreDireita(Arvore* arvore)
{
    return arvore->dir;
}

// Retorna a árvore da esquerda de uma árvore
Arvore* Arvore_ArvoreEsquerda(Arvore* arvore)
{
    return arvore->esq;
}

// Verificando se uma árvore é um nó folha:
int Arvore_EhFolha(Arvore* arvore)
{
    return (arvore->esq == NULL && arvore->dir == NULL);
}

// Calculando o número de nós folhas:
int Arvore_NumFolhas(Arvore* arvore)
{
    if (Arvore_EhFolha(arvore))
        return 1;
    return Arvore_NumFolhas(arvore->esq) + Arvore_NumFolhas(arvore->dir);
}

// Verificando se um caracter pertence a algum nó de uma árvore:
int Arvore_Pertence(Arvore* raiz, unsigned char* c)
{
    if(raiz == NULL)
        return 0;
    if (Arvore_EhFolha(raiz))
    {
        if(!strcmp(raiz->caracter,c))
            return 1;
        else
            return 0;
    }
    return Arvore_Pertence(raiz->esq, c) || Arvore_Pertence(raiz->dir, c);
}

// Verificando o caminho até um determinado nó da árvore
void Arvore_Caminho(bitmap* map, Arvore* raiz, unsigned int* pos, unsigned char* c)
{
    Arvore* a = raiz;
    while(!Arvore_EhFolha(a)) // enquanto não for um nó folha
    {
        if(Arvore_Pertence(a->esq,c)) // se estiver na árvore da esquerda
        {
            bitmapSetBit(map,*pos,0); // acrescenta 0 (codigo para esquerda) no bitmap na posição especificada
            *pos += 1; // incrementa posição
            a = a->esq; // continua o caminho na árvore da esquerda
        }
        else // se estiver na árvore da direita
        {
            bitmapSetBit(map,*pos,1); // acrescenta 1 (codigo para direita) no bitmap na posição especificada
            *pos += 1; // incrementa posição
            a = a->dir; // continua o caminho na árvore da direita
        }
    }
}

// Apagando a árvore:
Arvore* Arvore_DestroiArvore(Arvore* raiz)
{
    if(Arvore_EhFolha(raiz)) // Verificando se a raiz é folha
    {
        free(raiz->caracter); // Liberando caracter
    }
    else
    {
        if (raiz->esq != NULL)
            Arvore_DestroiArvore(raiz->esq); // Destruindo a subárvore da esquerda
        if (raiz->dir != NULL)
            Arvore_DestroiArvore(raiz->dir); // Destruindo a subárvore da direita
    }
    free(raiz); // Liberando árvore
    return NULL;
}