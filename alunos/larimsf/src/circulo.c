#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circulo.h"


typedef struct circ
{
    char c; 
    double x, y;       // canto inferior esquerdo
    double raio;  //   //raio 
    int i;          // identificador do circulo 
    char corb[20];  //cor da borda 
    char corp[20];  //cor do preenchimento 
    double angulo;  //ângulo inicial do circulo  
} StCirculo;



Circulo criaCirculo(double x, double y, double raio, int i, char *corb, char *corp)
{
    StCirculo *circle = (StCirculo *) malloc(sizeof(StCirculo));
    void *st = (void *)circle;

    if (circle == NULL) 
    {
        printf("Erro: nao foi possivel alocar memoria.\n");
        exit(1);
    }
    circle->x = x;
    circle->y = y;
    circle->raio = raio; 
    circle->i = i; 
    strcpy(circle->corb, corb);
    strcpy(circle->corp, corp);

    return st;
}

double getRaio(Circulo circle) 
{
    StCirculo *st = (StCirculo *)circle;
    return st->raio;
}


char *getCorbCircle(Circulo circle) 
{
    StCirculo *st = (StCirculo *)circle;
    return st->corb; 
}

char *getCorpCircle(Circulo circle) 
{
    StCirculo *st = (StCirculo *)circle;
    return st->corp; 
}

double getPosicXCircle(Circulo circle)
{
    StCirculo *st = (StCirculo *)circle;
    return st->x; 
}

double getPosicYCircle(Circulo circle)
{
    StCirculo *st = (StCirculo *)circle;
    return st->y; 
}


void moveCirculo(Circulo circle, double xAntigo, double yAntigo, double dx, double dy)
{
    StCirculo *st = (StCirculo *)circle;
    //printf("\nantigo X do circulo: %lf antigo Y do circulo: %lf\n", st->x, st->y);
    st->x = (xAntigo + dx);
    st->y = (yAntigo + dy);
    //printf("\nnovo X do circulo: %lf novo Y do circulo: %lf\n", st->x, st->y);
}

int getIdentificadorCirculo(Circulo circle)
{
    StCirculo *st = (StCirculo *)circle;
    return st->i; 
}










