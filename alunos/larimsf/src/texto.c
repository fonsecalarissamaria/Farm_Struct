#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texto.h"


typedef struct 
{
    double x, y, xRot, yRot;  // coordenadas da Texto
    int i;                    // identificador do Texto 
    char ancoraText[5];       //ancora do texto
    char corb[20];            //cor da borda do Texto
    char corp[20];            //cor do preenchimento do texto 
    char texto[100];          //texto do Texto
    char fontFamily[20];      //Fonte do texto 
    char fontWeigth[20];     //Estilo do texto 
    double angulo;            //ângulo inicial do Texto  
    int size;                 //tamanho da fonte
} StTexto;



Texto criaTexto(double x, double y, int i, char *ancora, char *txto, char *corb, char *corp, double size, char *fontFamily, char *fontWeigth)
{
    StTexto *text = (StTexto *) malloc(sizeof(StTexto));

    if (text == NULL) 
    {
        printf("Erro: nao foi possivel alocar memoria.\n");
        exit(1);
    }
    text->x = x;
    text->y = y;
    text->i = i; 
    text->size = size; 
    strcpy(text->corb, corb);
    strcpy(text->corp, corp);
    strcpy(text->ancoraText, ancora);
    strcpy(text->fontFamily, fontFamily);
    strcpy(text->fontWeigth, fontWeigth);
    strcpy(text->texto, txto);

    //printf("Ancora parametro: %s Ancora guardada: %s\n", ancora, text->ancoraText);
    return text;
}


char *getCorbText(Texto text) 
{
    StTexto *st = (StTexto *)text;
    return st->corb; 
}

char *getCorpText(Texto text) 
{
    StTexto *st = (StTexto *)text;
    return st->corp; 
}


char *getAncoraText(Texto text) 
{
    StTexto *st = (StTexto *)text;
    return st->ancoraText; 
}


char *getTextoText(Texto text) 
{
    StTexto *st = (StTexto *)text;
    return st->texto; 
}


double getPosicXText(Texto text)
{
    StTexto *st = (StTexto *)text;
    return st->x; 
}

double getAnguloText(Texto text)
{
    StTexto *st = (StTexto *)text;
    return st->angulo; 
}

int getSizeText(Texto text)
{
    StTexto *st = (StTexto *)text;
    return st->size;
}

double getPosicYText(Texto text)
{
    StTexto *st = (StTexto *)text;
    return st->y; 
}


int getIdentificadorText(Texto text)
{
    StTexto *st = (StTexto *)text;
    return st->i; 
}

void moveTexto(Texto text, double xAntigo, double yAntigo, double dx, double dy)
{
    StTexto *st = (StTexto *)text;
    //printf("\nantigo X do texto: %d antigo Y do texto: %d\n", st->x, st->y);
    st->x = xAntigo + dx;
    st->y = yAntigo + dy;
    //printf("\nnovo X do texto: %d novo Y do texto: %d\n", st->x, st->y);
}


void giraTexto(Texto text, double graus)
{
    StTexto *st = (StTexto *)text;
    st->angulo += graus;  
    //printf("\nnovo Grau do texto: %lf\n", st->angulo);
}

double getXRot(Texto text)
{
    StTexto *st = (StTexto *)text;
    return st->xRot; 
}

double getYRot(Texto text)
{
    StTexto *st = (StTexto *)text;
    return st->yRot; 
}

 
char *getFontFamily(Texto text) 
{
    StTexto *st = (StTexto *)text;
    return st->fontFamily; 
}

char *getFontWeigth(Texto text) 
{
    StTexto *st = (StTexto *)text;
    return st->fontWeigth; 
}