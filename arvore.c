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
    unsigned char caracter;
    int ocorrencias;
};

// Criando uma folha (árvore sem nós filhos):
Arvore* Arvore_CriaFolha(unsigned char caracter, int ocorrencias)
{
    Arvore* a = (Arvore*)malloc(sizeof(Arvore)); // Alocando espaço na memória dinamicamente
    // Populando os campos da estrutura:
    a->caracter = caracter;
    a->ocorrencias = ocorrencias;
    a->dir = a->esq = NULL;
    return a; // Retornando o nó
}

// Criando uma árvore com nós filhos:
Arvore* Arvore_CriaArvore(unsigned char caracter, int ocorrencias, Arvore* esq, Arvore* dir)
{
    Arvore* a = (Arvore*)malloc(sizeof(Arvore)); // Alocando espaço na memória dinamicamente
    // Populando os campos da estrutura:
    a->caracter = caracter;
    a->ocorrencias = ocorrencias;
    a->esq = esq;
    a->dir = dir;
    return a; // Retornando o nó
}

// Retorna o caracter de uma árvore:
unsigned char Arvore_Caracter(Arvore* arvore)
{
    return arvore->caracter;
}

// Retorna o valor ocorrências de uma árvore:
int Arvore_Ocorrencias(Arvore* arvore)
{
    return arvore->ocorrencias;
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
    if (raiz->caracter == c)
        return 1;
    return Arvore_Pertence(raiz->esq, c) || Arvore_Pertence(raiz->dir, c);
}

// Caminho da raiz até o nó:


// Apagando a árvore:
Arvore* Arvore_DestroiArvore(Arvore* raiz)
{
    if (Arvore_EhFolha(raiz)) // Verificando se é um nó folha
        free(raiz);
    else // Se não for um nó folha
    {
        if (raiz->esq != NULL)
            Arvore_DestroiArvore(raiz->esq); // Destruindo a subárvore da esquerda
        if (raiz->dir != NULL)
            Arvore_DestroiArvore(raiz->dir); // Destruindo a subárvore da direita
        free(raiz);
    }
    return NULL;
}