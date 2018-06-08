/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

listacaminho.h: definições para lista dos caminhos
********************************************/

#ifndef LISTACAMINHO_H_

#define LISTACAMINHO_H_

#include "lista.h"
#include "arvore.h"

/**
 * Tipo Caminho:
 * Será usada para quardar o caminho para cada carácter numa árvore;
 * Possui os seguintes campos:
 * unsigned char caracter;
 * Lista* caminho;
*/
typedef struct caminho Caminho;

/**
 * Função de criação da lista de caminhos:
 * Input: ponteiro para a árvore de Huffman;
 * Output: lista contendo todos os carácteres e seus respectivos caminhos;
 * Condições: árvore existe;
 * Efeitos Colaterais: nenhum;
*/
Lista* ListaCaminho_CriaLista(Arvore* huffman);

/**
 * Função que retorna o caminho para determinado carácter:
 * Input: ponteiro para a lista de caminhos e carácter;
 * Output: caminho para o carácter;
 * Condições: lista existente e alocada e carácter pertencente a lista;
 * Efeitos Colaterais: nenhum;
*/
Lista* ListaCaminho_Caminho(Lista* listaCam, unsigned char caracter);

/**
 * Função que destrói uma lista de caminhos da memória:
 * Input: ponteiro para a lista de caminhos;
 * Output: ponteiro vazio;
 * Condições: lista existente e alocada;
 * Efeitos Colaterais: a lista será apagada da memória;
*/
Lista* ListaCaminho_DestroiLista(Lista* lista);

#endif
