/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

arvore.h: definições para arvore genérica
********************************************/

#ifndef ARVORE_H_

#define ARVORE_H_

/**
 * Tipo Árvore:
 * Será usada para a codificação dos caracteres em um texto;
 * Possui os seguintes campos:
 * Arvore* esq: ponteiro para a subárvore da esquerda;
 * Arvore* dir: ponteiro para a subárvore da direita;
 * char caracter: caracter analisado;
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
Arvore* CriaFolha(char caracter, int ocorrencias);

/**
 * Função que cria uma árvore:
 * Inputs: caracter, número de ocorrências do caracter, ponteiro para a subárvore da esquerda e para a da direita;
 * Output: ponteiro para uma árvore com nós filhos;
 * Condições: subárvores existentes e alocadas;
 * Efeitos Colaterais: nenhum;
*/
Arvore* CriaArvore(char caracter, int ocorrencias, Arvore* esq, Arvore* dir);

/**
 * Função que verifica se a árvore é uma folha ou não:
 * Input: ponteiro para a árvore;
 * Output: inteiro com comportamento booleano que diz se a árvore é ou não uma folha;
 * Condições: árvore existente e alocada;
 * Efeitos Colaterais: nenhum;
*/
int EhFolha(Arvore* arvore);

/**
 * Função que calcula o número de nós folhas de uma árvore:
 * Input: ponteiro para a árvore;
 * Output: número de nós folha;
 * Condições: árvore existente e alocada;
 * Efeitos Colaterais: nenhum;
*/
int NumFolhas(Arvore* arvore);

/**
 * Função que verifica se um nó pertence a uma dada árvore:
 * Inputs: ponteiro para raiz da árvore e para o nó alvo;
 * Output: inteiro booleano que diz se o nó pertence ou não à árvore;
 * Condições: variáveis existentes e alocadas;
 * Efeitos Colaterais: nenhum;
*/
int Pertence(Arvore* raiz, Arvore* no);

/**
 * Função que retorna o caminho da raiz até um dado nó:
 * Inputs: nó raiz e nó alvo;
 * Output: string com os dados de cada nó no caminho
 * Condições: ambas as árvores existentes e alocadas;
 * Efeitos Colaterais: nenhum; 
*/
char* Caminho(Arvore* raiz, Arvore* no);

/**
 * Função que destrói uma árvore da memória:
 * Input: ponteiro para o nó raiz da árvore;
 * Output: ponteiro vazio;
 * Condições: árvore existente e alocada;
 * Efeitos Colaterais: a árvore será apagada da memória;
*/
Arvore* DestroiArvore(Arvore* raiz);

#endif
