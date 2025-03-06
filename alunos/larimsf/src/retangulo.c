#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "retangulo.h"

typedef struct 
{
    double x, y;       // canto inferior esquerdo
    double larg, alt;  // largura e altura
    int i;          // identificador do retângulo 
    char corb[20];  //cor da borda 
    char corp[20];  //cor do preenchimento 
    double angulo;  //ângulo inicial do retângulo  
    bool colheitadeira; //afirma de o retângulo é ou não uma colheitadeira 
} StRetangulo;



Retangulo criaRetangulo(double x, double y, double larg, double alt, int i, char *corb, char *corp)
{
    StRetangulo *rect = (StRetangulo *) malloc(sizeof(StRetangulo));

    if (rect == NULL) 
    {
        printf("Erro: nao foi possivel alocar memoria.\n");
        exit(1);
    }
    rect->x = x;
    rect->y = y;
    rect->larg = larg;
    rect->alt = alt;
    rect->i = i;
    rect->colheitadeira = false; 
    //printf("identificador: %d\n", rect->i); 
    strcpy(rect->corb, corb);
    strcpy(rect->corp, corp);

    return rect;
}

double getAltura(Retangulo rect) 
{
    StRetangulo *st = (StRetangulo *)rect;
    return st->alt;
}

double getLargura(Retangulo rect) 
{
    StRetangulo *st = (StRetangulo *)rect;
    return st->larg;
}


char *getCorbRect(Retangulo rect)  
{
    StRetangulo *st = (StRetangulo *)rect;
    return st->corb; 
}

char *getCorpRect(Retangulo rect) 
{
    StRetangulo *st = (StRetangulo *)rect;
    return st->corp; 
}

double getPosicXRect(Retangulo rect)
{
    StRetangulo *st = (StRetangulo *)rect;
    return st->x; 
}

double getPosicYRect(Retangulo rect)
{
    StRetangulo *st = (StRetangulo *)rect;
    return st->y; 
}


void moveRetangulo(Retangulo rect, double xAntigo, double yAntigo, double dx, double dy)
{
    StRetangulo *st = rect;
    //printf("\nantigo X do retangulo: %lf antigo Y do retangulo: %lf\n", st->x, st->y);
    st->x = (xAntigo + dx);
    st->y = (yAntigo + dy);
    //printf("\nnovo X do retangulo: %lf novo Y do retangulo: %lf\n", st->x, st->y);
}

void selecionaColheitadeira(Retangulo rect)
{
    StRetangulo *st = rect;
    st->colheitadeira = true;
}

bool verificaColheitadeira(Retangulo rect)
{
    StRetangulo *st = rect;
    if(st->colheitadeira == true)
    {
        //printf("É colheitadeira!\n");
        return true;  
    }else 
    {
        return false; 
    }
}

void moveColheitadeira(Retangulo rect, char *direcao)
{
    StRetangulo *st = rect;
    double dx = 0.0;
    double dy = 0.0;

    switch (*direcao)
    {
        case 'n':
            dy = st->alt;  // Deslocamento para o norte (baixo)
            break;
        case 's':
            dy = -st->alt;  // Deslocamento para o sul (cima)
            break;
        case 'l':
            dx = -st->larg;  // Deslocamento para o leste (esquerda)
            break;
        case 'o':
            dx = st->larg;  // Deslocamento para o oeste (direita)
            break;
        default:
            printf("Direção inválida!\n");
            return;
    } 
 
    st->x += dx;
    st->y += dy;
    
    //printf("Novo X da colheitadeira: %lf\nNovo Y do retângulo: %lf\n", st->x, st->y);
}


int getIdentificadorRetangulo(Retangulo rect)
{
    StRetangulo *st = (StRetangulo *)rect;
    return st->i; 
}


void liberaRetangulo(Retangulo *rect)
{
    free(rect);
}











