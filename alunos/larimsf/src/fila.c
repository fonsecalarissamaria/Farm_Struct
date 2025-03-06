#include "fila.h"
#include <stddef.h>
#include <stdlib.h>

struct elemento
{
    Item item;
    struct elemento *proximo; 
};

struct fila
{
    struct elemento *inicio;
    struct elemento *fim;
};

Fila criaFila()
{
    struct fila *fila = (struct fila *)malloc(sizeof(struct fila));
    fila->inicio = NULL;
    fila->fim = NULL;
    return fila;
}

void enqueue(Fila f, Item item)
{
    struct fila *fila = (struct fila *)f;
    struct elemento *elem = (struct elemento *)malloc(sizeof(struct elemento));

    if (fila->inicio == NULL)
    {
        fila->inicio = elem;
        fila->fim = elem;
        elem->item = item;
        elem->proximo = NULL;
    }
    else
    {
        fila->fim->proximo = elem;
        fila->fim = elem;
        elem->item = item;
        elem->proximo = NULL;
    }
}

Item dequeue(Fila f)
{
    struct fila *fila = (struct fila *)f;
    struct elemento *elem = fila->inicio;

    if (elem == NULL)
        return NULL;

    fila->inicio = elem->proximo;

    if (elem == fila->fim)
        fila->fim = NULL;

    Item info = elem->item;
    free(elem);

    return info;
}

int filaVazia(Fila f)
{
    struct fila *fila = (struct fila *)f;
    return fila->inicio == NULL;
}

void destroiFila(Fila f)
{
    struct fila *fila = (struct fila *)f;
    struct elemento *elem = fila->inicio;
    struct elemento *aux;

    while (elem != NULL)
    {
        aux = elem->proximo;
        free(elem);
        elem = aux;
    }

    free(fila);
}