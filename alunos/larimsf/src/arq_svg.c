#include "arq_svg.h" 
#include <stdio.h> 
#include <string.h>
#include "lista.h"
#include "retangulo.h"
#include "circulo.h"
#include "texto.h"


double Normalizar(double value)
{
  if (value < 0)
    value = 1;
  
  return value; 
}
 


ArqSvg abreEscritaSvg(char *fn) 
{
  FILE *file = fopen(fn, "w");
  if(file)
  {
    fprintf(file, "<svg xmlns=\"http://www.w3.org/2000/svg\">\n");
  }
  return file;
}


void preparaDecoracao(ArqSvg fsvg, char *deco, int decoLen,
		      char *corBorda, char *corPreenchimento,
		      char *larguraBorda, double transparencia,
		      double transparenciaPreenchimento, double transparenciaBorda)
{
    transparencia = Normalizar(transparencia);
    transparenciaPreenchimento = Normalizar(transparenciaPreenchimento);
    transparenciaBorda = Normalizar(transparenciaBorda); 
    
    sprintf(deco,
            "style = \"stroke:%s;fill:%s;stroke-width:%s;opacity:%lf;fill-opacity:%lf;stroke-opacity%lf\"",
            corBorda, corPreenchimento, larguraBorda, transparencia, transparenciaPreenchimento, transparenciaBorda);
}


void escreveCirculoSvg(Circulo *c, ArqSvg fsvg)
{

  if (fsvg != NULL)
    {
      fprintf(fsvg, "\n\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"%s\" stroke=\"%s\"/>", getPosicXCircle(c), getPosicYCircle(c), getRaio(c), getCorpCircle(c), getCorbCircle(c));
    }
}


void escreveRetanguloSvg(Retangulo *r, ArqSvg fsvg)
{
    if (fsvg != NULL)
    {
        fprintf(fsvg, "\n\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" style=\"fill:%s;stroke:%s\"/>", getPosicXRect(r), getPosicYRect(r), getLargura(r), getAltura(r), getCorpRect(r), getCorbRect(r));
    }
}




void escreveLinhaSvg(Linha *l, ArqSvg fsvg)
{
  if (fsvg != NULL)
    {
        fprintf(fsvg, "\n\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"stroke:%s\"/>", getPosicX1Line(l), getPosicY1Line(l), getPosicX2Line(l), getPosicY2Line(l), getCorLine(l));
    }
}



void escreveTextoSvg(Texto t, ArqSvg fsvg)
{
    if (fsvg != NULL)
    {
      char string[5], string2[5], string3[5];
      strcpy(string, getAncoraText(t));
      strcpy(string2, getFontFamily(t));
      //printf("String2 dentro da função escreve texto: %s\n", string2);
      strcpy(string3, getFontWeigth(t)); 
      
      //printf("string: %s\n", string);
      char ancor[10];  
      char fontFamily[12];
      char fontWeigth[10]; 
      
      //Definindo a ÂNCORA DO TEXTO
      if (strcmp(string, "i") == 0) 
      {
        strcpy(ancor, "start");
        //printf("ancor: %s", ancor);
      }
      else if (strcmp(string, "m") == 0) 
      {
          strcpy(ancor, "middle");
          //printf("ancor: %s", ancor);
      }
      else if (strcmp(string, "f") == 0) 
      {
          strcpy(ancor, "end");
          //printf("ancor: %s", ancor);
      }
      else 
      {
          printf("Você digitou uma palavra desconhecida para a Âncora. Você digitou: %s\n", string);
      }

      //Definindo a FONTFAMILY DO TEXTO
      if (strcmp(string2, "sans") == 0) 
      {
        strcpy(fontFamily, "sans-serif");
      }
      else if (strcmp(string2, "serif") == 0) 
      {
          strcpy(fontFamily, "serif");
      }
      else if (strcmp(string2, "cursive") == 0) 
      {
          strcpy(fontFamily, string2);
      }else if (strcmp(string2, "cursin") == 0) 
      {
          strcpy(fontFamily, "cursive");
      }else 
      {
        strcpy(fontFamily, string2);
      }


      //Definindo a FONTWEIGTH DO TEXTO
      if (strcmp(string3, "n") == 0) 
      {
        strcpy(fontWeigth, "normal");
      }
      else if (strcmp(string3, "b") == 0) 
      {
          strcpy(fontWeigth, "bold");
      }
      else if (strcmp(string3, "b+") == 0) 
      {
          strcpy(fontWeigth, "bolder");
      }
      else if (strcmp(string3, "l") == 0) 
      {
          strcpy(fontWeigth, "lighter");
      }else 
      {
          strcpy(fontWeigth, string3);
      }

          fprintf(fsvg, "\n\t<text anchor=\"%s\" x=\"%lf\" y=\"%lf\" fill=\"%s\" stroke=\"%s\" style=\"font-size:%d; font-family:%s; font-weight:%s\" transform=\"rotate(%lf,%lf,%lf)\">%s</text>", ancor, getPosicXText(t), getPosicYText(t), getCorpText(t), getCorbText(t), getSizeText(t), fontFamily, fontWeigth, getAnguloText(t), getPosicXText(t), getPosicYText(t), getTextoText(t));
      }
}



void preparaDecoracaoTexto(ArqSvg fsvg, char *deco, int decoLen,
			   char *fontFamily, char *fontStyle,
			   char *fontWeight, char *fontSize,
			   char *fontColor, char *textAnchor)
{
    sprintf(deco,
            "style = \"font-family:%s;font-style:%s;font-weight:%s;font-size:%s;fill:%s;text-anchor%s\"",
            fontFamily, fontStyle, fontWeight, fontSize, fontColor, textAnchor);
}



ArqSvg apagaConteudoSvg(char* arq)
{
    FILE *arquivo;
    arquivo = fopen(arq, "w"); 
    return arquivo; 
}


void fechaSvg(ArqSvg fsvg)
{
    FILE* ponteiro = fsvg;
    fprintf(ponteiro, "\n\n</svg>");
    fclose(ponteiro);
}