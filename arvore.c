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
    char caracter;
    int ocorrencias;
};

// Criando uma folha (árvore sem nós filhos):
Arvore* CriaFolha(char caracter, int ocorrencias)
{
    Arvore* a = (Arvore*)malloc(sizeof(Arvore)); // Alocando espaço na memória dinamicamente
    // Populando os campos da estrutura:
    a->caracter = caracter;
    a->ocorrencias = ocorrencias;
    a->dir = a->esq = NULL;
    return a; // Retornando o nó
}

// Criando uma árvore com nós filhos:
Arvore* CriaArvore(char caracter, int ocorrencias, Arvore* esq, Arvore* dir)
{
    Arvore* a = (Arvore*)malloc(sizeof(Arvore)); // Alocando espaço na memória dinamicamente
    // Populando os campos da estrutura:
    a->caracter = caracter;
    a->ocorrencias = ocorrencias;
    a->esq = esq;
    a->dir = dir;
    return a; // Retornando o nó
}

// Verificando se uma árvore é um nó folha:
int EhFolha(Arvore* arvore)
{
    return (arvore->esq == NULL && arvore->dir == NULL);
}

// Calculando o número de nós folhas:
int NumFolhas(Arvore* arvore)
{
    if (EhFolha(arvore))
        return 1;
    return NumFolhas(arvore->esq) + NumFolhas(arvore->dir);
}

// Apagando a árvore:
Arvore* DestroiArvore(Arvore* raiz)
{
    if (EhFolha(raiz)) // Verificando se é um nó folha
        free(raiz);
    else // Se não for um nó folha
    {
        if (raiz->esq != NULL)
            DestroiArvore(raiz->esq); // Destruindo a subárvore da esquerda
        if (raiz->dir != NULL)
            DestroiArvore(raiz->dir); // Destruindo a subárvore da direita
        free(raiz);
    }
    return NULL;
}