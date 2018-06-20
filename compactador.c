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

#define ASCII_TAM 256

/**
 * Função auxiliar que inicializa n posições de um vetor:
 * Input: vetor e número de posições;
 * Output: nunhum;
 * Condições: vetor existe e tem tamanho máximo >= n;
 * Efeitos Colaterais: todas as n primeiras posicões do vetor ficam iguais a 0;
*/
static void InicializaVetor(int* vet, unsigned int n)
{
    int i; // Variavel de incrementação
    for(i = 0 ; i < n ; i++) // inicializando conteudo do bitvet como 0
    {
        vet[i] = 0;
    }
}

/**
 * Função auxiliar que converte um caracter sem sinal para binário:
 * Input: vetor que armazenará o binário e um caracter sem sinal;
 * Output: vetor contendo o caracter convertido em seus 8 primeiros bits (posição 0 mais significativa);
 * Condições: vetor existe e tem tamanho máximo >= 8;
 * Efeitos Colaterais: vetor contem o binário de 8 bits;
*/
static void ConverteParaBinario(int* vet, unsigned char a)
{
    int i; // Variavel de incrementação

    // Realizando divisões por 2 e armazenado os restos no bitvet
    for(i = 7 ; i >= 0 ; a /= 2, i--)
    {
        vet[i] = a % 2;
    }
}

/**
 * Função auxiliar que converte um inteiro sem sinal menor que 8 para binário:
 * Input: vetor que armazenará o binário e um inteiro sem sinal;
 * Output: vetor contendo o inteiro convertido em seus 3 primeiros bits (posição 0 mais significativa);
 * Condições: vetor existe e tem tamanho máximo >= 3;
 * Efeitos Colaterais: vetor contem o binário de 3 bits;
*/
static void ConverteIntParaBinario(int* vet, unsigned int a)
{
    int i; // variavel de incrementação

    // Realizando divisões por 2 e armazenado os restos no bitvet
    for(i = 2 ; i >= 0 ; a /= 2, i--)
    {
        vet[i] = a % 2;
    }
}

/**
 * Função auxiliar que converte um binário para caracter sem sinal:
 * Input: vetor contendo um binário de 8 bits;
 * Output: o caracter convertido dos 8 primeiros bits do vetor;
 * Condições: vetor existentente e válido;
 * Efeitos Colaterais: nenhum;
*/
static unsigned char ConverteParaCharacter(int* vet)
{
    int i; // Variavel de incrementação
    int num; // armazena o valor do caracter em inteiro decimal

    // Percorrendo os 8 primeiros bits do bitvet
    for(i = 0, num = 0 ; i < 8 ; i++)
    {
        if(vet[i] == 1) // se o bit for 1
        {
            num += pow(2.0, 7 - i); // soma à conversão a respectiva potência de 2
        }
    }

    return (unsigned char) num; // retorna caracter sem sinal
}

/**
 * Função auxiliar que converte um binário de 3 bits para inteiro:
 * Input: vetor contendo um binário de 3 bits;
 * Output: o caracter convertido dos 3 primeiros bits do vetor;
 * Condições: vetor existente e válido;
 * Efeitos Colaterais: nenhum;
*/
static int ConverteParaInt(int* vet)
{
    int i; // Variavel de incrementação
    int num; // armazena o valor do binário em inteiro decimal

    // Percorrendo os 3 primeiros bits do bitvet
    for(i = 0, num = 0 ; i < 3 ; i++)
    {
        if(vet[i] == 1) // se o bit for 1
        {
            num += pow(2.0, 2 - i); // soma à conversão a respectiva potência de 2
        }
    }

    return num; // retorna o inteiro
}

/**
 * Função auxiliar que lê um carácter do arquivo de entrada e o converte para binário:
 * Input: arquivo de leitura, lista de inteiros e um vetor;
 * Output: inteiro de carater booleano indicando se ainda há caracteres para serem lidos;
 * Condições: arquivo existe, lista alocada e vetor com tamanho máximo de pelo menos 8;
 * Efeitos Colaterais: lista incrementada em 8 bits e vetor contem o binário;
*/
static int PegaCaracter(FILE* input, Lista* lista, int* vet)
{
    int i; // variável de incrementação
    char c; // variável auxiliar para leitura do arquivo
    Item* item; // variável auxiliar

    c = fgetc(input); // retira um carácter do arquivo
    if(c == EOF) // caso seja o fim do arquivo
    {
        return 0; // retorna 0
    }

    ConverteParaBinario(vet,c); // convertendo c para binário e armazenando no vetor
    // Copiando o vetor no fim da lista
    for(i = 0 ; i < 8 ; i++)
    {
        item = Lista_NovoItem("int*", ListaCaminho_CriaInt(vet[i]));
        Lista_ListaAdd(lista, item, Lista_TamanhoLista(lista));
    }

    return 1;
}

/**
 * Função auxiliar que codifica uma árvore para ser impressa e a armazena numa lista de inteiros:
 * Input: lista de inteiros, árvore e um vetor;
 * Output: nenhum;
 * Condições: lista tipo int, árvore e vetor existentem e vetor tem tamanho máximo de pelo menos 8;
 * Efeitos Colaterais: a lista recebe a árvore codificada no final;
*/
static void CompactaArvore(Lista* lista, Arvore* arvore, int* vet)
{
    if(Arvore_EhFolha(arvore)) // se a árvore for uma folha
    {
       int i; // Variavel de incrementação
       ConverteParaBinario(vet,Arvore_Caracter(arvore)); // converte o carácter para binário
       Lista_ListaAdd(lista,Lista_NovoItem("int*",ListaCaminho_CriaInt(0)),Lista_TamanhoLista(lista)); // insere 0 (codigo para folha) na lista
       for(i = 0 ; i < 8 ; i++) // insere em seguida o caracter em binário
       {
           Lista_ListaAdd(lista,Lista_NovoItem("int*",ListaCaminho_CriaInt(vet[i])),Lista_TamanhoLista(lista));
       }
    }
    else // se a árvore for um nó
    {
        Lista_ListaAdd(lista,Lista_NovoItem("int*",ListaCaminho_CriaInt(1)),Lista_TamanhoLista(lista)); // insere 1 (codigo para nó) na lista
        CompactaArvore(lista,Arvore_ArvoreEsquerda(arvore),vet); // compacta a árvore da esquerda
        CompactaArvore(lista,Arvore_ArvoreDireita(arvore), vet); // compacta a árvore da direita
    }
}

/**
 * Função auxiliar que reconstroi a árvore de Huffman compactada:
 * Input: arquivo de leitura, lista de inteiros e um vetor;
 * Output: árvore de Huffman;
 * Condições: arquivo existe, lista alocada e vetor com tamanho máximo de pelo menos 8;
 * Efeitos Colaterais: lista modificada;
*/
static Arvore* DescompactaArvore(FILE* input, Lista* bits, int* vet)
{
    if(Lista_ListaVazia(bits)) // se não houverem bits para ser lidos
    {
        PegaCaracter(input,bits,vet); // lê um byte do arquivo
    }
    if( *((int*) Lista_AchaItem(bits, 0))) // se o primeiro bit lido for 1
    {
        Lista_ListaRemove(bits,0,ListaCaminho_LiberaInt); // remove esse bit da lista

        // lê suas arvores esquerda e direita
        Arvore *esq = DescompactaArvore(input,bits,vet);
        Arvore *dir = DescompactaArvore(input,bits,vet);
        // Retorna uma nova árvore nó
        return Arvore_CriaArvore(0, esq, dir);
    }
    else // caso o bit for 0
    {
        Lista_ListaRemove(bits,0,ListaCaminho_LiberaInt); // remove esse bit da lista

        int i;
        if(Lista_TamanhoLista(bits) < 8) // verifica se todos os bits nescessarios estão na lista
        {
            PegaCaracter(input,bits,vet); // caso não, lê mais um byte
        }

        // Inserindo os 8 primeiros bits da lista em vet
        for(i = 0 ; i < 8 ; i++)
        {
            vet[i] = *((int*)Lista_AchaItem(bits,i));
        }

        // Cria uma nova árvore folha com o carácter lido nos atuais primeiros 8 bits da lista
        Arvore* folha = Arvore_CriaFolha(ConverteParaCharacter(vet), 0);
        // Remaove esses bits da lista
        for(i = 0 ; i < 8 ; i++)
        {
            Lista_ListaRemove(bits,0,ListaCaminho_LiberaInt);
        }

        return folha;
    }

}

/**
 * Função auxiliar que imprime os 8 primeiros bits de uma lista como carácter codificado no arquivo especificado:
 * Input: vetor, lista e arquivo;
 * Output: nenhum;
 * Condições: vetor com tamanho de pelo menos 8, lista tipo int com os bits e arquivo existente;
 * Efeitos Colaterais: os 8 primeiros bits são codificados, impressos no arquivo e removidos da lista;
*/
static void ImprimeCaracter(int* vet, Lista* lista, FILE* output)
{
    int i; // Variavel de incrementação
    for(i = 0 ; i < 8 ; i++)
    {
        vet[i] = *((int*) Lista_AchaItem(lista,0));
        Lista_ListaRemove(lista,0,ListaCaminho_LiberaInt);
    }

    fputc(ConverteParaCharacter(vet),output);
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
    char c; // auxiliar que guarda o caracter

    // Inicializando ocorrencias
    for(i = 0 ; i < ASCII_TAM ; i++)
    {
        pesos[i] = 0;
    }

    FILE *input = fopen(arquivo, "r"); // abrindo arquivo

    c = fgetc(input); // selecionando primeiro caracter
    while(c != EOF) // percorrendo todo arquivo
    {
        pesos[c] += 1; // contando as ocorrencias de cada caracter
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
    FILE* output = fopen(saida, "w+"); // abrindo arquivo de escrita
    FILE* input = fopen(entrada, "r"); // abrindo arquivo de leitura
    char c; // variável auxiliar para leitura do arquivo
    Lista* caminho = ListaCaminho_CriaLista(arvoreHuffman); // lista com os novos códigos para cada carácter
    Lista* bits = ListaCaminho_CriaListaInt(); // lista auxiliar que guarda a sequência de bits a serem impressos
    Lista* l; // lista auxiliar que guarda o caminho até certo carácter
    int caracter[8]; // vetor auxiliar que guarda o carácter a ser impresso

    InicializaVetor(caracter, 8); // inicializando todos os bits do vetor como 0

    // Reservando os 3 próximos bits da lista para informar o número de bits sobrando no último carácter do arquivo
    for(i = 0 ; i < 3 ; i++)
    {
        Lista_ListaAdd(bits, Lista_NovoItem("int*",ListaCaminho_CriaInt(0)),Lista_TamanhoLista(bits));
    }

    // Imprimindo a árvore compactada
    // Compacta a árvore em bits
    CompactaArvore(bits,arvoreHuffman, caracter);
    // Imprime os bits da lista, de 8 em 8, como carácteres
    while(Lista_TamanhoLista(bits) >= 8)
    {
        ImprimeCaracter(caracter,bits,output);
    }

    // Imprimindo em seguida a conversão do arquivo de entrada
    c = fgetc(input); // inicializando com o primeiro carácter
    while(c != EOF) // varrendo o arquivo de entrada
    {
        l = ListaCaminho_Caminho(caminho,c); // buscando o caminho para o carácter lido
        // Inserindo o caminho na lista bits
        for(i = 0 ; i < Lista_TamanhoLista(l) ; i++)
        {
            Lista_ListaAdd(bits,Lista_NovoItem("int*",ListaCaminho_CriaInt( *((int*) Lista_AchaItem(l,i)) ) ),Lista_TamanhoLista(bits));
        }

        // Imprimindo os bits caso completem um carácter
        while(Lista_TamanhoLista(bits) >= 8)
        {
            ImprimeCaracter(caracter,bits,output);
        }

        c = fgetc(input); // atualizando carácter
    }
    fclose(input); // fechando arquivo de leitura

    if(!Lista_ListaVazia(bits)) // se sobrarem bits para serem impressos
    {
        i = Lista_TamanhoLista(bits); // guardando tamanho atual da lista
        // Completa a lista até 8 bits
        while(Lista_TamanhoLista(bits) < 8)
        {
            Lista_ListaAdd(bits,Lista_NovoItem("int*",ListaCaminho_CriaInt(0)),Lista_TamanhoLista(bits));
        }
        
        // Imprime o último carácter
        ImprimeCaracter(caracter,bits,output);
        
        i = 8 - i; // i recebe o número de bits sobrando no último carácter

        rewind(output); // volta ao inicio do arquivo
        c = fgetc(output); // lê o primeiro carácter
        ConverteParaBinario(caracter,c); // converte-o para binário
        ConverteIntParaBinario(caracter,i); // substitui os 3 bits reservados no inicio pelo valor de i em binário

        rewind(output); // retorna ao começo novamente
        fputc(ConverteParaCharacter(caracter),output); // substitui o primeiro carácter
    }
    fclose(output); // fechando arquivo de escrita

    // Destruindo as listas utilizadas
    ListaCaminho_DestroiLista(caminho); // destruindo a lista de inteiros
    Lista_DestroiLista(bits,ListaCaminho_LiberaInt); // destruindo a lista de impressão
}

// Descompactando e imprimindo um arquivo:
void Compactador_Descompacta(char* entrada, char* saida)
{
    int i;
    FILE* input = fopen(entrada, "r"); // abrindo arquivo de leitura
    FILE* output = fopen(saida, "w"); // abrindo arquivo de escrita
    Lista *bits = ListaCaminho_CriaListaInt(); // lista auxiliar que armazenar a sequencia binária lida do arquivo de entrada
    int vet[8]; // auxiliar de conversão
    Arvore *huffman; // árvore de compactação/descompactação armazenada no cabeçalho do arquivo compactado
    Arvore *atual; // árvore auxiliar de busca
    int naoUtilizados; // armazena quantos bits não devem ser considerados no ultimo carácter do arquivo
    int fda = 0; // variável auxiliar que indica o fim do arquivo

    InicializaVetor(vet,8);
    //fseek(input, strlen(term) +1, SEEK_SET);

    PegaCaracter(input,bits,vet); // pegando primeiro carácter
    naoUtilizados = ConverteParaInt(vet); // convertendo seus 3 primeiros bits para inteiro
    // Removendo esses 3 bits da lista
    for(i = 0 ; i < 3 ; i++)
    {
        Lista_ListaRemove(bits, 0, ListaCaminho_LiberaInt);
    }

    // Descompactando a árvore de Huffman
    huffman = DescompactaArvore(input, bits, vet);

    // Preparando para iniciar o loop
    // Incrementando a lista de inteiros, caso esteja vazia
    if(Lista_ListaVazia(bits))
    {
        PegaCaracter(input,bits,vet);
    }

    atual = huffman; // inicializando auxiliar
    while(!Lista_ListaVazia(bits)) // loop para quando a lista de inteiros estiver vazia
    {
        // Incrementando a lista caso seu tamanho seja menor que 8
        if(Lista_TamanhoLista(bits) <= 8)
        {
            if( !(PegaCaracter(input,bits,vet) || fda) ) // se PegaCaracter retornar 0, restou apenas o ultimo carácter do arquivo
            {
                fda = 1; // incrementa fda para que essa condicional não se repita
                // Remove da lista os bits não utilizados, cuja quantidade foi informada no cabeçalho
                for(i = 0 ; i < naoUtilizados ; i++)
                {
                    Lista_ListaRemove(bits, 8 - naoUtilizados, ListaCaminho_LiberaInt);
                }
            }
        }
        
        // Avaliando caminho para a folha
        if( *((int*) Lista_AchaItem(bits,0)) ) // se o bit for 1
        {
            atual = Arvore_ArvoreDireita(atual); // segue para a árvore da direita
            Lista_ListaRemove(bits, 0, ListaCaminho_LiberaInt); // remove o bit já lido
        }
        else // se o bit for 0
        {
            atual = Arvore_ArvoreEsquerda(atual); // segue para a árvore da esquerda
            Lista_ListaRemove(bits, 0, ListaCaminho_LiberaInt); // remove o bit já lido
        }
        
        // Verificando se a árvore atual é uma folha
        if(Arvore_EhFolha(atual))
        {
            fputc(Arvore_Caracter(atual), output); // caso sim, imprime seu carácter
            atual = huffman; // reiniciando atual
        }
    }

    // Fechando os arquivos
    fclose(input); // fechando arquivo de leitura
    fclose(output); // fechando arquivo de escrita
    // Destruindo as estruturas utilizadas
    Lista_DestroiLista(bits,ListaCaminho_LiberaInt); // destruindo a lista de leitura
    Arvore_DestroiArvore(huffman); // destruindo a árvore de Huffman
}