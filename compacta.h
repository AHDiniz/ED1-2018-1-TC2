/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

compacta.h: definições para compactador
********************************************/

#ifndef COMPACTA_H_

#define COMPACTA_H_

#include "arvore.h"

/**
 * Função de criação da árvore de Huffman:
 * Input: nome do arquivo;
 * Output: ponteiro para a árvore;
 * Condições: arquivo existe;
 * Efeitos Colaterais: nenhum;
*/
Arvore* Compacta_MontaArvoreHuffman(char* arquivo);

/**
 * Função de impressão do arquivo compactado:
 * Input: arvore de Huffman para o arquivo e nome do arquivo a ser compactado (com extensão .txt);
 * Output: arquivo compactado;
 * Condições: arvore válida e arquivo existe;
 * Efeitos Colaterais: nenhum;
*/
void Compacta_ImprimeArquivo(Arvore* arvorHuffman, char* arquivo);

#endif
