#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

static void Printa(Lista*);
static void Freedom(void*);

int main(void)
{
    int *a = (int*) malloc(sizeof(int));
    *a = 1;
    int *b = (int*) malloc(sizeof(int));
    *b = 2;
    int *c = (int*) malloc(sizeof(int));
    *c = 3;
    int *d = (int*) malloc(sizeof(int));
    *d = 4;
    int *e = (int*) malloc(sizeof(int));
    *e = 5;
    int *f = (int*) malloc(sizeof(int));
    *f = 6;

    Lista *lista = Lista_NovaLista("int");

    printf("Vazia = %d\n", Lista_ListaVazia(lista));

    Lista_ListaAdd(lista,Lista_NovoItem("int",a),0);

    printf("Vazia = %d\n", Lista_ListaVazia(lista));

    Printa(lista);
    
    Lista_ListaAdd(lista,Lista_NovoItem("int",b),3);

    Printa(lista);

    Lista_ListaAdd(lista,Lista_NovoItem("int",c),1);

    Printa(lista);

    Lista_ListaAdd(lista,Lista_NovoItem("int",d),0);

    Printa(lista);

    Lista_ListaAdd(lista,Lista_NovoItem("int",e),Lista_TamanhoLista(lista));

    Printa(lista);
    
    Lista_ListaAdd(lista,Lista_NovoItem("int",f),0);

    Printa(lista);

    Lista_ListaRemove(lista,0,Freedom);

    Printa(lista);

    Lista_ListaRemove(lista,10,Freedom);

    Printa(lista);

    Lista_ListaRemove(lista,Lista_TamanhoLista(lista) -1,Freedom);

    Printa(lista);

    Lista_ListaRemove(lista,1,Freedom);

    Printa(lista);

    Lista_DestroiLista(lista,Freedom);

    return 0;
}

static void Printa(Lista* lista)
{
    int i;
    printf("Tamanho = %d\n", Lista_TamanhoLista(lista));
    for(i = 0 ; i < Lista_TamanhoLista(lista) ; i++)
    {
        printf("%d = %d\n", i, *( (int*) Lista_AchaItem(lista,i)));
    }
}

static void Freedom(void* item)
{
    free(item);
}