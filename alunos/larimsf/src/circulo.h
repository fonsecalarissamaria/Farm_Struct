#ifndef _CIRCULO_H
#define _CIRCULO_H

#include <stdbool.h>

typedef void *Circulo;

 
/*
  Provê operações para implementar um Circulo.

  Um Circulo é um elemento que pode representar diversas 
  estruturas dentro de um determinado contexto. Por exemplo, 
  se meu cenário de implementação é uma horta, posso criar um 
  círculo para representar uma estrutura de um repolho, uma
  laranja, uma melancia, etc.   
  O Círculo deve ser criado por criaCirculo. A seguir, operações
  para movimentá-lo e reportar seus parâmetros podem ser usadas 
  por esse contrato.
 */



/*
  Cria e retorna um TAD do tipo Circulo. Recebe como parâmetro a
  posição central do círculo (x,y), qual deve ser o seu raio (raio),
  seu identificador (i), qual deve ser a cor da sua borda (corb) e 
  qual deve ser a cor do seu preenchimento (corp). 
  Com isso, a função aloca um espaço na memória para 
  o TAD Circulo, guarda em registro seus parâmetros e retorna um ponteiro 
  para o tipo Circulo criado.   
 */
Circulo criaCirculo(double x, double y, double raio, int i, char corb[], char corp[]);




/*
  Cria um ponteiro auxiliar para o endereço de registros 
  passado por parâmetro (Circulo circle) e retorna o raio 
  desse círculo. 
 */
double getRaio(Circulo circle);




/*
  Cria um ponteiro auxiliar para o endereço de registros 
  passado por parâmetro (Circulo circle) e retorna a cor da borda 
  desse círculo. 
 */
char *getCorbCircle(Circulo circle);



/*
  Cria um ponteiro auxiliar para o endereço de registros 
  passado por parâmetro (Circulo circle) e retorna a cor de  
  preenchimento desse círculo. 
 */
char *getCorpCircle(Circulo circle);



/*
  Cria um ponteiro auxiliar para o endereço de registros 
  passado por parâmetro (Circulo circle) e retorna a posição X
  desse círculo. 
 */
double getPosicXCircle(Circulo circle);



/*
  Cria um ponteiro auxiliar para o endereço de registros 
  passado por parâmetro (Circulo circle) e retorna a posição Y 
  desse círculo. 
 */
double getPosicYCircle(Circulo circle);



/*
  Recebe por parâmetro o ponteiro para um Circulo, a posição
  atual dele (coordenadas xAntigo, yAntigo) e qual deve ser 
  a distancia no eixo x (dx) e a distância no eixo y (dy) que
  esse círculo deve percorrer para se movimentar.
  A função soma as coordenadas passadas: 
  xAntigo + dx
  yAntigo + dy 
  E atualiza no registro desse círculo suas novas coordenadas. 
 */
void moveCirculo(Circulo circle, double xAntigo, double yAntigo, double dx, double dy);



/*
  Cria um ponteiro auxiliar para o endereço de registros 
  passado por parâmetro (Circulo circle) e retorna o identificador  
  desse círculo. 
 */
int getIdentificadorCirculo(Circulo circle);







#endif