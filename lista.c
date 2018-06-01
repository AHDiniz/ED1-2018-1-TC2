/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

lista.c: implementações para lista genérica
********************************************/

#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implementando uma lista genérica:
struct lista
{
    Item* primeiro;           // Ponteiro para o primeiro item da lista
    Item* ultimo;             // Ponteiro para o último item da lista
    char* tipo;               // Tag com o tipo de item que a lista contém
    unsigned int comprimento; // Comprimento da lista (quantidade de itens)
};

// Implementando um item de uma lista genérica:
struct item
{
    void *conteudo; // Ponteiro para o conteúdo do item
    char *tipo;     // Tag com o tipo do conteúdo do item
    Item *proximo;  // Ponteiro para o próximo item da lista
    int posicao;    // Posição do item na lista
};

// Funções estáticas:

// Função de comparação de tipos entre lista e item:
static int CmpTipoListaItem(Lista* lista, Item* item)
{
    if (strcmp(lista->tipo, item->tipo) == 0)
        return 1;
    else
        return 0;
}

// Alocando uma nova lista dinamicamente:
Lista* Lista_NovaLista(const char* tipo)
{
    Lista* lista = (Lista*)malloc(sizeof(Lista)); // Alocando dinamicamente o espaço de memória necessário para a nova lista
    
    lista->tipo = (char*)malloc(strlen(tipo) + 1); // Alocando dinamicamente o espaço necessário para a tag de tipo da lista
    strcpy(lista->tipo, tipo); // Copiando o conteúdo da string de entrada para o campo do struct

    lista->primeiro = lista->ultimo = NULL; // A lista está vazia (não há nada nem na primeira nem na última posição)

    lista->comprimento = 0; // A lista tem tamanho zero

    return lista;
}

// Destruindo a lista na memória:
void Lista_DestroiLista(Lista* lista, FreeContItem Func)
{
    int i, tamanho;
    // Verificando se a lista não está vazia para destruir cada item ainda existente:
    if (!Lista_ListaVazia(lista))
    {
        for (i = 0, tamanho = lista->comprimento; i < tamanho; i++) // Varrendo a lista para destruir os itens
            Lista_ListaRemove(lista, 0, Func); // Removendo cada item da lista até ela ficar vazia
    }
    free(lista->tipo); // Liberando a tag de tipo da lista
    free(lista); // Liberando a lista em si
}

// Verificação de lista vazia:
int Lista_ListaVazia(Lista* lista)
{
    return (lista->comprimento == 0); // Se o comprimento da lista é igual a zero, então não há nenhum item nela
}

// Tamanho da lista:
int Lista_TamanhoLista(Lista* lista)
{
    return (lista->comprimento);
}

// Adicionando um item na lista:
void Lista_ListaAdd(Lista* lista, Item* item, unsigned int pos)
{
    if (!CmpTipoListaItem(lista, item)) // Comparando os tipos da lista e do item
    {
        printf("Tentando inserir um item de um tipo diferente da lista.\n");
        return; // Se forem diferentes, o item não será adicionado na lista
    }
    if (Lista_ListaVazia(lista)) // Se a lista for vazia
    {
        lista->primeiro = item; // O primeiro item da lista vai ser o item de entrada da função
        lista->ultimo = item; // O último item da lista tambem será o item de entrada
    }
    else
    {
        if(pos >= lista->comprimento) // Se a a posição for maior ou igual ao tamanho da lista
        {
            lista->ultimo->proximo = item; // O próximo item do atual último da lista é o item de entrada
            lista->ultimo = item; // O último item da lista será o item de entrada
        }
        else // Se a posição for no meio da lista
        {
            // Variável auxiliar que ajudará a percorrer a lista:
            Item* p = lista->primeiro;
            int i;
            for (i = 1; i < pos; i++) // Percorrendo cada item da lista
            {
                p = p->proximo; // atualizando auxiliar até que seja o item da posição pos-1
            }
            
            item->proximo = p->proximo;
            p->proximo = item;
            // Atualizando os valores das posições dos itens da lista:
            p = item->proximo; // Primeiro item que precisa ter sua posição atualizada
            for (i = pos+1; i < lista->comprimento - 1; i++)
            {
                p->posicao++;
                p = p->proximo;
            }
        }
    }
    item->posicao = pos; // Definindo a posicao do item de entrada na lista
    lista->comprimento++; // Atualizando o tamanho da lista
}

// Removendo um item da lista:
void Lista_ListaRemove(Lista* lista, const unsigned int pos, FreeContItem Func)
{
    int i;
    if (pos == 0) // Se o item a ser retirado é o primeiro
    {
        Item* alvo = lista->primeiro; // Auxiliar para não perder a referência do item a ser removido
        lista->primeiro = lista->primeiro->proximo; // Atualizando o valor do primeiro item da lista
        Item* item = lista->primeiro;
        // Atualizando o valor das posições dos itens da lista:
        for (i = 0; i < lista->comprimento - 1; i++)
        {
            item->posicao--;
            item = item->proximo;
        }
        Lista_LiberaItem(alvo, Func); // Liberando o item alvo da memória
    }
    else
    {
        if (pos == lista->comprimento - 1) // Se o item a ser retirado é o último
        {
            Item* anterior = lista->primeiro; // Auxiliar que apontará para o novo último item
            Item* alvo = lista->ultimo; // Auxiliar para não perder a referência do item a ser removido
            for (i = 0; i < lista->comprimento - 1; i++) // Percorrendo a lista para achar o penúltimo item
                anterior = anterior->proximo;
            // Atualizando o valor do último item da lista:
            lista->ultimo = anterior;
            anterior->proximo = NULL;
            // Liberando o item alvo da memória:
            Lista_LiberaItem(alvo, Func);
        }
        else
        {
            // Variáveis auxiliares que ajudarão a percorrer a lista:
            Item* anterior = lista->primeiro;
            Item* atual = lista->primeiro->proximo;
            for (i = 1; i < lista->comprimento -1; i++) // Percorrendo cada item da lista
                if (atual->posicao == pos) // Se o item atual estiver na posição desejada
                    break; // O loop será quebrado
                else
                {
                    // Atualizando os valores das variáveis auxiliares:
                    anterior = atual;
                    atual = atual->proximo;
                }
            anterior->proximo = atual->proximo; // Atualizando os valores dos ponteiros dos itens que continuam na lista
            // Atualizando os valores das posições dos itens da lista:
            anterior = anterior->proximo; // Primeiro item que precisa ter sua posição atualizada
            for (i += 1; i < lista->comprimento - 1; i++)
            {
                anterior->posicao--;
                anterior = anterior->proximo;
            }
            Lista_LiberaItem(atual, Func); // Liberando o item procurando da memória (vai acabar sendo retirado da lista)
        }
    }
    lista->comprimento--; // Atualizando o valor do comprimento da lista
}

// Achando um item na lista:
void* Lista_AchaItem(Lista* lista, const unsigned int pos)
{
    Item* ret = lista->primeiro; // Ponteiro que apontará para o item procurado
    // Procurando o item desejado:
    for (int i = 0; i < lista->comprimento; i++)
        if (ret->posicao == pos)
            break;
        else
            ret = ret->proximo;
    return ret->conteudo; // Retornando o conteudo do item procurado
}

// Alocando um novo item dinamicamente:
Item* Lista_NovoItem(const char* tipo, void* conteudo)
{
    Item *item = (Item *)malloc(sizeof(Item)); // Alocando o espaço necessário para um novo item dinamicamente

    item->conteudo = conteudo; // Fazendo o conteudo do item apontar para o conteudo desejado

    item->tipo = (char *)malloc(strlen(tipo) + 1); // Alocando o espaço necessário para a tag do tipo do item dinamicamente
    strcpy(item->tipo, tipo);                      // Definindo a tag do tipo do item

    item->proximo = NULL; // o proximo item só será deifnido quando o item estiver numa lista

    item->posicao = 0; // Inicializando a posição do item com um placeholder

    return item;
}

// Verificação de item com conteúdo opaco:
int Lista_ItemVazio(Item* item)
{
    return (item->conteudo == NULL); // Verificando se o conteudo apontado pelo respectivo ponteiro do item é opaco
}

// Liberação de item da memória:
int Lista_LiberaItem(Item* item, FreeContItem Func)
{
    if (Lista_ItemVazio(item)) // Checando se o item possui conteúdo
    {
        printf("O item nao possui conteudo.\n");
        return 0;
    }
    if(Func != NULL) // Checando se o ponteiro para a função existe
    {
        Func(item->conteudo); // Liberando o conteúdo do item
    }
    free(item->tipo); // Liberando a tag de tipo do item
    free(item); // Liberando o item
    return 1;
}

// Conteúdo de um item:
void* Lista_ConteudoItem(Item* item)
{
    return item->conteudo;
}