#ifndef FORMA_H
#define FORMA_H

#include <stdbool.h>
#include "lista.h"
#include "arq_svg.h"

/*
  Provê operações para implementar uma Forma.

  Uma Forma é um elemento que pode guardar qual é o tipo de 
  outras estruturas para reconhecimento das mesmas depois.  
  Por exemplo, a Forma forma pode guardar que o TAD Retangulo
  é um retangulo, para que, em uma lista generalizada de TAD's,
  possa ser reconhecido quem é Retangulo, Circulo, Linha, etc.  
  A Forma deve ser guardada por guarda_formato. A seguir, operações
  para reportar seus parâmetros podem ser usadas por esse contrato.
 */

typedef void *Forma;


/*
  Cria e retorna um TAD do tipo Forma. Recebe como parâmetro o
  ponteiro para objeto que será identificado posteriormente (objeto)
  e o formato que ele possui. Por exemplo, se for um retangulo, recebe
  "r" para ser identificado depois.  
  Com isso, a função aloca um espaço na memória para 
  o TAD Forma, guarda em registro seus parâmetros e retorna 
  um ponteiro para o tipo Forma criado.   
 */
Forma guarda_formato(void* objeto, char* formato);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Forma forma) e retorna o formato  
  dessa forma. 
 */
char* getFormato(Forma forma);



/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Forma forma) e retorna o identificador  
  dessa forma. 
 */
int identificar_forma(Forma forma);



/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Forma forma) e retorna o objeto  
  dessa forma. 
 */
void* getObjeto(Forma forma);



/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (void* obejto) e retorna o tipo  
  desse objeto. 
 */
char* getTipo(void* objeto);


#endif 
