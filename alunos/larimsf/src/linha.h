#ifndef _LINHA_H
#define _LINHA_H

#include <stdbool.h>


typedef void *Linha;

/*
  Provê operações para implementar uma Linha.

  Uma Linha é um elemento que pode representar diversas 
  estruturas dentro de um determinado contexto. Por exemplo, 
  se meu cenário de implementação é uma horta, posso criar uma 
  linha para representar a estrutura de uma estrada de terra, 
  mato em forma de linhas, etc.   
  A Linha deve ser criada por criaLinha. A seguir, operações
  para movimentá-la e reportar seus parâmetros podem ser usadas 
  por esse contrato.
 */



/*
  Cria e retorna um TAD do tipo Linha. Recebe como parâmetro as
  coordenadas das extremidades da linha (x1,y1) e (x2, y2) a ser
  criada, seu identificador (i) e qual deve ser a sua cor (cor). 
  Com isso, a função aloca um espaço na memória para 
  o TAD Linha, guarda em registro seus parâmetros e retorna 
  um ponteiro para o tipo Linha criado.   
 */
Linha criaLinha(double x1, double y1, double x2, double y2, int i, char *cor);



/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Linha line) e retorna a cor dessa linha.
 */
char *getCorLine(Linha line);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Linha line) e retorna o identificador
  dessa linha.
 */
int getIdentificadorLine(Linha line);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Linha line) e retorna a posição X1
  dessa linha.
 */
double getPosicX1Line(Linha line);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Linha line) e retorna a posição Y1
  dessa linha.
 */
double getPosicY1Line(Linha line);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Linha line) e retorna a posição X2
  dessa linha.
 */
double getPosicX2Line(Linha line);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Linha line) e retorna a posição Y2
  dessa linha.
 */
double getPosicY2Line(Linha line);


/*
  Recebe por parâmetro o ponteiro para uma Linha, a posição
  atual dela, coordenadas (x1Antigo, y1Antigo) e (x2Antigo, y2Antigo) 
  e qual deve ser a distancia no eixo x (dx) e a 
  distância no eixo y (dy) que essa linha deve percorrer 
  para se movimentar.
  A função soma as coordenadas passadas e atualiza no 
  registro dessa linha suas novas coordenadas. 
 */
void moveLinha(Linha line, double x1Antigo, double y1Antigo, double x2Antigo, double y2Antigo, double dx, double dy);

#endif