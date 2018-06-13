/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

compactador.h: definições para compactador
********************************************/

#ifndef COMPACTADOR_H_

#define COMPACTADOR_H_

#include "arvore.h"

/**
 * Função de criação da árvore de Huffman:
 * Input: nome do arquivo;
 * Output: ponteiro para a árvore;
 * Condições: arquivo existe;
 * Efeitos Colaterais: nenhum;
*/
Arvore* Compactador_MontaArvoreHuffman(char* arquivo);

/**
 * Função de impressão do arquivo compactado:
 * Input: arvore de Huffman para o arquivo e nome do arquivo a ser compactado (com extensão .txt);
 * Output: arquivo compactado;
 * Condições: arvore válida e arquivo existe;
 * Efeitos Colaterais: nenhum;
*/
void Compactador_Compacta(Arvore* arvoreHuffman, char* entrada, char* saida);

/**
 * Função de descompactação de um arquivo:
 * Input: nome do arquivo (com extensão .comp);
 * Output: arquivo descompactado;
 * Condições: arquivo existe;
 * Efeitos Colaterais: nenhum;
*/
void Compactador_Descompacta(char* arquivo, char* saida);

#endif
