#ifndef FILA_H
#define FILA_H

#include "lista.h"


/**
   Provê operações para implementar uma Fila Circular.
   Uma fila é uma estrutura de dados que usa o método 
   de inserção: "primeiro a entrar, primeiro a sair". 
   A idéia fundamental da fila é que só é permitido 
   inserir um novo elemento no final da fila. Além disso,
   só é permitido retirar um elemento do início da mesma.
   Uma fila sem nenhum elemento é denominada fila vazia.
   
   O módulo provê operações básicas para fila circular, tais
   como criar, inserir elemento, verificar seu tamanho, se está 
   vazia, se está cheia, etc. 
*/

// Estrutura de dados que representa uma fila
typedef void* Fila;
 
/** 
  Retorna uma fila vazia. Cria uma fila para poder ser usada 
  no programa da forma desejada  
*/
Fila criaFila();

/**  
  Insere o item elemento no final da Fila fila. O tamanho da
  fila é acrescido de 1 elemento. 
*/
void enqueue(Fila f, Item item);

/** 
  Remove o primeiro elemento da Fila fila. 
  O tamanho da fila é diminuido de 1 elemento. 
*/
Item dequeue(Fila f);

/** 
  Retorna 1 se a Fila fila estiver vazia
  ou 0 se não estiver vazia.  
*/ 
int filaVazia(Fila f);

/** 
  Retorna 1 se a Fila fila estiver cheia
  ou 0 se não estiver.  
*/ 
void destroiFila(Fila f);

#endif