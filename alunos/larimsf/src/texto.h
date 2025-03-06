#ifndef _TEXTO_H
#define _TEXTO_H

#include <stdbool.h>


typedef void *Texto;


/*
  Provê operações para implementar um Texto.

  Um Texto é um elemento que pode representar diversas 
  estruturas dentro de um determinado contexto. Por exemplo, 
  se meu cenário de implementação é uma horta, posso criar um 
  texto para representar estruturas de diversas hortaliças (@, /, %, $), etc.  
  O Texto deve ser criado por criaTexto. A seguir, operações
  para movimentá-lo, girá-lo e reportar seus parâmetros podem 
  ser usadas por esse contrato.
 */


/*
  Cria e retorna um TAD do tipo Texto. Recebe como parâmetro a
  coordenada do texto (x,y), qual deve ser o sua ancora (ancora),
  sendo que as opções são: i (start), m (middle) ou e (end),
  o conteúdo do texto em si (txto), seu identificador (i),
  qual deve ser a cor da sua borda (corb), qual deve ser a 
  cor do seu preenchimento (corp), o tamanho da sua fonte (size),
  o estilo do texto (fontFamily e fontWeigth). 
  Com isso, a função aloca um espaço na memória para 
  o TAD Texto, guarda em registro seus parâmetros e retorna 
  um ponteiro para o tipo Texto criado.   
 */
Texto criaTexto(double x, double y, int i, char *ancora, char *txto, char *corb, char *corp, double size, char *fontFamily, char *fontWeigth);



/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Texto text) e retorna a cor de 
  borda desse texto.  
 */
char *getCorbText(Texto text);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Texto text) e retorna a cor de 
  preenchimento desse texto.  
 */
char *getCorpText(Texto text);



/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Texto text) e retorna a ancora
  desse texto.  
 */
char *getAncoraText(Texto text);



/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Texto text) e retorna a string texto 
  desse texto.  
 */
char *getTextoText(Texto text);



/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Texto text) e retorna o identificador  
  desse texto. 
 */
int getIdentificadorText(Texto text);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Texto text) e retorna o tamanho  
  desse texto. 
 */
int getSizeText(Texto text);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Texto text) e retorna o angulo corrente  
  desse texto. 
 */
double getAnguloText(Texto text);

/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Texto text) e retorna a posição X  
  desse texto. 
 */
double getPosicXText(Texto text);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Texto text) e retorna a posição Y 
  desse texto. 
 */
double getPosicYText(Texto text);


/*
  Recebe por parâmetro o ponteiro para um Texto, a posição
  atual dele (coordenadas xAntigo, yAntigo) e qual deve ser 
  a distancia no eixo x (dx) e a distância no eixo y (dy) que
  esse texto deve percorrer para se movimentar.
  A função soma as coordenadas passadas: 
  xAntigo + dx
  yAntigo + dy 
  E atualiza no registro desse texto suas novas coordenadas. 
 */
void moveTexto(Texto text, double xAntigo, double yAntigo, double dx, double dy);


/*
  Recebe por parâmetro o ponteiro para um Texto, a posição
  atual dele (coordenadas xAntigo, yAntigo), a posição 
  (Xrot, Yrot) central do texto (para que não seja modificada
  ao girar o texto) e qual deve ser o angulo, em graus, 
  que o texto deve ser rotacionado (graus).
  A função soma o ângulo corrente e atualiza no registro 
  desse texto seu novo angulo. 
 */
void giraTexto(Texto text, double graus);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Texto text) e retorna a posição X Central  
  desse texto. 
 */
double getXRot(Texto text);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Texto text) e retorna a posição Y Central  
  desse texto. 
 */
double getYRot(Texto text);


/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Texto text) e retorna o estilo de fonte
  FontWeigth desse texto.
 */
char *getFontWeigth(Texto text); 



/*
  Cria um ponteiro auxiliar para o endereço de registros (struct)
  passado por parâmetro (Texto text) e retorna o estilo de fonte
  FontFamily desse texto.
 */
char *getFontFamily(Texto text);

#endif