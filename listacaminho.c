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
    Caminho* cam = (Caminho*) caminho; // convertendo para Caminho

    if(cam != NULL) // medida de segurança
    {
        Lista_DestroiLista(cam->caminho,NULL); // destroi a lista do caminho
        free(cam); // libera o struct
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
    if(Arvore_EhFolha(raiz)) // se for uma folha
    {
        // Cria um struct caminho com o carácter e o caminho até ele
        Caminho* cam = (Caminho*) malloc(sizeof(Caminho));
        cam->caracter = Arvore_Caracter(raiz);
        cam->caminho = Arvore_Caminho(huffman,raiz);
        // Adiciona o struct criado na lista
        Lista_ListaAdd(lista,Lista_NovoItem("Caminho",cam),0);
    }
    else // se não for uma folha aplica a função recursivamente a suas árvores
    {
        CriaCaminhos(lista,huffman,Arvore_ArvoreDireita(raiz)); // aplicando recursão a direita
        CriaCaminhos(lista,huffman,Arvore_ArvoreEsquerda(raiz)); // aplicando recursão a esquerda
    }
}

// Cria a lista de caminhos da árvore de Huffman
Lista* ListaCaminho_CriaLista(Arvore* huffman)
{
    Lista *novaLista = Lista_NovaLista("Caminho"); // cria a lista

    CriaCaminhos(novaLista,huffman,huffman); // insere cada carácter da árvore na lista com seus respectivos caminhos

    return novaLista;
}

// Retorna o caminho de um carácter
Lista* ListaCaminho_Caminho(Lista* listaCam, unsigned char caracter)
{
    int i; // variável de incrementação
    Caminho* p; // variável auxiliar de busca
    for(i = 0 ; i < Lista_TamanhoLista(listaCam) ; i++) // varre toda a lista
    {
        p = Lista_AchaItem(listaCam,i); // auxiliar recebe o struct caminho de um carácter
        if(p->caracter == caracter) // se for o carácter buscado interrompe a busca
        {
            break;
        }
    }
    return p->caminho; // retorna o caminho
}

// Destroi a lista de caminhos
Lista* ListaCaminho_DestroiLista(Lista* lista)
{
    if(lista != NULL) // medida de segurança
    {
        Lista_DestroiLista(lista,DestroiCaminho); // destroi a lista de structs
    }
    return NULL;
}