#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linha.h"


typedef struct 
{
    double x1, y1, x2, y2;  // coordenadas da linha
    int i;               // identificador do linha 
    char cor[20];        //cor da linha
    double angulo;       //ângulo inicial da linha  
} StLinha;



Linha criaLinha(double x1, double y1, double x2, double y2, int i, char *cor)
{
    StLinha *line = (StLinha *) malloc(sizeof(StLinha));

    if (line == NULL) 
    {
        printf("Erro: nao foi possivel alocar memoria.\n");
        exit(1);
    }
    line->x1 = x1;
    line->y1 = y1;
    line->x2 = x2;
    line->y2 = y2;
    line->i = i; 
    strcpy(line->cor, cor);

    return line;
}


char *getCorLine(Linha line) 
{
    StLinha *st = (StLinha *)line;
    return st->cor; 
}


double getPosicX1Line(Linha line)
{
    StLinha *st = (StLinha *)line;
    return st->x1; 
}

double getPosicY1Line(Linha line)
{
    StLinha *st = (StLinha *)line;
    return st->y1; 
}


double getPosicX2Line(Linha line)
{
    StLinha *st = (StLinha *)line;
    return st->x2; 
}


double getPosicY2Line(Linha line)
{
    StLinha *st = (StLinha *)line;
    return st->y2;
}


int getIdentificadorLine(Linha line)
{
    StLinha *st = (StLinha *)line;
    return st->i; 
}

void moveLinha(Linha line, double x1Antigo, double y1Antigo, double x2Antigo, double y2Antigo, double dx, double dy)
{
    StLinha *st = (StLinha *)line;; 
    //printf("\nantigo X1 da linha: %lf antigo Y1 da linha: %lf\n", st->x1, st->y1);
    st->x1 = (x1Antigo + dx);
    st->y1 = (y1Antigo + dy);
    st->x2 = (x2Antigo + dx);
    st->y2 = (y2Antigo + dy);
    //printf("\nnovo X1 do linha: %lf novo Y1 do linha: %lf\n", st->x1, st->y1);
}