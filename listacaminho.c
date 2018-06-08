/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

listacaminho.c: implementações para lista dos caminhos
********************************************/

#include "listacaminho.h"
#include <stdio.h>
#include <stdlib.h>

struct caminho
{
    unsigned char caracter;
    Lista* caminho;
};

/**
 * Função auxiliar que destrói um struct caminho da memória:
 * Input: ponteiro genérico para o struct;
 * Output: nenhum;
 * Condições: struct existente e alocado;
 * Efeitos Colaterais: o struct será apagado da memória;
*/
static void DestroiCaminho(void* caminho)
{
    Caminho* cam = (Caminho*) caminho;

    if(cam != NULL)
    {
        Lista_DestroiLista(cam->caminho,NULL);
        free(cam);
    }
}

/**
 * Função auxiliar de criação dos caminhos da lista:
 * Input: ponteiros para a lista, para a árvore de Huffman e para a árvore raiz;
 * Output: nenhum;
 * Condições: árvores e lista existem;
 * Efeitos Colaterais: lista contem os structs caminho das folhas da árvore de Huffman;
*/
static void CriaCaminhos(Lista* lista, Arvore* huffman, Arvore* raiz)
{
    if(Arvore_EhFolha(raiz))
    {
        Caminho* cam = (Caminho*) malloc(sizeof(Caminho));
        cam->caracter = Arvore_Caracter(raiz);
        cam->caminho = Arvore_Caminho(huffman,raiz);
        Lista_ListaAdd(lista,Lista_NovoItem("Caminho",cam),0);
    }
    else
    {
        CriaCaminhos(lista,huffman,Arvore_ArvoreDireita(raiz));
        CriaCaminhos(lista,huffman,Arvore_ArvoreEsquerda(raiz));
    }
}

// Cria a lista de caminhos da árvore de Huffman
Lista* ListaCaminho_CriaLista(Arvore* huffman)
{
    Lista *novaLista = Lista_NovaLista("Caminho");

    CriaCaminhos(novaLista,huffman,huffman);

    return novaLista;
}

// Retorna o caminho de um carácter
Lista* ListaCaminho_Caminho(Lista* listaCam, unsigned char caracter)
{
    int i;
    Caminho* p;
    for(i = 0 ; i < Lista_TamanhoLista(listaCam) ; i++)
    {
        p = Lista_AchaItem(listaCam,i);
        if(p->caracter == caracter)
        {
            break;
        }
    }
    return p->caminho;
}

// Destroi a lista de caminhos
Lista* ListaCaminho_DestroiLista(Lista* lista)
{
    if(lista != NULL)
    {
        Lista_DestroiLista(lista,DestroiCaminho);
        free(lista);
    }
    return NULL;
}