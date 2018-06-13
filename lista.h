/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

lista.h: definições para lista genérica
********************************************/

#ifndef LISTA_H_

#define LISTA_H_

// Definindo um novo tipo de lista:
typedef struct lista Lista;

// Definindo um tipo para o item da lista:
typedef struct item Item;

// Definindo o padrão de função que libera o conteúdo de um item:
typedef void FreeContItem(void*);

// Definindo padrão de função para impressão de conteúdo de item:
typedef void Imprime(void*);

/*
Nova Lista:
- Entrada: a string que indica o tipo de item que a lista comporta;
- Saída: um ponteiro para uma nova lista vazia;
- Pré-condições: o tipo deve ser algum existente no programa e preferencialmente escrito da mesma forma em que o tipo foi definido;
- Pós-condições: a lista está vazia;
*/
Lista* Lista_NovaLista(const char* tipo);

/*
Destruição de Lista:
- Entrada: um ponteiro para a lista que será destruída e outro para a função de liberação de conteúdo de item;
- Saída: nenhuma;
- Pré-condições: a lista deve estar alocada na memória;
- Pós-condições: os itens da lista que ainda estão nela serão destruídos também;
*/
void Lista_DestroiLista(Lista* lista, FreeContItem Func);

/*
Verificação de Lista Vazia:
- Entrada: a lista a ser verificada;
- Saída: um inteiro de comportamento booleano que diz se a lista está vazia ou não;
- Pré-condições: a lista precisa estar alocada na memória;
- Pós-condições: nenhum efeito colateral;
*/
int Lista_ListaVazia(Lista* lista);

/*
Tamanho da lista:
- Entrada: ponteiro para a lista a ser analisada;
- Sáida: tamanho da lista analisada;
- Pré-condições: lista alocada e não vazia;
- Pós-condições: nenhum efeito colateral;
*/
unsigned int Lista_TamanhoLista(Lista* lista);

/*
Adicionar item na lista:
- Entrada: a lista e o item a ser adicionado;
- Saída: nenhuma;
- Pré-condições: a lista e o item não podem estar vazios;
- Pós-condições: o item é adicionado na lista e ela fica maior;
*/
void Lista_ListaAdd(Lista* lista, Item* item, unsigned int pos);

/*
Remover item na lista:
- Entrada: a lista e a posição do item a ser removido;
- Saída: nenhuma;
- Pré-condições: a lista não pode estar vazia e a posição deve ficar entre zero e o tamanho atual da lista;
- Pós-condições: o item é liberado da memória e a lista fica menor;
*/
void Lista_ListaRemove(Lista* lista, const unsigned int pos, FreeContItem Func);

/*
Achar item na lista:
- Entrada: ponteiro para a lista e posição do item desejado;
- Saída: ponteiro para o item procurado;
- Pré-condições: lista alocada e não vazia e posição válida;
- Pós-condições: nenhum efeito colateral;
*/
void* Lista_AchaItem(Lista* lista, const unsigned int pos);

/*
Criação de um novo item:
- Entrada: tag com o tipo do item e o ponteiro opaco para o conteúdo;
- Saída: um ponteiro para um novo item;
- Pré-condições: tanto a tag quanto o conteúdo não podem apontar para NULL;
- Pós-condições: nenhum efeito colateral;
*/
Item* Lista_NovoItem(const char* tipo, void* conteudo);

/*
Verificação de item com conteúdo opaco:
- Entrada: o item a ser verificado;
- Saída: um inteiro com comportamento booleano que diz se o item tem ou não um conteúdo opaco;
- Pré-condições: o item já deve ter sido alocado;
- Pós-condições: nenhum efeito colateral;
*/
int Lista_ItemVazio(Item* item);

/*
Liberação de item da memória:
- Entrada: o item a ser liberado da memória e o ponteiro para a função que libera o conteúdo;
- Sáida: um inteiro com comportamento booleano que diz se operação ocorreu ou não da forma devida;
- Pré-condições: o item deve estar alocado na memória e a função deve existir e seguir o padrão determinado para a função que libera o conteúdo de um item;
- Pós-condições: o item está vazio;
*/
int Lista_LiberaItem(Item* item, FreeContItem Func);

/*
Conteúdo de um item:
- Entrada: ponteiro para o item;
- Saída: ponteiro opaco para conteúdo:
- Pré-condições: o item deve estar alocado e com conteúdo;
- Pós-condições: nenhum efeito colateral;
*/
void* Lista_ConteudoItem(Item* item);

#endif
