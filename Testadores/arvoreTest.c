#include "../arvore.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    Arvore *no = Arvore_CriaFolha('a', 0);

    Arvore *arvore = Arvore_CriaArvore(
        0,
        Arvore_CriaArvore(
            0,
            Arvore_CriaFolha('b', 1),
            Arvore_CriaFolha('c', 1)
        ),
        Arvore_CriaArvore(
            0,
            Arvore_CriaFolha('d', 1),
            no
        )
    );

    Lista *caminho = Arvore_Caminho(arvore, no);
    
    printf("Caminho da arvore ate o no:\n");
    for (int i = (Lista_TamanhoLista(caminho) - 1); i >= 0; i--)
    {
        int *item = (int*) Lista_AchaItem(caminho, i);
        printf("%d ", *item);
    }
    printf("\n");

    Lista_DestroiLista(caminho, free);
    Arvore_DestroiArvore(arvore);
    
    return 0;
}