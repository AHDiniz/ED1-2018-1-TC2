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

// Função auxiliar para achar caminho:
static void Arvore_CaminhoAux(Arvore* raiz, Arvore* alvo, Lista* caminho)
{
    if (raiz == NULL) // Verificação de exceção
        return;

    if (raiz->caracter == alvo->caracter)
        return;
    else if (Arvore_Pertence(raiz->esq, (unsigned char*) &(alvo->caracter)))
    {
        Item* itemCaminho = Lista_NovoItem("int*", ListaCaminho_CriaInt(0));
        Lista_ListaAdd(caminho, itemCaminho, Lista_TamanhoLista(caminho) - 1);
        Arvore_CaminhoAux(raiz->esq, alvo, caminho);
    }
    else if (Arvore_Pertence(raiz->dir, (unsigned char*) &(alvo->caracter)))
    {
        Item* itemCaminho = Lista_NovoItem("int*", ListaCaminho_CriaInt(1));
        Lista_ListaAdd(caminho, itemCaminho, Lista_TamanhoLista(caminho) - 1);
        Arvore_CaminhoAux(raiz->dir, alvo, caminho);
    }
}

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
unsigned char Arvore_Caracter(Arvore* arvore)
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
        if(raiz->caracter == *c)
            return 1;
        else
            return 0;
    }
    return Arvore_Pertence(raiz->esq, c) || Arvore_Pertence(raiz->dir, c);
}

// Verificando o caminho até um determinado nó da árvore
Lista* Arvore_Caminho(Arvore* raiz, Arvore* alvo)
{
    Lista* caminho = ListaCaminho_CriaListaInt();

    Arvore_CaminhoAux(raiz, alvo, caminho);

    return caminho;
}

// Apagando a árvore:
Arvore* Arvore_DestroiArvore(Arvore* raiz)
{
    if (!Arvore_EhFolha(raiz))
    {
        if (raiz->esq != NULL)
            Arvore_DestroiArvore(raiz->esq); // Destruindo a subárvore da esquerda
        if (raiz->dir != NULL)
            Arvore_DestroiArvore(raiz->dir); // Destruindo a subárvore da direita
    }
    free(raiz); // Liberando árvore
    return NULL;
}










// Funções e estruturas auxiliares utilizadas na procura do caminho da raiz de uma árvore até um nó dessa árvore:

struct caminho
{
    unsigned char caracter;
    Lista *caminho;
};

// Função auxiliar que destrói um struct caminho da memória:
static void DestroiCaminho(void *caminho)
{
    Caminho *cam = (Caminho *)caminho; // convertendo para Caminho

    if (cam != NULL) // medida de segurança
    {
        Lista_DestroiLista(cam->caminho, ListaCaminho_LiberaInt); // destroi a lista do caminho
        free(cam);                                                // libera o struct
    }
}

// Função auxiliar de criação dos caminhos da lista:
static void CriaCaminhos(Lista *lista, Arvore *huffman, Arvore *raiz)
{
    if (Arvore_EhFolha(raiz)) // se for uma folha
    {
        // Cria um struct caminho com o carácter e o caminho até ele
        Caminho *cam = (Caminho *)malloc(sizeof(Caminho));
        cam->caracter = Arvore_Caracter(raiz);
        cam->caminho = Arvore_Caminho(huffman, raiz);
        // Adiciona o struct criado na lista
        Lista_ListaAdd(lista, Lista_NovoItem("Caminho", cam), 0);
    }
    else // se não for uma folha aplica a função recursivamente a suas árvores
    {
        CriaCaminhos(lista, huffman, Arvore_ArvoreDireita(raiz));  // aplicando recursão a direita
        CriaCaminhos(lista, huffman, Arvore_ArvoreEsquerda(raiz)); // aplicando recursão a esquerda
    }
}

// Cria a lista de caminhos da árvore de Huffman
Lista *ListaCaminho_CriaLista(Arvore *huffman)
{
    Lista *novaLista = Lista_NovaLista("Caminho"); // cria a lista

    CriaCaminhos(novaLista, huffman, huffman); // insere cada carácter da árvore na lista com seus respectivos caminhos

    return novaLista;
}

// Retorna o caminho de um carácter
Lista *ListaCaminho_Caminho(Lista *listaCam, unsigned char caracter)
{
    int i;                                             // variável de incrementação
    Caminho *p;                                        // variável auxiliar de busca
    for (i = 0; i < Lista_TamanhoLista(listaCam); i++) // varre toda a lista
    {
        p = (Caminho *)Lista_AchaItem(listaCam, i); // auxiliar recebe o struct caminho de um carácter
        if (p->caracter == caracter)                // se for o carácter buscado interrompe a busca
        {
            break;
        }
    }
    return p->caminho; // retorna o caminho
}

// Destroi a lista de caminhos
Lista *ListaCaminho_DestroiLista(Lista *lista)
{
    if (lista != NULL) // medida de segurança
    {
        Lista_DestroiLista(lista, DestroiCaminho); // destroi a lista de structs
    }
    return NULL;
}

// Criando uma lista de inteiros:
Lista *ListaCaminho_CriaListaInt(void)
{
    return Lista_NovaLista("int*");
}

// Inicializando um inteiro para ser adicionado a lista de inteiros:
int *ListaCaminho_CriaInt(int valor)
{
    int *inteiro = (int *)malloc(sizeof(int));
    *inteiro = valor;

    return inteiro;
}

// Liberando um inteiro da lista de inteiros:
void ListaCaminho_LiberaInt(void *conteudo)
{
    free(conteudo);
}