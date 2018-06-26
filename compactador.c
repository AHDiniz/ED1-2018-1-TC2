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
#include "compactador.h"
#include "bitmap.h"

#define ASCII_TAM 256
#define BM_MIN_TAM 500 // em bytes

/**
 * Função auxiliar que inicializa n posições em um bitmap:
 * Input: bitmap e número de posições;
 * Output: nunhum;
 * Condições: bitmap existe e tem tamanho máximo maior ou igual a seu tamanho atual +n;
 * Efeitos Colaterais: bitmap sofre incremento de n bits;
*/
static void InicializaBitmap(bitmap* map, unsigned int n)
{
    int i; // Variavel de incrementação
    // Inicializando conteudo no final do bitmap como 0 n vezes
    for(i = 0 ; i < n ; i++)
    {
        bitmapAppendLeastSignificantBit(map,0);
    }
}

/**
 * Função auxiliar que converte um caracter sem sinal para binário:
 * Input: bitmap que armazenará o binário e um caracter sem sinal;
 * Output: nenhum;
 * Condições: bitmap existe e tem tamanho máximo maior ou igual a seu tamanho atual +8;
 * Efeitos Colaterais: bitmap contem o binário de 8 bits no final;
*/
static void ConverteParaBinario(bitmap* map, unsigned char a)
{
    int i; // Variavel de incrementação
    int len; // tamanho do bitmap

    InicializaBitmap(map,8); // inicializando 8 bits no fim do bitmap
    len = bitmapGetLength(*map); // len recebe seu novo tamanho

    // Realizando divisões por 2 e armazenado os restos no bitmap (inseridos em ordem inversa)
    for(i = 0 ; i < 8 ; a /= 2, i++)
    {
        bitmapSetBit(map,len -i -1, a % 2);
    }
}

/**
 * Função auxiliar que converte um inteiro sem sinal menor que 8 para binário:
 * Input: bitmap que armazenará o binário e um inteiro sem sinal;
 * Output: nenhum;
 * Condições: vetor existe e seus 3 primeiros bit são iguais a 0;
 * Efeitos Colaterais: vetor contem o binário em seus 3 primeiros bits;
*/
static void ConverteIntParaBinario(bitmap* map, unsigned int a)
{
    int i; // variavel de incrementação

    // Realizando divisões por 2 e armazenado os restos no bitmap (inseridos em ordem inversa)
    for(i = 0 ; i < 3 ; a /= 2, i++)
    {
        bitmapSetBit(map,2 -i, a % 2);
    }
}

/**
 * Função auxiliar que converte um binário para caracter sem sinal:
 * Input: bitmap contendo um binário de 8 bits e posição de seu primeiro bit;
 * Output: o caracter sem sinal convertido;
 * Condições: bitmap existentente e válido;
 * Efeitos Colaterais: nenhum;
*/
static unsigned char ConverteParaCharacter(bitmap* map, int pos)
{
    int i; // Variavel de incrementação
    int num; // armazena o valor do caracter em inteiro decimal

    // Percorrendo os 8 bits do bitmap a partir de pos
    for(i = pos, num = 0 ; i < pos+8 ; i++)
    {
        if(bitmapGetBit(*map,i) == 1) // se o bit for 1
        {
            num += pow(2.0, 7 - (i-pos)); // soma à conversão a respectiva potência de 2
        }
    }

    return (unsigned char) num; // retorna o caracter sem sinal
}

/**
 * Função auxiliar que converte um binário de 3 bits para inteiro:
 * Input: bitmap contendo um binário de 3 bits;
 * Output: o inteiro convertido dos 3 primeiros bits do bitmap;
 * Condições: bitmap existente e válido;
 * Efeitos Colaterais: nenhum;
*/
static int ConverteParaInt(bitmap* map)
{
    int i; // Variavel de incrementação
    int num; // armazena o valor do binário em inteiro decimal

    // Percorrendo os 3 primeiros bits do bitmap
    for(i = 0, num = 0 ; i < 3 ; i++)
    {
        if(bitmapGetBit(*map,i) == 1) // se o bit for 1
        {
            num += pow(2.0, 2 - i); // soma à conversão a respectiva potência de 2
        }
    }

    return num; // retorna o inteiro
}

/**
 * Função auxiliar que codifica uma árvore para ser impressa e a armazena num bitmap:
 * Input: bitmap e árvore;
 * Output: nenhum;
 * Condições: bitmap e árvore existentes, bitmap com tamanho máximo suficiente;
 * Efeitos Colaterais: o bitmap recebe a árvore codificada no final;
*/
static void CompactaArvore(bitmap* map, Arvore* arvore)
{
    if(Arvore_EhFolha(arvore)) // se a árvore for uma folha
    {
        bitmapAppendLeastSignificantBit(map,0); // acressenta 0 ao bitmap (código para folha)
        ConverteParaBinario(map,Arvore_Caracter(arvore)); // converte o carácter para binário e também o acressenta
    }
    else // se a árvore for um nó
    {
        bitmapAppendLeastSignificantBit(map,1); // acressenta 1 ao bitmap (código para árvore)
        CompactaArvore(map,Arvore_ArvoreEsquerda(arvore)); // compacta a árvore da esquerda
        CompactaArvore(map,Arvore_ArvoreDireita(arvore)); // compacta a árvore da direita
    }
}

/**
 * Função auxiliar que reconstroi a árvore de Huffman compactada:
 * Input: bitmap e ponteiro para a posição de início da árvore no bitmap;
 * Output: árvore de Huffman;
 * Condições: bitmap contêm a árvore codificada;
 * Efeitos Colaterais: posição incrementada;
*/
static Arvore* DescompactaArvore(bitmap* map, int* pos)
{
    if( bitmapGetBit(*map,*pos)) // se o primeiro bit lido for 1
    {
        // Incrementa posição
        *pos += 1;
        // lê suas arvores esquerda e direita
        Arvore *esq = DescompactaArvore(map,pos);
        Arvore *dir = DescompactaArvore(map,pos);
        // Retorna uma nova árvore nó
        return Arvore_CriaArvore(0, esq, dir);
    }
    // Caso o bit for 0

    *pos += 1; // incrementa posição
    // Cria uma nova árvore folha com o carácter lido nos 8 bits seguintes da lista
    Arvore* folha = Arvore_CriaFolha(ConverteParaCharacter(map,*pos), 0);
    // pula esses bits no bitmap
    *pos += 8;

    return folha;
}

/**
 * Função auxiliar que calcula o tamanho de um arquivo de leitura:
 * Input: ponteiro para o arquivo;
 * Output: tamanho do arquivo em bytes;
 * Condições: arquivo existente e inicializado;
 * Efeitos Colaterais: ponteiro do arquivo retorna ao seu início;
*/static int TamanhoArquivo(FILE* f)
{
    fseek(f, 0L, SEEK_END); // ponteiro atualizado para o fim do arquivo
    int ret = ftell(f); // ret recebe numero de bytes do início ao ponteiro
    rewind(f); // ponteiro retorna ao início
    return ret; // retornando ret
}

/**
 * Função auxiliar que calcula o tamanho para o bitmap:
 * Input: tamanho do arquivo de leitura em bytes;
 * Output: tamanho do bitmap em bits;
 * Condições: tamanho maior que 0;
 * Efeitos Colaterais: nenhum;
*/
static int TamanhoBitmap(int tamArq)
{
    if(tamArq < BM_MIN_TAM) // se o tamanho for menor que o tamanho minimo escolhido
    {
        return BM_MIN_TAM*8; // retorna o tamanho minimo vezes 8
    }
    else // se não
    {
        return tamArq*8*2; // retorna o dobro do tamanho do arquivo de entrada vezes 8
    }
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
    int *pesos = (int*) malloc(sizeof(int) * ASCII_TAM); // vetor que guarda o peso do caracter na masma posição de sua posição da tabela ASCII
    unsigned char c; // auxiliar que guarda o caracter

    // Inicializando ocorrencias
    for(i = 0 ; i < ASCII_TAM ; i++)
    {
        pesos[i] = 0;
    }

    FILE *input = fopen(arquivo, "r"); // abrindo arquivo

    int arqTam = TamanhoArquivo(input); // armazena o tamanho do arquivo de entrada
    int posicao = 0; // indica a posição no arquivo

    while(posicao < arqTam) // percorrendo todo arquivo
    {
        c = fgetc(input); // inicializando/atualizando caracter
        pesos[c] += 1; // contando as ocorrencias de cada caracter
        posicao += 1; // atualizando posição
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
        tr = Arvore_CriaArvore(Arvore_Ocorrencias(t1) + Arvore_Ocorrencias(t2), t1, t2);

        Lista_ListaRemove(listaArvores, 0, NULL); // removendo t1 da lista
        Lista_ListaRemove(listaArvores, 0, NULL); // removendo t2 da lista

        InsereArvoreOrdenado(listaArvores, tr); // inserindo tr na lista ordenadamente
    } // loop encerra quando só houver uma árvore na lista. Essa é a árvore de Huffman

    tr = (Arvore*) Lista_AchaItem(listaArvores, 0); // tr recebe a árvore de Huffman
    Lista_DestroiLista(listaArvores, NULL); // liberando memória da lista
    free(pesos); // liberando pesos

    return tr;
}

// Compactando e imprimindo o arquivo
void Compactador_Compacta(Arvore* arvoreHuffman, char* entrada, char* saida)
{
    int i; // Variavel de incrementação
    FILE* output; // arquivo de escrita
    FILE* input = fopen(entrada, "r"); // abrindo arquivo de leitura
    int arqTam = TamanhoArquivo(input); // tamanho do arquivo de leitura
    int posicao = 0; // atual posição no arquivo
    unsigned char c; // variável auxiliar para leitura do arquivo
    Lista* caminho = ListaCaminho_CriaLista(arvoreHuffman); // lista com os códigos binários para cada carácter
    Lista* l; // lista auxiliar que guarda o caminho até certo carácter
    bitmap map = bitmapInit(TamanhoBitmap(arqTam)); // bitmap para impressão

    //- Preparando o cabeçalho -//

    // Reservando os 3 primeiros bits do bitmap para informar o número de bits sobrando no último carácter do arquivo compactado
    for(i = 0 ; i < 3 ; i++)
    {
        bitmapAppendLeastSignificantBit(&map,0);
    }

    // Inserindo a árvore compactada no bitmap
    CompactaArvore(&map,arvoreHuffman);

    //- Imprimindo em seguida a conversão do arquivo de entrada -//

    while(posicao < arqTam) // varrendo o arquivo de entrada
    {
        c = fgetc(input); // inicializando/atualizando carácter

        l = ListaCaminho_Caminho(caminho,c); // buscando o caminho para o carácter lido
        // Inserindo o caminho no bitmap
        for(i = 0 ; i < Lista_TamanhoLista(l) ; i++)
        {
            bitmapAppendLeastSignificantBit(&map, *((int*) Lista_AchaItem(l,i)) );
        }

        posicao += 1; // atualizando posição
    }
    fclose(input); // fechando arquivo de leitura

    i = 0; // inicializando i para armazenar o número de bits restantes no bitmap que não completarem um byte
    if(bitmapGetLength(map) % 8 != 0) // se faltarem bits para completar um carácter
    {
        i = 8 - bitmapGetLength(map) % 8; // i recebe o número de bits sobrando no último carácter
        ConverteIntParaBinario(&map,i); // os 3 bits reservados no inicio são substituidos pelo valor de i em binário
    }

    // Imprimindo:
    output = fopen(saida, "w"); // abrindo arquivo de escrita

    // Imprimindo todos os bits do bitmap como carácteres
    for(i = 0 ; i < bitmapGetLength(map) ; i++)
    {
        fputc(bitmapGetContents(map)[i] ,output);
    }

    fclose(output); // fechando arquivo de escrita

    // Destruindo as estruturas utilizadas
    ListaCaminho_DestroiLista(caminho); // destruindo a lista de inteiros
    free(bitmapGetContents(map)); // liberando o conteúdo do bitmap
}

// Descompactando e imprimindo um arquivo:
void Compactador_Descompacta(char* entrada, char* saida)
{
    int i; // variável de incrementação
    unsigned char c; // carácter auxiliar para leitura do arquivo de entrada
    FILE* input = fopen(entrada, "r"); // abrindo arquivo de leitura
    FILE* output; // arquivo de escrita
    int tamArq = TamanhoArquivo(input); // número de carácteres no arquivo de leitura
    Arvore *huffman; // árvore de compactação/descompactação armazenada no cabeçalho do arquivo compactado
    Arvore *atual; // árvore auxiliar de busca
    bitmap map = bitmapInit(TamanhoBitmap(tamArq)); // bitmap que armazenará o arquivo de leitura em formato binário
    int naoUtilizados; // armazena quantos bits não devem ser considerados no ultimo carácter do arquivo
    int bitPos; // indica a posição atual no bitmap

    // lendo todo o arquivo de entrada para o bitmap
    for(i = 0 ; i < tamArq ; i++)
    {
        c = fgetc(input); // retira um carácter do arquivo
        ConverteParaBinario(&map,c); // convertendo o carácter para binário e armazenando no bitmap
    }
    fclose(input); // fechando arquivo de leitura

    naoUtilizados = ConverteParaInt(&map); // convertendo seus 3 primeiros bits para inteiro
    bitPos = 3; // pulando esses 3 bits no bitmap

    // Descompactando a árvore de Huffman
    huffman = DescompactaArvore(&map, &bitPos);

    // Preparando para iniciar o loop
    output = fopen(saida, "w"); // abrindo arquivo de escrita
    atual = huffman; // inicializando auxiliar
    while(bitPos < bitmapGetLength(map) -naoUtilizados)// percorrendo bitmap, desconsiderando os bits não utilizados
    {   
        // Avaliando caminho para a folha
        if( bitmapGetBit(map,bitPos) ) // se o bit for 1
        {
            atual = Arvore_ArvoreDireita(atual); // segue para a árvore da direita
        }
        else // se o bit for 0
        {
            atual = Arvore_ArvoreEsquerda(atual); // segue para a árvore da esquerda
        }
        bitPos++; // pula o bit já lido
        
        // Verificando se a árvore atual é uma folha
        if(Arvore_EhFolha(atual))
        {
            fputc(Arvore_Caracter(atual), output); // caso sim, imprime seu carácter
            atual = huffman; // reiniciando atual
        }
    }
    fclose(output); // fechando arquivo de escrita

    // Destruindo as estruturas utilizadas
    Arvore_DestroiArvore(huffman); // destruindo a árvore de Huffman
    free(bitmapGetContents(map)); // liberando o bitmap
}