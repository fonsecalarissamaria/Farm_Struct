#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


struct StNode
{
  struct StNode *prox, *ant;
  Item info;
};

struct StLista
{
  struct StNode *prim, *ult;
  int capac;
  int length;
};

typedef struct StLista ListaImpl;
typedef struct StNode Node;


struct StIterator
{
  Node *curr;
  bool reverso;
};

typedef struct StIterator IteratorImpl;

Lista createLst(int capacidade)
{
  ListaImpl *newLista = (ListaImpl *) malloc (sizeof(ListaImpl));
  newLista->prim = NULL;
  newLista->ult = NULL;
  newLista->capac = capacidade;
  newLista->length = 0;

  return newLista; 
}

int lengthLst(Lista L)
{
  ListaImpl *lst = ( ListaImpl *)L;
  return lst->length;
}


int maxLengthLst(Lista L)
{
  ListaImpl *lst = ( ListaImpl *)L;
  return lst->capac;
}


bool isEmptyLst(Lista L)
{
  ListaImpl *lst = ( ListaImpl *)L;
  return lst->length == 0;
}



bool isFullLst(Lista L)
{
  ListaImpl *lst = ( ListaImpl *)L;
  return lst->length == lst->capac;
}

Posic insertLst(Lista L, Item info)
{
  ListaImpl *lst = ( ListaImpl *)L;
  Node *newNode = (Node *) malloc (sizeof(Node));
  newNode->info = info;
  newNode->prox = NULL;
  
  if (isEmptyLst(L)){
    lst->prim = newNode;
  }
  else{
    lst->ult->prox = newNode;
  }
    
  newNode->ant = lst->ult;
  lst->ult = newNode;
  lst->length++;

  return newNode;
}



Item popLst(Lista L)
{
  ListaImpl *lst = (ListaImpl *)L;
  Item item = lst->prim->info;
  Node *paux;
  paux = lst->prim;
  lst->prim = paux->prox;

  if(paux->prox == NULL){
    // lista vai ficar vazia
    lst->ult = NULL;
  }
  else{
    paux->prox->ant = NULL;
  }

  lst->length--;
  free(paux);
  return item;
}

void removeLst(Lista L, Posic p)
{
  
  ListaImpl *lst = (ListaImpl *)L;
  Node *paux = (Node *)p;

  if (paux == lst->prim){
    lst->prim = paux->prox;
  }
  else{
    paux->ant->prox = paux->prox;
  }

  if (paux == lst->ult){
    lst->ult = paux->ant;
  }
  else{
    paux->prox->ant = paux->ant;
  }

  lst->length--;
  free(paux);
}


bool isNodeRemoved(Lista L, Posic p)
{
  ListaImpl *lst = (ListaImpl *)L;
  Node *paux = (Node *)p;

  Node *current = lst->prim;

  while (current != NULL)
  {
    if (current == paux)
    {
      return false; // O nó ainda está presente na lista
    }
    current = current->prox;
  }

  return true; // O nó foi removido da lista
}


Item getLst(Lista L, Posic p)
{
  ListaImpl *lst = ( ListaImpl *)L;
  Node *paux = (Node *)p;
  return paux->info;
}

Posic getFirstLst(Lista L)
{
  ListaImpl *lst = ( ListaImpl *)L;
  Node *paux;
  paux = L;
  return paux->prox;  
}

Posic getNextLst(Lista L,Posic p)
{
  ListaImpl *lst = ( ListaImpl *)L;
  Node *paux = (Node *)p;
  return paux->prox;
}

Posic getPreviousLst(Lista L, Posic p)
{
  Node *paux = (Node *)p;
  return paux->ant;
}


void killLst(Lista L) 
{
    Posic atual = getFirstLst(L);
    Posic prox;

    while (atual != NULL) 
    {
        prox = getNextLst(L, atual);
        removeLst(L, atual);
        atual = prox;
    }
    free(L);
}

/*

Iterador
*/


Iterador createIterador(Lista L, bool reverso) 
{
  IteratorImpl* newIterator = (IteratorImpl*) malloc(sizeof(IteratorImpl));
  ListaImpl* lst = (ListaImpl*) L;
  newIterator->reverso = reverso;

  if (reverso) 
  {
    newIterator->curr = lst->ult;
  } 
  else 
  {
    newIterator->curr = lst->prim;
  }

  return (Iterador) newIterator;
}

bool isIteratorEmpty(Lista L, Iterador it) 
{
  ListaImpl *lst = (ListaImpl *) L;
  Node *pos = ((IteratorImpl *) it)->curr;

  if (pos == NULL) 
  {
    return true;
  }

  if (((IteratorImpl *) it)->reverso) 
  {
    return pos == lst->prim->ant;
  } 
  else 
  {
    return pos == lst->ult->prox;
  }
}

Item getIteratorCurrent(Lista L, Iterador it)
{
  IteratorImpl *itimpl = (IteratorImpl *)it;
  Posic p = itimpl->curr;
  return getLst(L, p);
}

Item getIteratorNext(Lista L, Iterador it)
{
  IteratorImpl *itimpl = (IteratorImpl *)it;
  Item valor = itimpl->curr->info;
  Posic p = itimpl->curr;
  if (itimpl->reverso) 
  {
    itimpl->curr = (Node *)getPreviousLst(L, p);
  }else 
  {
    itimpl->curr = (Node *)getNextLst(L, p);
  }
  return valor;
}

Item getIteratorPrevious(Lista L, Iterador it)
{
  IteratorImpl *itimpl = (IteratorImpl *)it;
  Item valor = itimpl->curr->info;
  Posic p = itimpl->curr;
  if (itimpl->reverso) 
  {
    itimpl->curr = (Node *)getNextLst(L, p);
  }else 
  {
    itimpl->curr = (Node *)getPreviousLst(L, p);
  }
  return valor;
}

Iterador getIterator(Lista L, bool reverso)
{
  ListaImpl *lst = (ListaImpl *)L;
  IteratorImpl *newIterator = (IteratorImpl *)malloc(sizeof(IteratorImpl));
  newIterator->reverso = reverso;
  if (reverso) 
  {
    newIterator->curr = lst->ult;
  }else 
  {
    newIterator->curr = lst->prim;
  }
    return newIterator;
}

void removeIterator(Iterador it)
{
  free(it);
}

void killIterator(Lista L, Iterador it) 
{
  removeIterator(it);
}


void map(Lista *list, Apply f, Lista *listaA) 
{
    Node *current = getFirstLst(list);
    while (current != NULL) 
    {
        Item i = getLst(list, current);
        f(listaA, i);
        current = current->prox;
    }
}


Lista* filter(Lista *list, Check f, void *extra) 
{
    Lista *newList = createLst(500);
    Node *current = getFirstLst(list);
    while (current != NULL) 
    {
        if (f(current->info, extra)) 
        {
            insertLst(newList, current->info);
        }
        current = current->prox;
    }
    return newList;
}


void fold(Lista *list, ApplyClosure f, void *c) 
{
    Node *current = getFirstLst(list);
    while (current != NULL) 
    {
        f(current->info, c);
        current = current->prox;
    }
}

