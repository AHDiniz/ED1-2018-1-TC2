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

// Apagando a árvore:
Arvore* DestroiArvore(Arvore* raiz)
{
    if (EhFolha(raiz)) // Verificando se é um nó folha
        free(raiz);
    else
    {
        DestroiArvore(raiz->esq);
        DestroiArvore(raiz->dir);
        free(raiz);
    }
    return NULL;
}