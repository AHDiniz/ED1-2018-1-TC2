/********************************************
Trabalho 2 ED1 2018/1: Compactador de arquivos

Alan Herculano Diniz e Rafael Belmock Pedruzzi

compactador.c: implementações para compactador
********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lista.h"
#include "bitmap.h"
#include "compactador.h"
#include "listacaminho.h"

#define ASCII_TAM 256

/**
 * Função auxiliar que inicializa n posições de um bitmap:
 * Input: bitmap e número de posições;
 * Output: nunhum;
 * Condições: bitmap existe e tem tamanho máximo >= n;
 * Efeitos Colaterais: todas as n primeiras posicões do bitmap ficam iguais a 0;
*/
static void InicializaBitmap(bitmap* map, unsigned int n)
{
    int i; // Variavel de incrementação
    for(i = 0 ; i < n ; i++) // inicializando conteudo do bitmap como 0
    {
        bitmapAppendLeastSignificantBit(map,0);
    }
}

/**
 * Função auxiliar que converte um caracter sem sinal para binário:
 * Input: bitmap que armazenará o binário e um caracter sem sinal;
 * Output: bitmap contendo o caracter convertido em seus 8 primeiros bits (posição 0 mais significativa);
 * Condições: bitmap existe e tem tamanho máximo >= 8;
 * Efeitos Colaterais: bitmap contem o binário de 8 bits;
*/
static void ConverteParaBinario(bitmap* map, unsigned char a)
{
    int i; // Variavel de incrementação
    unsigned int num; // armazena o valor do caracter em inteiro decimal

    // Realizando divisões por 2 e armazenado os restos no bitmap
    for(i = 7, num = (unsigned int) a ; i >= 0 ; num /= 2, i--)
    {
        bitmapSetBit(map,i,num % 2);
    }
}

/**
 * Função auxiliar que converte um binário para caracter sem sinal:
 * Input: bitmap contendo um binário de 8 bits;
 * Output: o caracter convertido dos 8 primeiros bits do bitmap;
 * Condições: bitmap existente e válido;
 * Efeitos Colaterais: nenhum;
*/
static unsigned char ConverteParaCharacter(bitmap* map)
{
    int i; // Variavel de incrementação
    unsigned int num; // armazena o valor do caracter em inteiro decimal

    // Percorrendo os 8 primeiros bits do bitmap
    for(i = 0, num = 0 ; i < 8 ; i++)
    {
        if(bitmapGetBit(*map, i) == 1) // se o bit for 1
        {
            num += pow(2.0, 7 - i); // soma à conversão a respectiva potência de 2
        }
    }

    return (unsigned char) num; // retorna caracter sem sinal
}

/**
 * Função auxiliar que codifica uma árvore para ser impressa e a armazena numa lista de inteiros:
 * Input: lista de inteiros e árvore;
 * Output: nenhum;
 * Condições: lista tipo int e árvore existente;
 * Efeitos Colaterais: a lista recebe a árvore codificada no final;
*/
static void CompactaArvore(Lista* lista, Arvore* arvore)
{
    if(Arvore_EhFolha(arvore)) // se a árvore for uma folha
    {
       int i; // Variavel de incrementação
       bitmap map = bitmapInit(8); // bitmap para auxiliar a conversão do carácter para binário
       InicializaBitmap(&map,8); // inicializa o bitmap
       ConverteParaBinario(&map,Arvore_Caracter(arvore)); // converte o carácter para binário
       Lista_ListaAdd(lista,Lista_NovoItem("int",0),0); // insere 0 (codigo para folha) na lista
       for(i = 0 ; i < 8 ; i++) // insere em seguida o caracter em binário
       {
           Lista_ListaAdd(lista,Lista_NovoItem("int",bitmapGetBit(map,i)),Lista_TamanhoLista(lista) -1);
       }
    }
    else // se a árvore for um nó
    {
        Lista_ListaAdd(lista,Lista_NovoItem("int",1),Lista_TamanhoLista(lista) -1); // insere 1 (codigo para nó) na lista
        CompactaArvore(lista,Arvore_ArvoreEsquerda(arvore)); // compacta a árvore da esquerda
        CompactaArvore(lista,Arvore_ArvoreDireita(arvore)); // compacta a árvore da direita
    }
}

/**
 * Função auxiliar que imprime os 8 primeiros bits de uma lista como carácter codificado no arquivo especificado:
 * Input: bitmap, lista e arquivo;
 * Output: nenhum;
 * Condições: bitmap com 8 bits significativos, lista tipo int com os bits e arquivo existente;
 * Efeitos Colaterais: os 8 primeiros bits são codificados e impressos no arquivo;
*/
static void ImprimeCaracter(bitmap* bitmap, Lista* lista, FILE* output)
{
    int i; // Variavel de incrementação
    for(i = 0 ; i < 8 ; i++)
    {
        bitmapSetBit(bitmap,i,*((int*) Lista_AchaItem(lista,0)));
        Lista_ListaRemove(lista,0,ListaCaminho_LiberaInt);
    }

    fputc(ConverteParaCharacter(bitmap),output);
}

/**
 * Função auxiliar que identifica os pesos de cada caracter do arquivo:
 * Input: nome do arquivo;
 * Output: vetor com os pesos de cada caracter (identificados pela sua posição no vetor);
 * Condições: arquivo existe;
 * Efeitos Colaterais: nenhum;
*/
static int* MontaVetorPesos(char* arquivo)
{
    int i; // Variavel de incrementação
    int pesos[ASCII_TAM]; // vetor que guarda o peso do caracter na masma posição de sua posição da tabela ASCII
    unsigned char c; // auxiliar que guarda o caracter

    // Inicializando ocorrencias
    for(i = 0 ; i < ASCII_TAM ; i++)
    {
        pesos[i] = 0;
    }

    FILE *input = fopen(arquivo, "r"); // abrindo arquivo

    c = fgetc(input); // selecionando primeiro caracter
    while(c != EOF) // percorrendo todo arquivo
    {
        pesos[(int) c]++; // contando as ocorrencias de cada caracter
        c = fgetc(input); // atualizando caracter
    }

    fclose(input); // fechando arquivo
    return pesos;
}

/**
 * Função auxiliar que insere ordenadamente uma árvore numa lista:
 * Input: lista e árvore;
 * Output: nenhum;
 * Condições: lista e árvore existem;
 * Efeitos Colaterais: árvore inclusa à lista mantendo-a ordenada;
*/
static void InsereArvoreOrdenado(Lista* l, Arvore* arvore)
{
    int i; // Variavel de incrementação
    for(i = 0 ; i < Lista_TamanhoLista(l) ; i++) // Varrendo toda a lista
    {
        // Interrompendo o loop ao encontrar uma árvore com campo 'ocorrencias' maior do que o da árvore a ser inserida
        if(Arvore_Ocorrencias((Arvore*) Lista_AchaItem(l,i)) > Arvore_Ocorrencias(arvore))
        {
            break;
        }
    } // Ao fim do loop, 'i' será a posição onde deve ser inserida a árvore

    Lista_ListaAdd(l, Lista_NovoItem("Arvore", arvore), i); // Inserindo árvore em sua devida posição
}

/**
 * Função auxiliar que monta uma lista com os caminhos para cada:
 * Input: lista e árvore;
 * Output: nenhum;
 * Condições: lista e árvore existem;
 * Efeitos Colaterais: árvore inclusa à lista mantendo-a ordenada;
*/

// Lendo arquivo e montando a árvore de Huffman:
Arvore* Compactador_MontaArvoreHuffman(char* arquivo)
{
    int i; // Variavel de incrementação
    // Variáveis auxiliares para criação da árvore
    Arvore *t1;
    Arvore *t2;
    Arvore *tr;

    int *pesos = MontaVetorPesos(arquivo); // Encontrando o numero de ocorrencias de cada caracter no arquivo

    Lista *listaArvores = Lista_NovaLista("Arvore"); // Inicializando a lista de árvores

    for(i = 0; i < ASCII_TAM; i++) // Varrendo todo vetor de caracteres
    {
        if(pesos[i] > 0) // Encontrando os caracteres presentes no arquivo
        {
            InsereArvoreOrdenado(listaArvores, Arvore_CriaFolha(i, pesos[i])); // inserindo os caracteres na lista, ordenados segundo o peso
        }
    }

    // Criando a árvore de Huffman
    while(Lista_TamanhoLista(listaArvores) > 1) // inicio do loop
    {
        t1 = (Arvore*) Lista_AchaItem(listaArvores, 0); // t1 recebe a primeira árvore da lista
        t2 = (Arvore*) Lista_AchaItem(listaArvores, 1); // t2 recebe a segunda árvore da lista

        // criando nova árvore cujo peso é a soma dos pesos de t1 e t2 e suas árvores esquerda e direita são t1 e t2, respectivamente
        tr = Arvore_CriaArvore((int)Arvore_Ocorrencias(t1) + Arvore_Ocorrencias(t2), t1, t2);

        Lista_ListaRemove(listaArvores, 0, NULL); // removendo t1 da lista
        Lista_ListaRemove(listaArvores, 1, NULL); // removendo t2 da lista

        InsereArvoreOrdenado(listaArvores, tr); // inserindo tr na lista ordenadamente
    } // loop encerra quando só houver uma árvore na lista. Essa é a árvore de Huffman

    tr = (Arvore*) Lista_AchaItem(listaArvores, 0); // tr recebe a árvore de Huffman
    Lista_DestroiLista(listaArvores, NULL); // liberando memória da lista

    return tr;
}

// Compactando e imprimindo o arquivo
void Compactador_Compacta(Arvore* arvoreHuffman, char* entrada, char* saida)
{
    int i; // Variavel de incrementação
    FILE* output = fopen(saida, "w"); // abrindo arquivo de escrita
    FILE* input = fopen(entrada, "r"); // abrindo arquivo de leitura
    unsigned char c; // variável auxiliar para leitura do arquivo
    Lista* caminho = ListaCaminho_CriaLista(arvoreHuffman); // lista com os novos códigos para cada carácter
    Lista* bits = Lista_NovaLista("int*"); // lista auxiliar que guarda a sequência de bits a serem impressos
    Lista* l;
    bitmap caracter = bitmapInit(8); // bitmap auxiliar que guarda o carácter a ser impresso

    InicializaBitmap(&caracter, 8); // inicializando todos os bits do bitmap como 0

    // Imprimindo primeiro a árvore compactada
    CompactaArvore(bits,arvoreHuffman); // compacta a árvore em bits
    // Imprime os bits da lista, de 8 em 8, como carácteres
    while(Lista_TamanhoLista(bits) >= 8)
    {
        ImprimeCaracter(&caracter,bits,output);
    }

    // Imprimindo em seguida a conversão do arquivo de entrada
    c = fgetc(input); // inicializando com o primeiro carácter
    while(c != EOF) // varrendo o arquivo de entrada
    {
        l = ListaCaminho_Caminho(caminho,c); // buscando o caminho para o carácter lido
        // Inserindo o caminho na lista bits
        for(i = 0 ; i < Lista_TamanhoLista(l) ; i++)
        {
            Lista_ListaAdd(bits,Lista_NovoItem("int*",Lista_AchaItem(l,i)),Lista_TamanhoLista(bits));
        }

        // Imprimindo os bits caso completem um carácter
        while(Lista_TamanhoLista(bits) >= 8)
        {
            ImprimeCaracter(&caracter,bits,output);
        }

        c = fgetc(input); // atualizando carácter
    }

    if(!Lista_ListaVazia(bits)) // se sobrarem bits para serem impressos
    {
        // Cria um auxiliar para completar o espaço faltando
        int* aux = ListaCaminho_CriaInt(0);

        // Completa a lista até 8 bits
        while(Lista_TamanhoLista(bits) < 8)
        {
            Lista_ListaAdd(bits,Lista_NovoItem("int*",aux),Lista_TamanhoLista(bits));
        }
        
        // Imprime o último carácter
        ImprimeCaracter(&caracter,bits,output);
    }

    // Fechando os arquivos
    fclose(input); // fechando arquivo de leitura
    fclose(output); // fechando arquivo de escrita
    // Destruindo as listas utilizadas
    ListaCaminho_DestroiLista(caminho); // destruindo a lista de inteiros
    Lista_DestroiLista(bits,ListaCaminho_LiberaInt); // destruindo a lista de impressão
}

/**
 * Função de descompactação de um arquivo:
 * Input: nome do arquivo (com extensão .comp);
 * Output: arquivo descompactado;
 * Condições: arquivo existe;
 * Efeitos Colaterais: nenhum;
*/
void Compactador_Descompacta(char* arquivo);