/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

arvore.h: definições para arvore genérica
********************************************/

#ifndef ARVORE_H_

#define ARVORE_H_

#include "bitmap.h"
#include "lista.h"

/**
 * Tipo Árvore:
 * Será usada para a codificação dos caracteres em um texto;
 * Possui os seguintes campos:
 * Arvore* esq: ponteiro para a subárvore da esquerda;
 * Arvore* dir: ponteiro para a subárvore da direita;
 * unsigned char caracter: caracter analisado;
 * int ocorrencias: número de ocorrências do caracter;
*/
typedef struct arvore Arvore;

/**
 * Função de criação de árvore sem nós filhos:
 * Input: caracter e número de ocorrências do caracter;
 * Output: ponteiro para uma folha (árvore sem nós filhos);
 * Condições: nenhuma;
 * Efeitos Colaterais: nenhum;
*/
Arvore* Arvore_CriaFolha(unsigned char caracter, int ocorrencias);

/**
 * Função que cria uma árvore:
 * Inputs: caracter, número de ocorrências do caracter, ponteiro para a subárvore da esquerda e para a da direita;
 * Output: ponteiro para uma árvore com nós filhos;
 * Condições: subárvores existentes e alocadas;
 * Efeitos Colaterais: nenhum;
*/
Arvore* Arvore_CriaArvore(int ocorrencias, Arvore* esq, Arvore* dir);

/**
 * Função que retorna o caracter de uma árvore:
 * Input: ponteiro para a árvore;
 * Output: caracter;
 * Condições: árvore existente e alocada;
 * Efeitos Colaterais: nenhum;
*/
unsigned char Arvore_Caracter(Arvore* arvore);

/**
 * Função que retorna o valor ocorrências de uma árvore:
 * Input: ponteiro para a árvore;
 * Output: inteiro;
 * Condições: árvore existente e alocada;
 * Efeitos Colaterais: nenhum;
*/
int Arvore_Ocorrencias(Arvore* arvore);

/**
 * Função que retorna a árvore da direita de uma árvore:
 * Input: ponteiro para a árvore;
 * Output: árvore da direita;
 * Condições: árvore existente e alocada;
 * Efeitos Colaterais: nenhum;
*/
Arvore* Arvore_ArvoreDireita(Arvore* arvore);

/**
 * Função que retorna a árvore da esquerda de uma árvore:
 * Input: ponteiro para a árvore;
 * Output: árvore da esquerda;
 * Condições: árvore existente e alocada;
 * Efeitos Colaterais: nenhum;
*/
Arvore* Arvore_ArvoreEsquerda(Arvore* arvore);

/**
 * Função que verifica se a árvore é uma folha ou não:
 * Input: ponteiro para a árvore;
 * Output: inteiro com comportamento booleano que diz se a árvore é ou não uma folha;
 * Condições: árvore existente e alocada;
 * Efeitos Colaterais: nenhum;
*/
int Arvore_EhFolha(Arvore* arvore);

/**
 * Função que calcula o número de nós folhas de uma árvore:
 * Input: ponteiro para a árvore;
 * Output: número de nós folha;
 * Condições: árvore existente e alocada;
 * Efeitos Colaterais: nenhum;
*/
int Arvore_NumFolhas(Arvore* arvore);

/**
 * Função que verifica se um caracter pertence a algum nó de uma dada árvore:
 * Inputs: ponteiro para raiz da árvore e caracter (sem sinal);
 * Output: inteiro booleano que diz se o caracter pertence ou não à árvore;
 * Condições: variáveis existentes;
 * Efeitos Colaterais: nenhum;
*/
int Arvore_Pertence(Arvore* raiz, unsigned char* c);

/**
 * Função que retorna, em um bitmap, o caminho da raiz até um dado nó:
 * Inputs: bitmap, nó raiz, ponteiro para a primeira posição onde ficará a sequncia no bitmap e carácter do nó alvo (deve ser um carácter que esteja em uma única posição na árvore);
 * Output: nenhum;
 * Condições: ambas as árvores e bitmap existentes e alocadas;
 * Efeitos Colaterais: bitmap contem o caminho, pos é incrementado; 
*/
Lista* Arvore_Caminho(Arvore *raiz, Arvore *alvo);

/**
 * Função que destrói uma árvore da memória:
 * Input: ponteiro para o nó raiz da árvore;
 * Output: ponteiro vazio;
 * Condições: árvore existente e alocada;
 * Efeitos Colaterais: a árvore será apagada da memória;
*/
Arvore* Arvore_DestroiArvore(Arvore* raiz);










// Funções e estruturas auxiliares utilizadas na procura do caminho da raiz de uma árvore até um nó dessa árvore:

/**
 * Tipo Caminho:
 * Será usada para guardar o caminho para cada carácter numa árvore;
 * Possui os seguintes campos:
 * unsigned char caracter;
 * Lista* caminho;
*/
typedef struct caminho Caminho;

/**
 * Função de criação de uma lista de inteiros:
 * Input: nenhum;
 * Output: ponteiro para lista de inteiros criada;
 * Condições: nenhuma;
 * Efeitos Colaterais: nenhum;
*/
Lista *ListaCaminho_CriaListaInt(void);

/**
 * Função de inicialização de um inteiro para ser adicionado a lista de inteiros:
 * Input: int;
 * Output: ponteiro para o inteiro;
 * Condições: nenhuma;
 * Efeitos Colaterais: nenhum;
*/
int *ListaCaminho_CriaInt(int valor);

/**
 * Função de criação da lista de caminhos:
 * Input: ponteiro para a árvore de Huffman;
 * Output: lista contendo todos os carácteres e seus respectivos caminhos;
 * Condições: árvore existe;
 * Efeitos Colaterais: nenhum;
*/
Lista *ListaCaminho_CriaLista(Arvore *huffman);

/**
 * Função que retorna o caminho para determinado carácter:
 * Input: ponteiro para a lista de caminhos e carácter;
 * Output: caminho para o carácter;
 * Condições: lista existente e alocada e carácter pertencente a lista;
 * Efeitos Colaterais: nenhum;
*/
Lista *ListaCaminho_Caminho(Lista *listaCam, unsigned char caracter);

/**
 * Função que destrói uma lista de caminhos da memória:
 * Input: ponteiro para a lista de caminhos;
 * Output: ponteiro vazio;
 * Condições: lista existente e alocada;
 * Efeitos Colaterais: a lista será apagada da memória;
*/
Lista *ListaCaminho_DestroiLista(Lista *lista);

/**
 * Função para liberar um inteiro da lista de inteiros:
 * Input: ponteiro genérico para o conteudo;
 * Output: nenhum;
 * Condições: conteudo existente e alocado;
 * Efeitos Colaterais: o conteudo será apagado da memória;
*/
void ListaCaminho_LiberaInt(void *conteudo);

#endif
