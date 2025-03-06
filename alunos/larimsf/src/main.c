#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>
#include <string.h>
#include "math.h"
#include "lista.h"
#include "radialtree.h"
#include "circulo.h"
#include "linha.h"
#include "retangulo.h"
#include "arq_svg.h"
#include "texto.h"
#include "forma.h"


/*
 * Estrutura de dados que representa informações sobre hortaliças.
 * Contém campos como tipo, peso, identificador, níveis de praga, adubo e defensivo.
 */
typedef struct 
{
    char *tipo;         // Tipo da hortaliça
    double peso;        // Peso da hortaliça
    int id;             // Identificador da hortaliça
    double praga;       // Nível de praga da hortaliça
    double adubo;       // Nível de adubo da hortaliça
    double defensivo;   // Nível de defensivo da hortaliça
} Hortalicas;



//Protótipos 
void le_comando(int argc, char *argv[], char **entrada, char **saida, char **arquivoGeo, 
                 char **arquivoQry, char **sufixo_completo, char **caminho_completoGeo, 
                 char **caminho_completoQry, int *ns, double *p);

char* strdup(const char* s);
void abrir_arquivo(char* path, FILE** file); 
void desenharFotosSvg(Lista* formas, ArqSvg desenho);
void visitaNoRadial2(int id, double x, double y, void* aux);
void visitaNoRadial(Info i, double x, double y, void* aux);
double obterPesoPorId(Hortalicas* hortalicas, int tamanho, int id);
void acumularPraga(Hortalicas* hortalicas, double valorAcumulado);
void acumularCura(Hortalicas* hortalicas, double valorAcumulado);
void acumularAdubo(Hortalicas* hortalicas, double valorAcumulado);
double calcularMassaFinal(Hortalicas* hortalica);





void processaGeo(FILE *geo_file, Lista circulosDaListaBD, Lista retangulosDaListaBD, 
                Lista linhasDaListaBD, Lista textosDaListaBD, Lista listaBD, 
                char *FontFamily, Retangulo* vetorRetangulos[], Node* vetorNos[],
                char *FontWeigth, double size, RadialTree arvore_radial, Forma* vetorFormas[],
                Posic* vetorPosicoes[], Hortalicas vetorHortalicas[], double vetorX[], double vetorY[])
{
    int num; 
    char line[150];
    int id, q=1;
    double x1, y1, x2, y2, dx, raio, largura, altura, profundidade, graus, distancia;
    char corb[10], corp[10], ancora[1], txt[500], sufixo[20], capacidade[5];
    

    while (fgets(line, 150, geo_file) != NULL) 
    {
        if (line[0] == 'c') 
        {
            Circulo c;
            Hortalicas abobora; 
            sscanf(line, "c %d %lf %lf %lf %s %s", &id, &x1, &y1, &raio, corb, corp);
            //printf("Id: %d X1: %lf Y1: %lf Raio: %lf Corb: %s Corp: %s\n", id, x1, y1, raio, corb, corp);
            
            //Criando o circulo e adicionando a lista Banco de Dados 
            c = criaCirculo(x1,y1,raio,id,corb,corp);
            Forma f = guarda_formato(c, "c");  
            vetorPosicoes[id] = insertLst(listaBD, f); 
            vetorNos[id] = insertRadialT(arvore_radial, x1, y1, c, id);
            vetorFormas[id] = f; 
            vetorHortalicas[id] = abobora; 
            vetorX[id] = x1;
            vetorY[id] = y1;
            abobora.id = id;
            abobora.peso = 2000;
            abobora.praga = 0; 
            abobora.defensivo = 0;
            abobora.adubo = 0; 
        }else if (line[0] == 'r') 
        {
            Retangulo r;
            Hortalicas repolho; 
            sscanf(line, "r %d %lf %lf %lf %lf %s %s", &id, &x1, &y1, &largura, &altura, corb, corp);
            //printf("RETANGULO: %d %lf %lf %lf %lf %s %s\n", id, x1, y1, largura, altura, corb, corp);
            
            //Criando o retangulo e adicionando na lista Banco de Dados 
            r = criaRetangulo(x1,y1,largura,altura,id,corb,corp);
            vetorRetangulos[id] = r;
            Forma f = guarda_formato(r, "r"); 
            vetorPosicoes[id] = insertLst(listaBD, f);
            vetorNos[id] = insertRadialT(arvore_radial, x1, y1, r, id);
            vetorFormas[id] = f;
            vetorHortalicas[id] = repolho; 
            vetorX[id] = x1;
            vetorY[id] = y1;
            repolho.id = id;
            repolho.peso = 1000;
            repolho.praga = 0; 
            repolho.defensivo = 0;
            repolho.adubo = 0; 
        }else if (line[0] == 'l') 
        {
            Linha l;
            Hortalicas mato; 
            sscanf(line, "l %d %lf %lf %lf %lf %s", &id, &x1, &y1, &x2, &y2, corp);
            //printf("********Antes Id: %d %lf %lf %lf %lf %s\n", id, x1, y1, x2, y2, corp);

            //Criando a linha e adicionando na lista Banco de Dados 
            l = criaLinha(x1,y1,x2,y2,id,corp);             
            Forma f = guarda_formato(l, "l");
            vetorPosicoes[id] = insertLst(listaBD, f);
            vetorNos[id] = insertRadialT(arvore_radial, x1, y1, l, id);
            vetorFormas[id] = f;
            vetorHortalicas[id] = mato; 
            vetorX[id] = x1;
            vetorY[id] = y1;
            mato.id = id;
            mato.peso = (10*(x2-x1));
            mato.praga = 0; 
            mato.defensivo = 0;
            mato.adubo = 0; 
        } else if ((line[0] == 't') && (line[1] == 's'))
        {
            sscanf(line, "ts %s %s %lf", FontFamily, FontWeigth, &size);
            //printf("Estilo dos TEXTOS: %s %s %lf\n", FontFamily, FontWeigth, size);

        }else if (line[0] == 't' && (line[1] == ' ')) 
        {
            
            Texto t;
            sscanf(line, "t %d %lf %lf %s %s %s %s", &id, &x1, &y1, corb, corp, ancora, txt);
            //printf("Id: %d X: %lf Y: %lf Corb: %s Corp: %s Ancora: %s Texto: %s \n", id, x1, y1, corb, corp, ancora, txt);
            
            //Criando o texto e adicionando na lista Banco de Dados 
            t = criaTexto(x1,y1,id,ancora,txt,corb,corp,size,FontFamily,FontWeigth);
            Forma f = guarda_formato(t, "t");
            vetorPosicoes[id] = insertLst(listaBD, f);
            vetorNos[id] = insertRadialT(arvore_radial, x1, y1, t, id);
            //printf("Vetor de nós: %p id: %d item do nó: %p\n", vetorNos[id], id, getInfoRadialT(arvore_radial, vetorNos[id]));
            vetorFormas[id] = f;
            vetorX[id] = x1;
            vetorY[id] = y1;
            char texto[5];
            strcpy(texto, getTextoText(t));
            if(strcmp(texto, "@") == 0)
            {
                Hortalicas cebola; 
                vetorHortalicas[id] = cebola; 
                cebola.id = id;
                cebola.peso = 200;
                cebola.praga = 0; 
                cebola.defensivo = 0;
                cebola.adubo = 0; 
            }else if(strcmp(texto, "*") == 0)
            {
                Hortalicas morango; 
                vetorHortalicas[id] = morango; 
                morango.id = id;
                morango.peso = 20;
                morango.praga = 0; 
                morango.defensivo = 0;
                morango.adubo = 0;
            }else if(strcmp(texto, "%") == 0)
            {
                Hortalicas cenoura; 
                vetorHortalicas[id] = cenoura; 
                cenoura.id = id;
                cenoura.peso = 70;
                cenoura.praga = 0; 
                cenoura.defensivo = 0;
                cenoura.adubo = 0;
            }else
            {
                Hortalicas matoT; 
                vetorHortalicas[id] = matoT; 
                matoT.id = id;
                matoT.peso = 15;
                matoT.praga = 0; 
                matoT.defensivo = 0;
                matoT.adubo = 0;
            }
        }
    }
}



/*
 * Estrutura de dados que representa uma região para consulta.
 * Contém diversos elementos relacionados à região, bem como ponteiros
 * e contadores para auxiliar no processamento da consulta.
 */
typedef struct {
    Lista* lista;               // Ponteiro para uma lista
    Lista* lista2;              // Ponteiro para outra lista
    double regiaoX;             // Coordenada x da região retangular
    double regiaoY;             // Coordenada y da região retangular
    double regiaoLargura;       // Largura da região retangular
    double regiaoAltura;        // Altura da região retangular
    Info* vetor;                // Vetor para armazenar informações de nós dentro da região
    int id;                     // Contador para o índice no vetor de informações
    int totalNos;               // Total de nós na estrutura
    FILE *arquivo;              // Ponteiro para um arquivo
    RadialTree arvore;          // Árvore radial
    double* vetorX;             // Vetor de coordenadas x
    double *vetorY;             // Vetor de coordenadas y
    Node** vetorNos;            // Vetor de ponteiros para nós
    int totalItens;             // Total de itens na estrutura
    int* vetorDeIds;            // Vetor para armazenar os identificadores dos nós dentro da região
} RegiaoLista;



/*
 * Função de visita a um nó em uma árvore radial.
 * Verifica se as coordenadas (x, y) do nó estão dentro da região retangular
 * especificada em `aux`, e guarda o identificador do nó na lista de região.
 *
 * @param id O identificador associado ao nó visitado.
 * @param x A coordenada x do nó visitado.
 * @param y A coordenada y do nó visitado.
 * @param aux O ponteiro para a estrutura de dados auxiliar (RegiaoLista).
 */
void visitaNoRadial2(int id, double x, double y, void* aux) 
{
    RegiaoLista* regiaoLista = (RegiaoLista*)aux;
    double regiaoX = regiaoLista->regiaoX;
    double regiaoY = regiaoLista->regiaoY;
    double regiaoLargura = regiaoLista->regiaoLargura;
    double regiaoAltura = regiaoLista->regiaoAltura; 
    int idDoNo; 
    
    // Verifica se o nó está dentro da região retangular e guarda seu id
    if ((x >= regiaoX) && (x <= regiaoX + regiaoLargura) &&
    (y >= regiaoY) && (y <= regiaoY + regiaoAltura)) 
    {
        //printf("id da figura que está dentro da região: %d\n", id);
        regiaoLista->vetorDeIds[regiaoLista->id] = id; 
        regiaoLista->id += 1;
    }
}



void processaQry(FILE *qry_file, Lista circulosDaListaBD, Lista retangulosDaListaBD, 
            Lista linhasDaListaBD, Lista textosDaListaBD, Lista listaBD,  FILE *arquivoTXT, 
            char *saida, char *sufixo_completo, char *FontFamily, char *FontWeigth, 
            double size, Lista formasAux, RadialTree arvore_radial, Node* vetorNos[], 
            Forma* vetorFormas[], Posic* vetorPosicoes[], Hortalicas vetorHortalicas[])
{

    //Criando variáveis que vão me auxiliar no programa 
    char line[100];
    int totalDeItens = lengthLst(listaBD);    

    //Chamando as funções do .qry 
    if (qry_file != NULL)
    { 
        while (fgets(line, 100, qry_file) != NULL) 
        {     
            
            //-----> Informa que o retângulo de código i é uma colheitadeira.
            if ((line[0] == 'c') && (line[1] == 'l')) 
            {
                int id; 
                sscanf(line, "cl %d", &id);
                Info info = getInfoRadialT(arvore_radial, vetorNos[id]); 
                Retangulo rect = (Retangulo)info; 
                selecionaColheitadeira(rect);
                verificaColheitadeira(rect);
            }


            //----> COLHEITA 
            if ((line[0] == 'h') && (line[1] == 'v') && (line[2] == 't'))
            {
                fprintf(arquivoTXT, "Colheita:\n");
                int passos, k=0, id;
                char direcao[10];
                sscanf(line, "hvt %d %d %s", &id, &passos, direcao);
                Retangulo r1 = getObjeto(vetorFormas[id]);
                double contab = 0; 
                
                if(verificaColheitadeira(r1) == true)
                {
                    while(k < passos)
                    {
                        //movendo 
                        Info info = getInfoRadialT(arvore_radial, vetorNos[id]); 
                        Retangulo rect = (Retangulo)info; 
                        
                        //colhendo
                        Info vetorDeItens[totalDeItens]; 
                        int vetorDeIdentificadores[totalDeItens]; 
                        double x = getPosicXRect(rect); 
                        double y = getPosicYRect(rect);
                        //printf("X: %lf Y: %lf altura: %lf largura: %lf do retangulo\n", x, y, getLargura(rect), getAltura(rect));

                        //Desenhando o traço da área no SVG 
                        Retangulo area = criaRetangulo(x, y, getLargura(rect), getAltura(rect), 100, "red", "none"); 
                        Forma areaPontilhada = guarda_formato(area, "r"); 
                        insertLst(listaBD, areaPontilhada);
                        Circulo gota = criaCirculo(x, y, 2, 301, "red", "red"); 
                        Forma gotaPontilhada = guarda_formato(gota, "c");
                        insertLst(listaBD, gotaPontilhada);
                        Circulo gota2 = criaCirculo(x, y, 5, 302, "red", "none"); 
                        Forma gotaPontilhada2 = guarda_formato(gota2, "c");
                        insertLst(listaBD, gotaPontilhada2);

                        // Criando a estrutura RegiaoLista e preenchendo os valores
                        RegiaoLista regiaoLista;
                        regiaoLista.vetor = vetorDeItens; 
                        regiaoLista.regiaoX = x;
                        regiaoLista.regiaoY = y;
                        regiaoLista.regiaoLargura = getLargura(rect);
                        regiaoLista.regiaoAltura = getAltura(rect);
                        regiaoLista.id = 0; 
                        regiaoLista.lista2 = listaBD;
                        regiaoLista.arvore = arvore_radial;
                        regiaoLista.vetorDeIds = vetorDeIdentificadores;

                        //printf("Área de colheita x: %lf y: %lf largura: %lf altura: %lf\n", x, y, getLargura(rect), getAltura(rect));
                        
                        //Buscando na árvore os nós que estão dentro da região e pegando seus id's
                        char formato[5];
                        procuraNoRadialT(arvore_radial, visitaNoRadial2, &regiaoLista); 

                        //Reconhecendo a forma e colhendo ela
                        for(int e = 0; e<regiaoLista.id; e++)
                        {
                            int ident = vetorDeIdentificadores[e]; 
                            Node n2 = vetorNos[ident];
                            if(n2 != NULL)
                            {
                                strcpy(formato, getFormato(vetorFormas[ident]));
                                if (strcmp(formato, "r") == 0) 
                                {
                                    Info infoR = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                                    Retangulo r = (Retangulo)infoR;
                                    if(verificaColheitadeira(r) == false)
                                    {        
                                        bool retanguloCompletamenteDentro = (getPosicXRect(r) >= x && getPosicYRect(r) >= y &&
                                                                            (getPosicXRect(r)+getLargura(r)) <= x+getLargura(rect) &&
                                                                            getPosicYRect(r)+getAltura(r) <= y+getAltura(rect)); 
                                        if(retanguloCompletamenteDentro && ((isNodeRemoved(listaBD, vetorPosicoes[ident]))==false))
                                        {
                                            removeNoRadialT(arvore_radial, n2);
                                            fprintf(arquivoTXT, "        Repolho de id %d colhido\n", ident);
                                            removeLst(listaBD, vetorPosicoes[ident]);
                                             //contabilidade 
                                            Hortalicas* repolho = &vetorHortalicas[ident]; 
                                            double pesoFinal = calcularMassaFinal(repolho); 
                                            if(verificaColheitadeira(r) == false)
                                                fprintf(arquivoTXT, "        Contabilidade Parcial: Repolho de id %d: %lf gramas\n", ident, pesoFinal);
                                        }
                                    }
                                }else if(strcmp(formato, "c") == 0)
                                {
                                    Info infoC = getInfoRadialT(arvore_radial, vetorNos[ident]);
                                    Circulo circle = (Circulo)infoC;
                                    int centroX = getPosicXCircle(circle);
                                    int centroY = getPosicYCircle(circle);
                                    int raio = getRaio(circle);
                                    bool circuloCompletamenteDentro = (centroX >= x + raio &&
                                                                       centroX <= x + getLargura(rect) - raio &&
                                                                       centroY >= y + raio &&
                                                                       centroY <= y + getAltura(rect) - raio);
                                    if (circuloCompletamenteDentro && ((isNodeRemoved(listaBD, vetorPosicoes[ident]))==false)) 
                                    {
                                        removeNoRadialT(arvore_radial, n2);
                                        fprintf(arquivoTXT, "        Abóbora de id %d colhida\n", ident);
                                        removeLst(listaBD, vetorPosicoes[ident]);
                                        //contabilidade 
                                        Hortalicas* abobora = &vetorHortalicas[ident]; 
                                        double pesoFinal = calcularMassaFinal(abobora); 
                                        fprintf(arquivoTXT, "        Contabilidade Parcial: Abobora de id %d: %lf gramas\n", ident, pesoFinal);
                                    }
                                }else if(strcmp(formato, "t") == 0)
                                {
                                    
                                    Info infoT = getInfoRadialT(arvore_radial, vetorNos[ident]);
                                    Texto t = (Texto)infoT; 
                                    char texto[5];
                                    strcpy(texto, getTextoText(t));
                                    Node noT = vetorNos[ident];
                                    Posic posicao = vetorPosicoes[ident];
                                    //printf("Valor de m: %d Posição do texto m: %p\n", m, vetorPosicoes[m]);
                                    
                                    if((isNodeRemoved(listaBD, vetorPosicoes[ident]))==false)
                                    {
                                        if(strcmp(texto, "@") == 0)
                                        {
                                            fprintf(arquivoTXT, "        Cebola de id %d colhida\n", ident);
                                            //contabilidade 
                                            Hortalicas* cebola = &vetorHortalicas[ident]; 
                                            double pesoFinal = calcularMassaFinal(cebola); 
                                            fprintf(arquivoTXT, "        Contabilidade Parcial: Cebola de id %d: %lf gramas\n", ident, pesoFinal);
                                        }else if(strcmp(texto, "%") == 0)
                                        {
                                            fprintf(arquivoTXT, "        Cenoura de id %d colhida\n", ident);
                                            //contabilidade 
                                            Hortalicas* cenoura = &vetorHortalicas[ident]; 
                                            double pesoFinal = calcularMassaFinal(cenoura);
                                            fprintf(arquivoTXT, "        Contabilidade Parcial: Cenoura de id %d: %lf gramas\n", ident, pesoFinal);
                                        }else if(strcmp(texto, "*") == 0)
                                        {
                                            fprintf(arquivoTXT, "        Morango de id %d colhido\n", ident);
                                            //contabilidade 
                                            Hortalicas* morango = &vetorHortalicas[ident]; 
                                            double pesoFinal = calcularMassaFinal(morango); 
                                            fprintf(arquivoTXT, "        Contabilidade Parcial: Morango de id %d: %lf gramas\n", ident, pesoFinal);
                                        }

                                        removeNoRadialT(arvore_radial, noT);
                                        removeLst(listaBD, posicao);
                                    }
                                }else
                                {
                                    if((isNodeRemoved(listaBD, vetorPosicoes[ident]))==false)
                                    {
                                        removeNoRadialT(arvore_radial, n2);
                                        removeLst(listaBD, vetorPosicoes[ident]);
                                        fprintf(arquivoTXT, "        Mato de id %d colhido\n", ident);
                                        //contabilidade 
                                        Hortalicas* mato = &vetorHortalicas[ident]; 
                                        double pesoFinal = calcularMassaFinal(mato); 
                                        fprintf(arquivoTXT, "        Contabilidade Parcial: Mato de id %d: %lf gramas\n", ident, pesoFinal);
                                    }
                                }
                            }
                            
                        }

                        if(k < passos-1)
                            moveColheitadeira(rect, direcao); 
                        fprintf(arquivoTXT, "Colheitadeira de id %d e posição inicial %lf,%lf moveu %d passos para a direção %s\n", id, getPosicXRect(rect), getPosicYRect(rect), passos, direcao);
                        k++;
                    }
                }else
                {
                    printf("Colheitadeira de id %d não identificada.\n", id);
                }
            }



            //-----> MOVE FIGURAS 
            if ((line[0] == 'm') && (line[1] == 'v'))
            {
                int id; 
                double dx, dy;
                sscanf(line, "mv %d %lf %lf", &id, &dx, &dy);

                Forma f = vetorFormas[id];
                char formato[5]; 
                strcpy(formato, getFormato(vetorFormas[id]));
                Node no = vetorNos[id];

                if (strcmp(formato, "r") == 0) 
                {
                    Info infoR = getInfoRadialT(arvore_radial, vetorNos[id]); 
                    Retangulo r = (Retangulo)infoR;   
                    double xRect = getPosicXRect(r);
                    double yRect = getPosicYRect(r);
                    moveRetangulo(r, xRect, yRect, dx, dy);
                    fprintf(arquivoTXT, "Moveu o Retângulo de identificador: %d Posição original: %lf,%lf Posição final: %lf,%lf\n", id, xRect, yRect, (xRect+dx), (yRect+dy));
                }else if (strcmp(formato, "c") == 0)
                {
                    Info infoC = getInfoRadialT(arvore_radial, vetorNos[id]); 
                    Circulo c = (Circulo)infoC;   
                    double xCirc = getPosicXCircle(c);
                    double yCirc = getPosicYCircle(c);
                    moveCirculo(c, xCirc, yCirc, dx, dy);
                    fprintf(arquivoTXT, "Moveu o Circulo de identificador: %d Posição original: %lf,%lf Posição final: %lf,%lf\n", id, xCirc, yCirc, (dx+xCirc), (dy+yCirc));    
                }else if (strcmp(formato, "l") == 0)
                {
                    Info infoL = getInfoRadialT(arvore_radial, vetorNos[id]); 
                    Linha l = (Linha)infoL;
                    double x1Line = getPosicX1Line(l);
                    double y1Line = getPosicY1Line(l);
                    double x2Line = getPosicX2Line(l);
                    double y2Line = getPosicY2Line(l);
                    moveLinha(l, x1Line, y1Line, x2Line, y2Line, dx, dy);
                    fprintf(arquivoTXT, "Moveu a Linha de identificador: %d Posição original da primeira ancora: %lf,%lf Posição final da primeira ancora: %lf,%lf\n", id, x1Line, y1Line, (x1Line+dx), (y1Line+dy));
                }else if (strcmp(formato, "t") == 0)
                {
                    Info infoT = getInfoRadialT(arvore_radial, vetorNos[id]); 
                    Texto t = (Texto)infoT;
                    double xTexto = getPosicXText(t);
                    double yTexto = getPosicYText(t);
                    moveTexto(t, xTexto, yTexto, dx, dy);
                    fprintf(arquivoTXT, "Moveu o Texto de identificador: %d Posição original: %lf,%lf Posição final: %lf,%lf\n", id, xTexto, yTexto, (xTexto+dx), (yTexto+dy));
                }
            }


            //-----> PRAGAS 
            if ((line[0] == 'c') && (line[1] == 't'))
            {
                fprintf(arquivoTXT, "Ataque de pragas:\n");
                int g = 0;
                double x, y, w, h, raio;
                sscanf(line, "ct %lf %lf %lf %lf %lf", &x, &y, &w, &h, &raio);
                //printf("Raio da gota da praga: %lf\n", raio);
                Info vetorDeItens[totalDeItens];
                Circulo vetorDeGotas[totalDeItens]; 
                int vetorDeIdentificadores[totalDeItens];
                double areaGota = 3.14159 * raio * raio;
                

                //Desenhando o traço da área no SVG 
                Retangulo area = criaRetangulo(x, y, w, h, 200, "red", "none"); 
                Forma areaPontilhada = guarda_formato(area, "r"); 
                insertLst(listaBD, areaPontilhada);
                Circulo gota = criaCirculo(x, y, raio, 300, "red", "none"); 
                Forma gotaPontilhada = guarda_formato(gota, "c");
                insertLst(listaBD, gotaPontilhada);

                //Distribuindo as gotas uniformemente 
                double dist = raio * 2; // Distância entre os círculos é o dobro do raio
                
                //Espalhando as gotas pela área
                for (double posX = x; posX < x + w; posX += dist) 
                {
                  for (double posY = y; posY < y + h; posY += dist) 
                  {
                    Circulo gotas = criaCirculo(posX, posY, raio, 300, "red", "none");
                    Forma gotasPontilhadas = guarda_formato(gotas, "c");
                    //insertLst(listaBD, gotasPontilhadas);
                    vetorDeGotas[g] = gotas;
                  }
                  g++;
                }

                // Criando a estrutura RegiaoLista e preenchendo os valores
                RegiaoLista regiaoLista;
                regiaoLista.vetor = vetorDeItens; 
                regiaoLista.regiaoX = x;
                regiaoLista.regiaoY = y;
                regiaoLista.regiaoLargura = w;
                regiaoLista.regiaoAltura = h;
                regiaoLista.id = 0; 
                regiaoLista.vetorDeIds = vetorDeIdentificadores;
                
                //Buscando na árvore os nós que estão dentro da região e pegando seus id's
                char formato[5];
                procuraNoRadialT(arvore_radial, visitaNoRadial2, &regiaoLista); 
                
                //Reconhecendo a forma e jogando praga nela caso a gota esteja dentro 
                for(int e = 0; e<regiaoLista.id; e++)
                {
                    int ident = vetorDeIdentificadores[e]; 
                    Node n2 = vetorNos[ident];
                    if(n2 != NULL)
                    {
                        strcpy(formato, getFormato(vetorFormas[ident]));
                        if (strcmp(formato, "r") == 0) 
                        {
                            Info infoR = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                            Retangulo r = (Retangulo)infoR;
                            Hortalicas* repolho = &vetorHortalicas[ident]; 
                            double areaRet = getAltura(r)*getLargura(r);
                            double pragaAtual = 0; 
                            double proporcao = areaGota/areaRet;
                            for(int p=0; p<g; p++)
                            {
                                Circulo gotinhas = vetorDeGotas[p]; 
                                
                                if (areaGota <= areaRet &&
                                getPosicXCircle(gotinhas) - raio >= getPosicXRect(r) &&
                                getPosicXCircle(gotinhas) + raio <= getPosicXRect(r) + getLargura(r) &&
                                getPosicYCircle(gotinhas) - raio >= getPosicYRect(r) &&
                                getPosicYCircle(gotinhas) + raio <= getPosicYRect(r) + getAltura(r)) 
                                { 
                                    pragaAtual += proporcao;
                                    //printf("Praga atual do repolho dentro do for: %lf\n", pragaAtual);
                                }
                            }
                            acumularPraga(repolho, pragaAtual);
                            if(repolho->praga > 0)    
                                fprintf(arquivoTXT, "        Repolho de id %d atingido por praga. Grau de dano: %lf\n", ident, repolho->praga);
                            //printf("Proporção total repolho: %lf\n\n", repolho->praga);
                            if (pragaAtual >= 0.75 * areaRet) 
                            {
                                //Pelo menos 75 por cento da área da hortaliça foi atingida
                                removeNoRadialT(arvore_radial, n2);
                                fprintf(arquivoTXT, "        Repolho de id %d teve pelo menos 75 por cento da sua área atingida e foi removido\n", ident);
                                removeLst(listaBD, vetorPosicoes[ident]);
                                //Desenhando o X no SVG 
                                Texto X = criaTexto(getPosicXRect(r), getPosicYRect(r), 206, "m", "X", "red", "red", 25, FontFamily, FontWeigth); 
                                Forma XPontilhado = guarda_formato(X, "t"); 
                                insertLst(listaBD, XPontilhado); 
                            }
                        }else if (strcmp(formato, "c") == 0)
                        {
                            Info infoC = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                            Circulo c = (Circulo)infoC;
                            Hortalicas* abobora = &vetorHortalicas[ident];
                            double areaCirc = 3.14159 * getRaio(c) * getRaio(c);
                            double proporcao = areaGota/areaCirc;
                            double pragaAtual = 0;

                            for(int p=0; p<g; p++)
                            {
                                Circulo gotinhas = vetorDeGotas[p]; 
                                if (areaGota <= areaCirc &&
                                    sqrt(pow(getPosicXCircle(gotinhas) - getPosicXCircle(c), 2) + pow(getPosicYCircle(gotinhas) - getPosicYCircle(c), 2)) + raio <= getRaio(c)) 
                                {
                                    pragaAtual += proporcao;
                                    //printf("Praga atual do repolho dentro do for: %lf\n", pragaAtual);
                                }
                            }
                            acumularPraga(abobora, pragaAtual);
                            if(abobora->praga > 0)
                                fprintf(arquivoTXT, "        Abobora de id %d atingida por praga. Grau de dano: %lf\n", ident, abobora->praga);
                            
                            if (pragaAtual >= 0.75 * areaCirc) 
                            {
                                //Pelo menos 75 por cento da área da hortaliça foi atingida
                                removeNoRadialT(arvore_radial, n2);
                                fprintf(arquivoTXT, "        Abobora de id %d teve pelo menos 75 por cento da sua área atingida e foi removida\n", ident);
                                removeLst(listaBD, vetorPosicoes[ident]);
                                //Desenhando o X no SVG 
                                Texto X = criaTexto(getPosicXCircle(c), getPosicYCircle(c), 207, "m", "X", "red", "red", 25, FontFamily, FontWeigth); 
                                Forma XPontilhado = guarda_formato(X, "t"); 
                                insertLst(listaBD, XPontilhado); 
                            }
                        }else if (strcmp(formato, "t") == 0)
                        {
                            Info infoT = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                            Texto t = (Texto)infoT;
                            char texto[5];
                            strcpy(texto, getTextoText(t));
                            double proporcao = 0.1;
                            double pragaAtual = 0;

                            for(int p=0; p<g; p++)
                            {
                                Circulo gotinhas = vetorDeGotas[p]; 
                                double distancia = sqrt(pow(getPosicXText(t) - getPosicXCircle(gotinhas), 2) + pow(getPosicYText(t) - getPosicYCircle(gotinhas), 2));
                                //printf("Distancia do ponto até a gota: %lf Raio da gota: %lf\n", distancia, raio);
                                if (distancia <= raio)
                                {
                                    pragaAtual += proporcao;
                                    //printf("Praga atual do texto: %lf\n", pragaAtual);
                                }
                            }
                            
                            if(strcmp(texto, "@") == 0)
                            {
                                Hortalicas* cebola = &vetorHortalicas[ident]; 
                                acumularPraga(cebola, pragaAtual);
                                if(cebola->praga > 0)
                                    fprintf(arquivoTXT, "        Cebola de id %d atingida por praga. Grau de dano: %lf\n", ident, cebola->praga);
                                //printf("Proporção total cebola: %lf\n\n", cebola->praga);
                                
                            }else if(strcmp(texto, "*") == 0)
                            {
                                Hortalicas* morango = &vetorHortalicas[ident]; 
                                acumularPraga(morango, pragaAtual);
                                if(morango->praga > 0)
                                    fprintf(arquivoTXT, "        Morango de id %d atingido por praga. Grau de dano: %lf\n", ident, morango->praga);
                                //printf("Praga total morango: %lf\n\n", morango->praga);
                                
                            }else if(strcmp(texto, "%") == 0)
                            {
                                Hortalicas* cenoura = &vetorHortalicas[ident]; 
                                acumularPraga(cenoura, pragaAtual);
                                if(cenoura->praga > 0)
                                    fprintf(arquivoTXT, "        Cenouta de id %d atingida por praga. Grau de dano: %lf\n", ident, cenoura->praga);
                                //printf("Proporção total cenoura: %lf\n\n", cenoura->praga);
                            }else
                            {
                                Hortalicas* matoT = &vetorHortalicas[ident]; 
                                acumularPraga(matoT, pragaAtual);
                                if(matoT->praga > 0)
                                    fprintf(arquivoTXT, "        Mato de id %d atingido por praga. Grau de dano: %lf\n", ident, matoT->praga);
                                //printf("Proporção total matoT: %lf\n\n", matoT->praga);
                            }
                        }else if (strcmp(formato, "l") == 0)
                        {
                            Info infoL = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                            Linha l = (Linha)infoL;
                            Hortalicas* mato = &vetorHortalicas[ident];
                            double proporcao = 0.1;
                            double pragaAtual = 0;
                            for(int p=0; p<g; p++)
                            {
                                Circulo gotinhas = vetorDeGotas[p]; 
                                bool ponto1Dentro = (sqrt(pow(getPosicX1Line(l) - getPosicXCircle(gotinhas), 2) + pow(getPosicY1Line(l) - getPosicYCircle(gotinhas), 2)) <= raio);
                                bool ponto2Dentro = (sqrt(pow(getPosicX2Line(l) - getPosicXCircle(gotinhas), 2) + pow(getPosicY2Line(l) - getPosicYCircle(gotinhas), 2)) <= raio);
                                if (ponto1Dentro && ponto2Dentro)
                                {
                                    double distanciaCentros = sqrt(pow(getPosicXCircle(gotinhas) - getPosicX1Line(l), 2) + pow(getPosicYCircle(gotinhas) - getPosicY1Line(l), 2));
                                    if (distanciaCentros <= raio) //Linha completamente dentro da gota
                                    {
                                        pragaAtual += proporcao;
                                        //printf("Praga atual do mato dentro do for: %lf\n", pragaAtual); 
                                    }
                                }
                            }
                            acumularPraga(mato, pragaAtual);
                            //printf("Proporção total mato: %lf\n\n", mato->praga);
                            if(mato->praga > 0)
                                fprintf(arquivoTXT, "        Mato de id %d atingido por praga. Grau de dano: %lf\n", ident, mato->praga);
                        }
                    }
                }
            }



            //-----> CURA 
            if ((line[0] == 'c') && (line[1] == 'r'))
            {
                fprintf(arquivoTXT, "Cura das hortaliças:\n");
                double x, y, w, h, raio;
                int g = 0;
                sscanf(line, "cr %lf %lf %lf %lf %lf", &x, &y, &w, &h, &raio);
                Info vetorDeItens[totalDeItens];
                Circulo vetorDeGotas[totalDeItens]; 
                double areaGota = 3.14159 * raio * raio;
                int vetorDeIdentificadores[totalDeItens];

                //Desenhando o traço da área no SVG 
                Retangulo area = criaRetangulo(x, y, w, h, 200, "red", "none"); 
                Forma areaPontilhada = guarda_formato(area, "r"); 
                insertLst(listaBD, areaPontilhada);
                Circulo gota = criaCirculo(x, y, raio, 300, "yellow", "none"); 
                Forma gotaPontilhada = guarda_formato(gota, "c");
                insertLst(listaBD, gotaPontilhada);

                //Distribuindo as gotas uniformemente 
                double dist = raio * 2; // Distância entre os círculos é o dobro do raio
                
                //Espalhando as gotas pela área
                for (double posX = x; posX < x + w; posX += dist) 
                {
                  for (double posY = y; posY < y + h; posY += dist) 
                  {
                    Circulo gotas = criaCirculo(posX, posY, raio, 300, "yellow", "none");
                    Forma gotasPontilhadas = guarda_formato(gotas, "c");
                    //insertLst(listaBD, gotasPontilhadas);
                    vetorDeGotas[g] = gotas;
                    g++;
                  }
                }

                // Criando a estrutura RegiaoLista e preenchendo os valores
                RegiaoLista regiaoLista;
                regiaoLista.vetor = vetorDeItens; 
                regiaoLista.regiaoX = x;
                regiaoLista.regiaoY = y;
                regiaoLista.regiaoLargura = w;
                regiaoLista.regiaoAltura = h;
                regiaoLista.id = 0; 
                regiaoLista.vetorDeIds = vetorDeIdentificadores;

                //Buscando na árvore os nós que estão dentro da região e pegando seus id's
                char formato[5];
                procuraNoRadialT(arvore_radial, visitaNoRadial2, &regiaoLista); 

                //Reconhecendo a forma e jogando cura nela caso a gota esteja dentro                
                for(int e = 0; e<regiaoLista.id; e++)
                {
                    int ident = vetorDeIdentificadores[e]; 
                    Node n2 = vetorNos[ident];
                    if(n2 != NULL)
                    {
                        strcpy(formato, getFormato(vetorFormas[ident]));
                        if (strcmp(formato, "r") == 0) 
                        {
                            Info infoR = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                            Retangulo r = (Retangulo)infoR;
                            Hortalicas* repolho = &vetorHortalicas[ident]; 
                            double areaRet = getAltura(r)*getLargura(r);
                            double curaAtual = 0; 
                            double proporcao = areaGota/areaRet;
                            for(int p=0; p<g; p++)
                            {
                                Circulo gotinhas = vetorDeGotas[p]; 
                                
                                if (areaGota <= areaRet &&
                                getPosicXCircle(gotinhas) - raio >= getPosicXRect(r) &&
                                getPosicXCircle(gotinhas) + raio <= getPosicXRect(r) + getLargura(r) &&
                                getPosicYCircle(gotinhas) - raio >= getPosicYRect(r) &&
                                getPosicYCircle(gotinhas) + raio <= getPosicYRect(r) + getAltura(r)) 
                                { 
                                    curaAtual += proporcao;
                                    //printf("Defensivo atual do repolho dentro do for: %lf\n", curaAtual);
                                }
                            }
                            acumularCura(repolho, curaAtual);
                            if(repolho->defensivo > 0)    
                                fprintf(arquivoTXT, "        Repolho de id %d atingido por defensivo. Grau de cura: %lf\n", ident, repolho->defensivo);
                            //printf("Proporção total repolho: %lf\n\n", repolho->defensivo);
                        }else if (strcmp(formato, "c") == 0)
                        {
                            Info infoC = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                            Circulo c = (Circulo)infoC;
                            Hortalicas* abobora = &vetorHortalicas[ident];
                            double areaCirc = 3.14159 * getRaio(c) * getRaio(c);
                            double proporcao = areaGota/areaCirc;
                            double curaAtual = 0;
                            for(int p=0; p<g; p++)
                            {
                                Circulo gotinhas = vetorDeGotas[p]; 
                                if (areaGota <= areaCirc &&
                                    sqrt(pow(getPosicXCircle(gotinhas) - getPosicXCircle(c), 2) + pow(getPosicYCircle(gotinhas) - getPosicYCircle(c), 2)) + raio <= getRaio(c)) 
                                {
                                    curaAtual += proporcao;
                                    //printf("defensivo atual do repolho dentro do for: %lf\n", curaAtual);
                                }
                            }
                            acumularCura(abobora, curaAtual);
                            if(abobora->defensivo > 0)
                                fprintf(arquivoTXT, "        Abobora de id %d atingida por defensivo. Grau de cura: %lf\n", ident, abobora->defensivo);
                        }else if (strcmp(formato, "t") == 0)
                        {
                            Info infoT = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                            Texto t = (Texto)infoT;
                            char texto[5];
                            strcpy(texto, getTextoText(t));
                            double proporcao = 0.1;
                            double curaAtual = 0;
                            for(int p=0; p<g; p++)
                            {
                                Circulo gotinhas = vetorDeGotas[p]; 
                                double distancia = sqrt(pow(getPosicXText(t) - getPosicXCircle(gotinhas), 2) + pow(getPosicYText(t) - getPosicYCircle(gotinhas), 2));
                                if (distancia <= raio)
                                {
                                    curaAtual += proporcao;
                                    //printf("defensivo atual do repolho dentro do for: %lf\n", curaAtual);
                                }
                            }
                            
                            if(strcmp(texto, "@") == 0)
                            {
                                Hortalicas* cebola = &vetorHortalicas[ident]; 
                                acumularCura(cebola, curaAtual);
                                if(cebola->defensivo > 0)
                                    fprintf(arquivoTXT, "        Cebola de id %d atingida por defensivo. Grau de cura: %lf\n", ident, cebola->defensivo);
                                //printf("Proporção total cebola: %lf\n\n", cebola->defensivo);
                                
                            }else if(strcmp(texto, "*") == 0)
                            {
                                Hortalicas* morango = &vetorHortalicas[ident]; 
                                acumularCura(morango, curaAtual);
                                if(morango->defensivo > 0)
                                    fprintf(arquivoTXT, "        Morango de id %d atingido por defensivo. Grau de cura: %lf\n", ident, morango->defensivo);
                                //printf("Proporção total morango: %lf\n\n", morango->defensivo);
                                
                            }else if(strcmp(texto, "%") == 0)
                            {
                                Hortalicas* cenoura = &vetorHortalicas[ident]; 
                                acumularCura(cenoura, curaAtual);
                                if(cenoura->defensivo > 0)
                                    fprintf(arquivoTXT, "        Cenouta de id %d atingida por defensivo. Grau de cura: %lf\n", ident, cenoura->defensivo);
                                //printf("Proporção total cenoura: %lf\n\n", cenoura->defensivo);
                            }else
                            {
                                Hortalicas* matoT = &vetorHortalicas[ident]; 
                                acumularCura(matoT, curaAtual);
                                if(matoT->defensivo > 0)
                                    fprintf(arquivoTXT, "        Mato de id %d atingido por defensivo. Grau de cura: %lf\n", ident, matoT->defensivo);
                                //printf("Proporção total matoT: %lf\n\n", matoT->defensivo);
                            }
                        }else if (strcmp(formato, "l") == 0)
                        {
                            Info infoL = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                            Linha l = (Linha)infoL;
                            Hortalicas* mato = &vetorHortalicas[ident];
                            double proporcao = 0.1;
                            double curaAtual = 0;
                            for(int p=0; p<g; p++)
                            {
                                Circulo gotinhas = vetorDeGotas[p]; 
                                bool ponto1Dentro = (sqrt(pow(getPosicX1Line(l) - getPosicXCircle(gotinhas), 2) + pow(getPosicY1Line(l) - getPosicYCircle(gotinhas), 2)) <= raio);
                                bool ponto2Dentro = (sqrt(pow(getPosicX2Line(l) - getPosicXCircle(gotinhas), 2) + pow(getPosicY2Line(l) - getPosicYCircle(gotinhas), 2)) <= raio);
                                if (ponto1Dentro && ponto2Dentro)
                                {
                                    double distanciaCentros = sqrt(pow(getPosicXCircle(gotinhas) - getPosicX1Line(l), 2) + pow(getPosicYCircle(gotinhas) - getPosicY1Line(l), 2));
                                    if (distanciaCentros <= raio) //Linha completamente dentro da gota
                                    {
                                        curaAtual += proporcao;
                                        //printf("defensivo atual do mato dentro do for: %lf\n", curaAtual); 
                                    }
                                }
                            }
                            acumularCura(mato, curaAtual);
                            //printf("Proporção total mato: %lf\n\n", mato->defensivo);
                            if(mato->defensivo > 0)
                                fprintf(arquivoTXT, "        Mato de id %d atingido por defensivo. Grau de cura: %lf\n", ident, mato->defensivo);
                        }
                    }
                }
            }

            //-----> ADULBO 
            if ((line[0] == 'a') && (line[1] == 'd'))
            {
                fprintf(arquivoTXT, "Adulbo nas hortaliças:\n");
                double x, y, w, h, raio;
                int g = 0;
                sscanf(line, "ad %lf %lf %lf %lf %lf", &x, &y, &w, &h, &raio);
                Info vetorDeItens[totalDeItens];
                Circulo vetorDeGotas[totalDeItens]; 
                double areaGota = 3.14159 * raio * raio;
                int vetorDeIdentificadores[totalDeItens];

                //Desenhando o traço da área no SVG 
                Retangulo area = criaRetangulo(x, y, w, h, 200, "red", "none"); 
                Forma areaPontilhada = guarda_formato(area, "r"); 
                insertLst(listaBD, areaPontilhada);
                Circulo gota = criaCirculo(x, y, raio, 300, "green", "none"); 
                Forma gotaPontilhada = guarda_formato(gota, "c");
                insertLst(listaBD, gotaPontilhada);

                //Distribuindo as gotas uniformemente 
                double dist = raio * 2; // Distância entre os círculos é o dobro do raio

                //Espalhando as gotas pela área 
                for (double posX = x; posX < x + w; posX += dist) 
                {
                  for (double posY = y; posY < y + h; posY += dist) 
                  {
                    Circulo gotas = criaCirculo(posX, posY, raio, 300, "green", "none");
                    Forma gotasPontilhadas = guarda_formato(gotas, "c");
                    //insertLst(listaBD, gotasPontilhadas);
                    vetorDeGotas[g] = gotas;
                    g++;
                  }
                }

                // Criando a estrutura RegiaoLista e preenchendo os valores
                RegiaoLista regiaoLista;
                regiaoLista.vetor = vetorDeItens; 
                regiaoLista.regiaoX = x;
                regiaoLista.regiaoY = y;
                regiaoLista.regiaoLargura = w;
                regiaoLista.regiaoAltura = h;
                regiaoLista.id = 0; 
                regiaoLista.vetorDeIds = vetorDeIdentificadores;

                //Buscando na árvore os nós que estão dentro da região e pegando seus id's
                char formato[5];
                procuraNoRadialT(arvore_radial, visitaNoRadial2, &regiaoLista); 
                
                //Reconhecendo a forma e adubando ela caso a gota esteja dentro 
                for(int e = 0; e<regiaoLista.id; e++)
                {
                    int ident = vetorDeIdentificadores[e]; 
                    Node n2 = vetorNos[ident];
                    if(n2 != NULL)
                    {
                        strcpy(formato, getFormato(vetorFormas[ident]));
                        if (strcmp(formato, "r") == 0) 
                        {
                            Info infoR = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                            Retangulo r = (Retangulo)infoR;
                            Hortalicas* repolho = &vetorHortalicas[ident]; 
                            double areaRet = getAltura(r)*getLargura(r);
                            double aduboAtual = 0; 
                            double proporcao = areaGota/areaRet;
                            for(int p=0; p<g; p++)
                            {
                                Circulo gotinhas = vetorDeGotas[p]; 
                                
                                if (areaGota <= areaRet &&
                                getPosicXCircle(gotinhas) - raio >= getPosicXRect(r) &&
                                getPosicXCircle(gotinhas) + raio <= getPosicXRect(r) + getLargura(r) &&
                                getPosicYCircle(gotinhas) - raio >= getPosicYRect(r) &&
                                getPosicYCircle(gotinhas) + raio <= getPosicYRect(r) + getAltura(r)) 
                                { 
                                    aduboAtual += proporcao;
                                    //printf("adubo atual do repolho dentro do for: %lf\n", aduboAtual);
                                }
                            }
                            acumularAdubo(repolho, aduboAtual);
                            if(repolho->adubo > 0)    
                                fprintf(arquivoTXT, "        Repolho de id %d atingido por adubo. Grau de adubagem: %lf\n", ident, repolho->adubo);
                            //printf("Proporção total repolho: %lf\n\n", repolho->adubo);
                            
                        }else if (strcmp(formato, "c") == 0)
                        {
                            Info infoC = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                            Circulo c = (Circulo)infoC;
                            Hortalicas* abobora = &vetorHortalicas[ident];
                            double areaCirc = 3.14159 * getRaio(c) * getRaio(c);
                            double proporcao = areaGota/areaCirc;
                            double aduboAtual = 0;
                            for(int p=0; p<g; p++)
                            {
                                Circulo gotinhas = vetorDeGotas[p]; 
                                if (areaGota <= areaCirc &&
                                    sqrt(pow(getPosicXCircle(gotinhas) - getPosicXCircle(c), 2) + pow(getPosicYCircle(gotinhas) - getPosicYCircle(c), 2)) + raio <= getRaio(c)) 
                                {
                                    aduboAtual += proporcao;
                                    //printf("adubo atual do repolho dentro do for: %lf\n", aduboAtual);
                                }
                            }
                            acumularAdubo(abobora, aduboAtual);
                            if(abobora->adubo > 0)
                                fprintf(arquivoTXT, "        Abobora de id %d atingida por adubo. Grau de adubagem: %lf\n", ident, abobora->adubo);
                        }else if (strcmp(formato, "t") == 0)
                        {
                            Info infoT = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                            Texto t = (Texto)infoT;
                            char texto[5];
                            strcpy(texto, getTextoText(t));
                            double proporcao = 0.1;
                            double aduboAtual = 0;
                            for(int p=0; p<g; p++)
                            {
                                Circulo gotinhas = vetorDeGotas[p]; 
                                double distancia = sqrt(pow(getPosicXText(t) - getPosicXCircle(gotinhas), 2) + pow(getPosicYText(t) - getPosicYCircle(gotinhas), 2));
                                if (distancia <= raio)
                                {
                                    aduboAtual += proporcao;
                                    //printf("adubo atual do repolho dentro do for: %lf\n", aduboAtual);
                                }
                            }
                            
                            if(strcmp(texto, "@") == 0)
                            {
                                Hortalicas* cebola = &vetorHortalicas[ident]; 
                                acumularAdubo(cebola, aduboAtual);
                                if(cebola->adubo > 0)
                                    fprintf(arquivoTXT, "        Cebola de id %d atingida por adubo. Grau de adubagem: %lf\n", ident, cebola->adubo);
                                //printf("Proporção total cebola: %lf\n\n", cebola->adubo);
                                
                            }else if(strcmp(texto, "*") == 0)
                            {
                                Hortalicas* morango = &vetorHortalicas[ident]; 
                                acumularAdubo(morango, aduboAtual);
                                if(morango->adubo > 0)
                                    fprintf(arquivoTXT, "        Morango de id %d atingido por adubo. Grau de adubagem: %lf\n", ident, morango->adubo);
                                //printf("Proporção total morango: %lf\n\n", morango->adubo);
                                
                            }else if(strcmp(texto, "%") == 0)
                            {
                                Hortalicas* cenoura = &vetorHortalicas[ident]; 
                                acumularAdubo(cenoura, aduboAtual);
                                if(cenoura->adubo > 0)
                                    fprintf(arquivoTXT, "        Cenouta de id %d atingida por adubo. Grau de adubagem: %lf\n", ident, cenoura->adubo);
                                //printf("Proporção total cenoura: %lf\n\n", cenoura->adubo);
                            }else
                            {
                                Hortalicas* matoT = &vetorHortalicas[ident]; 
                                acumularAdubo(matoT, aduboAtual);
                                if(matoT->adubo > 0)
                                    fprintf(arquivoTXT, "        Mato de id %d atingido por adubo. Grau de adubagem: %lf\n", ident, matoT->adubo);
                                //printf("Proporção total matoT: %lf\n\n", matoT->adubo);
                            }
                        }else if (strcmp(formato, "l") == 0)
                        {
                            Info infoL = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                            Linha l = (Linha)infoL;
                            Hortalicas* mato = &vetorHortalicas[ident];
                            double proporcao = 0.1;
                            double aduboAtual = 0;
                            for(int p=0; p<g; p++)
                            {
                                Circulo gotinhas = vetorDeGotas[p]; 
                                bool ponto1Dentro = (sqrt(pow(getPosicX1Line(l) - getPosicXCircle(gotinhas), 2) + pow(getPosicY1Line(l) - getPosicYCircle(gotinhas), 2)) <= raio);
                                bool ponto2Dentro = (sqrt(pow(getPosicX2Line(l) - getPosicXCircle(gotinhas), 2) + pow(getPosicY2Line(l) - getPosicYCircle(gotinhas), 2)) <= raio);
                                if (ponto1Dentro && ponto2Dentro)
                                {
                                    double distanciaCentros = sqrt(pow(getPosicXCircle(gotinhas) - getPosicX1Line(l), 2) + pow(getPosicYCircle(gotinhas) - getPosicY1Line(l), 2));
                                    if (distanciaCentros <= raio) //Linha completamente dentro da gota
                                    {
                                        aduboAtual += proporcao;
                                        //printf("adubo atual do mato dentro do for: %lf\n", aduboAtual); 
                                    }
                                }
                            }
                            acumularAdubo(mato, aduboAtual);
                            //printf("Proporção total mato: %lf\n\n", mato->adubo);
                            if(mato->adubo > 0)
                                fprintf(arquivoTXT, "        Mato de id %d atingido por adubo. Grau de adubagem: %lf\n", ident, mato->adubo);
                        }
                    }
                }
            }



            //-----> SEMENTES  
            if ((line[0] == 's') && (line[1] == 't'))
            {
                fprintf(arquivoTXT, "Semeadura das hortaliças:\n");
                int j;
                double x, y, w, h, dx, dy, f;
                sscanf(line, "st %lf %lf %lf %lf %lf %lf %lf %d", &x, &y, &w, &h, 
                                                                        &f, &dx, &dy, &j);
                if(f<1)
                    f = 1;
                
                Info vetorDeItens[totalDeItens];
                int vetorDeIdentificadores[totalDeItens];
                Forma forma;

                //Desenhando o traço da área no SVG 
                Retangulo area = criaRetangulo(x, y, w, h, 200, "red", "none"); 
                Forma areaPontilhada = guarda_formato(area, "r"); 
                insertLst(listaBD, areaPontilhada);
                double novoX = x + dx; 
                double novoY = y + dy; 
                Retangulo area2 = criaRetangulo(novoX, novoY, w, h, 201, "red", "none"); 
                Forma areaPontilhada2 = guarda_formato(area2, "r"); 
                insertLst(listaBD, areaPontilhada2);
                Circulo gota = criaCirculo(x, y, 9, 300, "red", "red"); 
                Forma gotaPontilhada = guarda_formato(gota, "c");
                insertLst(listaBD, gotaPontilhada);

                // Criando a estrutura RegiaoLista e preenchendo os valores
                RegiaoLista regiaoLista;
                regiaoLista.vetor = vetorDeItens; 
                regiaoLista.regiaoX = x;
                regiaoLista.regiaoY = y;
                regiaoLista.regiaoLargura = w;
                regiaoLista.regiaoAltura = h;
                regiaoLista.id = 0; 
                regiaoLista.vetorDeIds = vetorDeIdentificadores;

                //Buscando na árvore os nós que estão dentro da região e pegando seus id's
                char formato[5];
                procuraNoRadialT(arvore_radial, visitaNoRadial2, &regiaoLista); 
                
                //Reconhecendo a forma e semeando a área selecionada 
                for(int e = 0; e<regiaoLista.id; e++)
                {
                    int ident = vetorDeIdentificadores[e]; 
                    Node n2 = vetorNos[ident];
                    if(n2 != NULL)
                    {
                        strcpy(formato, getFormato(vetorFormas[ident]));
                        if (strcmp(formato, "r") == 0) 
                        {
                            Info infoR = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                            Retangulo r = (Retangulo)infoR;
                            
                            if(verificaColheitadeira(r) == false)
                            {
                                double xOriginal = getPosicXRect(r);
                                double yOriginal = getPosicYRect(r); 
                                double xSemente = xOriginal + dx; 
                                double ySemente = yOriginal + dy; 
                                int fator = 0;
                                while(fator < f)
                                {
                                    fator++;
                                    if (fator >= 1)
                                    {
                                        xSemente += 5; // Deslocamento de 5 unidades para a direita
                                        ySemente -= 5;
                                    }
                                    Hortalicas repolho; 
                                    Retangulo r2 = criaRetangulo(xSemente,ySemente,getLargura(r),getAltura(r),j,getCorbRect(r),getCorpRect(r));
                                    Forma novo = guarda_formato(r2, "r"); 
                                    vetorPosicoes[j] = insertLst(listaBD, novo);
                                    vetorNos[j] = insertRadialT(arvore_radial, xSemente, ySemente, r2, j);
                                    vetorFormas[j] = novo;
                                    vetorHortalicas[j] = repolho; 
                                    repolho.id = j;
                                    repolho.peso = 1000;
                                    repolho.praga = 0; 
                                    repolho.defensivo = 0;
                                    repolho.adubo = 0;
                                    fprintf(arquivoTXT, "        Repolho de id %d foi semeado e gerou o clone de id %d\n", ident, j);
                                    j++;
                                }
                            }
                        }else if (strcmp(formato, "c") == 0)
                        {
                            Info infoC = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                            Circulo c = (Circulo)infoC;
                            
                            double xOriginal = getPosicXCircle(c);
                            double yOriginal = getPosicYCircle(c); 
                            double xSemente = xOriginal + dx; 
                            double ySemente = yOriginal + dy; 
                            for(int fator=0; fator<f; fator++)
                            {
                                if (fator >= 1)
                                {
                                    xSemente += 5; // Deslocamento de 5 unidades para a direita
                                    ySemente -= 5;
                                }
                                Hortalicas abobora;  
                                Circulo c2 = criaCirculo(xSemente,ySemente,getRaio(c),j,getCorbCircle(c),getCorpCircle(c));
                                forma = guarda_formato(c2, "c");  
                                vetorPosicoes[j] = insertLst(listaBD, forma); 
                                vetorNos[j] = insertRadialT(arvore_radial, xSemente, ySemente, c2, j);
                                vetorFormas[j] = forma; 
                                vetorHortalicas[j] = abobora; 
                                abobora.id = j;
                                abobora.peso = 2000;
                                abobora.praga = 0; 
                                abobora.defensivo = 0;
                                abobora.adubo = 0; 
                                fprintf(arquivoTXT, "        Abobora de id %d foi semeada e gerou o clone de id %d\n", ident, j);
                                j++; 
                            }    
                        }else if (strcmp(formato, "t") == 0)
                        {
                            Info infoT = getInfoRadialT(arvore_radial, vetorNos[ident]); 
                            Texto t = (Texto)infoT;
                            
                            double xOriginal = getPosicXText(t);
                            double yOriginal = getPosicYText(t); 
                            double xSemente = xOriginal + dx; 
                            double ySemente = yOriginal + dy; 
                            char texto[5];
                            strcpy(texto, getTextoText(t));
                            
                            for(int fator=0; fator<f; fator++)
                            {
                                if (fator >= 1)
                                {
                                    xSemente += 5; // Deslocamento de 5 unidades para a direita
                                    ySemente -= 5;
                                }
                                Texto t2 = criaTexto(xSemente,ySemente,j,"m", texto, getCorbText(t), getCorpText(t), getSizeText(t), FontFamily, FontWeigth);
                                Posic p4 = insertLst(textosDaListaBD, t);
                                Item i4 = getLst(textosDaListaBD, p4); 
                                Forma forminha = guarda_formato(t2, "t");
                                vetorPosicoes[j] = insertLst(listaBD, forminha);
                                vetorNos[j] = insertRadialT(arvore_radial, xSemente, ySemente, t2, j);
                                vetorFormas[j] = forminha;
                                if(strcmp(texto, "@") == 0)
                                {
                                    Hortalicas cebola = vetorHortalicas[ident]; 
                                    vetorHortalicas[j] = cebola; 
                                    cebola.id = j;
                                    cebola.peso = 2000;
                                    cebola.praga = 0; 
                                    cebola.defensivo = 0;
                                    cebola.adubo = 0; 
                                    fprintf(arquivoTXT, "        Cebola de id %d foi semeada e gerou o clone de id %d\n", ident, j);
                                }else if(strcmp(texto, "*") == 0)
                                {
                                    Hortalicas morango = vetorHortalicas[ident];  
                                    vetorHortalicas[j] = morango; 
                                    morango.id = j;
                                    morango.peso = 20;
                                    morango.praga = 0; 
                                    morango.defensivo = 0;
                                    morango.adubo = 0;
                                    fprintf(arquivoTXT, "        Morango de id %d foi semeado e gerou o clone de id %d\n", ident, j);
                                }else if(strcmp(texto, "%") == 0)
                                {
                                    Hortalicas cenoura = vetorHortalicas[ident]; 
                                    vetorHortalicas[j] = cenoura; 
                                    cenoura.id = j;
                                    cenoura.peso = 70;
                                    cenoura.praga = 0; 
                                    cenoura.defensivo = 0;
                                    cenoura.adubo = 0;
                                    fprintf(arquivoTXT, "        Cenoura de id %d foi semeada e gerou o clone de id %d\n", ident, j);
                                }else
                                {
                                    Hortalicas matoT = vetorHortalicas[ident]; 
                                    vetorHortalicas[j] = matoT; 
                                    matoT.id = j;
                                    matoT.peso = 15;
                                    matoT.praga = 0; 
                                    matoT.defensivo = 0;
                                    matoT.adubo = 0;
                                    fprintf(arquivoTXT, "        Mato de id %d foi semeada e gerou o clone de id %d\n", ident, j);
                                }
                                j++; 
                            }
                        }
                    }
                }
            }



            //-----> REPORTA DADOS DA FIGURA id 
            if ((line[0] == 'd') && (line[1] == '?'))
            {
                fprintf(arquivoTXT, "Reportando dados de hortaliças:\n");
                int id; 
                sscanf(line, "d? %d", &id);
                char formatoH[6]; 
                strcpy(formatoH, getFormato(vetorFormas[id]));
                //printf("Formato da hortaliça: %s\n", formatoH);

                if(strcmp(formatoH, "r") == 0)
                {
                    Hortalicas* repolho = &vetorHortalicas[id]; 
                    Info infoR = getInfoRadialT(arvore_radial, vetorNos[id]); 
                    Retangulo r = (Retangulo)infoR;   
                    double xRect = getPosicXRect(r);
                    double yRect = getPosicYRect(r);
                    fprintf(arquivoTXT, "        Dados da hortaliça %d:\n                Tipo: Repolho\n                Peso: %lf\n                Posição: %lf,%lf\n", id, repolho->peso, xRect, yRect);
                }else if(strcmp(formatoH, "c") == 0)
                {
                    Hortalicas* abobora = &vetorHortalicas[id]; 
                    Info infoC = getInfoRadialT(arvore_radial, vetorNos[id]); 
                    Circulo c = (Circulo)infoC;   
                    double xCirc = getPosicXCircle(c);
                    double yCirc = getPosicYCircle(c);
                    fprintf(arquivoTXT, "        Dados da hortaliça %d:\n                Tipo: Abobora\n                Peso: %lf\n                Posição: %lf,%lf\n", id, calcularMassaFinal(abobora), xCirc, yCirc);
                }else if(strcmp(formatoH, "t") == 0)
                { 
                    Info infoT = getInfoRadialT(arvore_radial, vetorNos[id]); 
                    Texto t = (Texto)infoT;   
                    double xTexto = getPosicXText(t);
                    double yTexto = getPosicYText(t);
                    char texto[10];
                    strcpy(texto, getTextoText(t));

                    if(strcmp(texto, "@") == 0)
                    {
                        Hortalicas* cebola = &vetorHortalicas[id]; 
                        fprintf(arquivoTXT, "        Dados da hortaliça %d:\n                Tipo: Cebola\n                Peso: %lf\n                Posição: %lf,%lf\n", id, calcularMassaFinal(cebola), xTexto, yTexto);
                    }else if(strcmp(texto, "*") == 0)
                    {
                        Hortalicas* morango = &vetorHortalicas[id];  
                        fprintf(arquivoTXT, "        Dados da hortaliça %d:\n                Tipo: Morango\n                Peso: %lf\n                Posição: %lf,%lf\n", id, calcularMassaFinal(morango), xTexto, yTexto);
                    }else if(strcmp(texto, "%") == 0)
                    {
                        Hortalicas* cenoura = &vetorHortalicas[id]; 
                        fprintf(arquivoTXT, "        Dados da hortaliça %d:\n                Tipo: Cenoura\n                Peso: %lf\n                Posição: %lf,%lf\n", id, calcularMassaFinal(cenoura), xTexto, yTexto);
                    }else
                    {
                        Hortalicas* matoT = &vetorHortalicas[id]; 
                        fprintf(arquivoTXT, "        Dados da hortaliça %d:\n                Tipo: Mato\n                Peso: %lf\n                Posição: %lf,%lf\n", id, calcularMassaFinal(matoT), xTexto, yTexto);
                    }
                }
            }



            //-----> REPORTA ATRIBUTOS DE TODAS COLHEITADEIRAS 
            if ((line[0] == 'c') && (line[1] == '?'))
            {
                fprintf(arquivoTXT, "Dados de todas colheitadeiras:\n");
                char formato[5];
                for(int q=1; q<=totalDeItens; q++)
                {
                    if(vetorNos[q] != NULL)
                    {
                        strcpy(formato, getFormato(vetorFormas[q]));
                        if (strcmp(formato, "r") == 0) 
                        {
                            Info infoR = getInfoRadialT(arvore_radial, vetorNos[q]); 
                            Retangulo r = (Retangulo)infoR;
                            if(verificaColheitadeira(r))
                            {
                                fprintf(arquivoTXT, "        Colheitadeira de id %d:\n                Posição: %lf,%lf\n                Cor de preenchimento: %s\n", q, getPosicXRect(r), getPosicYRect(r), getCorpRect(r));
                            }
                        }
                    }
                } 
            }
        }
    }
}






int main(int argc, char *argv[]) 
{
    //Criando variáveis que vão me auxiliar no programa  
    Retangulo* vetorPonteirosRetangulos[30000];
    Node* vetorDeNos[30000]; 
    char *entrada = NULL;
    char *saida = NULL;
    char *sufixo_completo = NULL;
    char *caminho_completoGeo = NULL;
    char *caminho_completoQry = NULL;
    char *arquivoGeo, *arquivoQry;
    char FontFamily[10]; 
    char FontWeigth[10];
    double size;
    Lista formasAuxiliares = createLst(300);
    Forma* vetorDeFormas[30000];
    Posic* vetorDePosicoes[30000];
    Hortalicas vetorDeHortalicas[30000]; 
    double vetorX[30000];
    double vetorY[30000];
    int ns;
    double fd;
    
    
    //Chamando a função que guarda caminhos para os arquivos que serão lidos 
    le_comando(argc, argv, &entrada, &saida, &arquivoGeo, &arquivoQry, &sufixo_completo, 
                                &caminho_completoGeo, &caminho_completoQry, &ns, &fd);
    
    if(ns && fd)
        printf("Número de setores: %d Fator de degradação: %lf\n", ns, fd); 
    
    int numeroSetores = ns;
    double fatorDeDegradacao = fd; 

    //Criando os ponteiros para abrir os arquivos para leitura 
    char* geo_path = NULL;
    char* qry_path = NULL;
    FILE* geo_file = NULL;
    FILE* qry_file = NULL; 

    
    //Abrindo os arquivos .geo e .qry para leitura 
    if (caminho_completoGeo != NULL) 
       abrir_arquivo(caminho_completoGeo, &geo_file);

    if (caminho_completoQry != NULL)
        abrir_arquivo(caminho_completoQry, &qry_file); 
    

    //Criando a árvore radial que vai armazenar tudo 
    RadialTree arvore_radial = newRadialTree(numeroSetores, fatorDeDegradacao); 
    
    //Criando algumas listas para auxiliar 
    Lista *listaBancoDados = createLst(30000);
    Lista *r = createLst(30000);
    Lista *c = createLst(30000);
    Lista *l = createLst(30000);
    Lista *t = createLst(30000);


    //Criando o arquivo de texto 
    char nome_arquivoTXT[100]; 
    sprintf(nome_arquivoTXT, "%s.txt", sufixo_completo);
    char caminho_arquivoTXT[200];
    sprintf(caminho_arquivoTXT, "%s/%s", saida, nome_arquivoTXT);
    FILE *arquivoTXT;
    arquivoTXT = fopen(caminho_arquivoTXT, "w");


    //Colocando uma mensagem no .txt em casos de não haver .qry 
    if (qry_file == NULL)
    {
        fprintf(arquivoTXT, "         \nNão há arquivo .qry para reportar movimentações.\n");
    }


    //Processando o .geo e .qry 
    processaGeo(geo_file, c, r, l, t, listaBancoDados, FontFamily, vetorPonteirosRetangulos, 
                                        vetorDeNos, FontWeigth, size, arvore_radial, vetorDeFormas, 
                                        vetorDePosicoes, vetorDeHortalicas, vetorX, vetorY);
    
    processaQry(qry_file, c, r, l, t, listaBancoDados, arquivoTXT, saida, sufixo_completo, 
                                        FontFamily, FontWeigth, size, formasAuxiliares, arvore_radial, 
                                        vetorDeNos, vetorDeFormas, vetorDePosicoes, vetorDeHortalicas); 

   
   //Criando e Escrevendo no arquivo SVG 
    char nome_arquivoSVG[100]; 
    sprintf(nome_arquivoSVG, "%s.svg", sufixo_completo);
    char caminho_arquivoSVG[200];
    sprintf(caminho_arquivoSVG, "%s/%s", saida, nome_arquivoSVG);
    ArqSvg desenho = abreEscritaSvg(caminho_arquivoSVG);
    desenharFotosSvg(listaBancoDados, desenho);


    //Criando e Escrevendo no arquivo DOT
    char nome_arquivoDot[100];  
    sprintf(nome_arquivoDot, "%s.dot", sufixo_completo);
    char caminho_arquivoDOT[200];
    sprintf(caminho_arquivoDOT, "%s/%s", saida, nome_arquivoDot);
    FILE *arquivoDOT = fopen(caminho_arquivoDOT, "w");
    fprintf(arquivoDOT, "digraph G {\n");
    fprintf(arquivoDOT, "    node [shape=box];\n");
    fprintf(arquivoDOT, "    graph [layout=neato];\n"); 
    Node raiz = getRaiz(arvore_radial);
    printDotRadialTreeRecursive(arvore_radial, raiz, arquivoDOT); 
    fprintf(arquivoDOT, "}\n");


    
    //Fechando os arquivos que foram abertos 
    if (qry_file != NULL) 
    {
        fclose(qry_file);
    }else 
        printf("ponteiro para qry NULO\n");

    if (geo_file != NULL) 
        fclose(arquivoTXT);

    if (arquivoDOT != NULL)
        fclose(arquivoDOT);

    if (geo_path != NULL) 
    {
        free(geo_path);
    }
    if (qry_path != NULL) 
    {
        free(qry_path);
    }

    //Liberando a memória alocada pelas estruturas
    killLst(listaBancoDados);
    killLst(r);
    killLst(l);
    killLst(c);
    killLst(t);
    killLst(formasAuxiliares);
    killRadialTree(arvore_radial);

    return 0;
}






                                //FUNÇÕES PARA USAR NO main.c 



/** Abre o arquivo de texto para ser lido.
 * A função recebe o caminho de onde está esse arquivo por 
 * meio do parâmetro char* path e abre o arquivo, no qual 
 * o caminho leva. */
void abrir_arquivo(char* path, FILE** file) 
{
    *file = fopen(path, "r");
    if (*file == NULL) 
    {
        printf("Erro ao abrir o arquivo %s.\n", path);
        return;
    }else
    {
        printf("Arquivo %s aberto com sucesso.\n", path);
    }
}


 
/** Função auxiliar para gerar o nome dos 
 * arquivos finais na função le_comando() */
char* strdup(const char* s) 
{
    // Calcula o tamanho da string
    size_t len = strlen(s) + 1;
    
    // Aloca memória suficiente para a cópia
    char* new_str = malloc(len);
    
    // Copia a string original para a nova área de memória
    if (new_str != NULL) {
        memcpy(new_str, s, len);
    }
    
    // Retorna o ponteiro para a nova cópia da string
    return new_str;
}


/** Função lê a linha de comando no terminal, guarda nas variáveis passadas por
 * parâmetro os seguintes conteúdos: entrada: caminho para arquivos de entrada -e
 * saida: caminho para arquivos de saída -o arquivoGeo: caminho recebido após -f
 * arquivoQry: caminho recebido após -q sufixo_completo: nome base dos arquivos de saída SVG e TXT 
 * caminho_completoGeo: caminho absoluto para achar arquivo .geo
 * caminho_completoQry: caminho absoluto para achar arquivo .qry*/
void le_comando(int argc, char *argv[], char **entrada, char **saida, char **arquivoGeo, 
                 char **arquivoQry, char **sufixo_completo, char **caminho_completoGeo, 
                 char **caminho_completoQry, int *ns, double *p) 
{
    char *sufixo_geo = NULL;
    char *sufixo_qry = NULL;
    bool quiet; 
    char *caminho_completo = NULL;
    char *base_geo = NULL;
    char *base_qry = NULL; 
    


    // Loop para extrair as informações dos argumentos de linha de comando
    for (int i = 1; i < argc; i++) 
    {
        if (strcmp(argv[i], "-e") == 0 && i+1 < argc) 
        {
            *entrada = argv[i+1];
            // verifica se o último caractere é uma barra ("/")
            //int len = strlen(*entrada);
            //if ((*entrada)[len-1] == '/') 
            //{
            //    (*entrada)[len-1] = '\0'; // remove a barra
            //}
            //printf("Achei o -e: %s\n", *entrada); 
        }
        else if (strcmp(argv[i], "-o") == 0 && i+1 < argc) 
        {
            *saida = argv[i+1];
            //printf("Achei o -o: %s\n", *saida);
            i++;
        }
        else if (strcmp(argv[i], "-f") == 0 && i+1 < argc) 
        {
            *arquivoGeo = argv[i+1];
            base_geo = argv[i+1];
            //printf("Achei o -f!!! Arquivo -f: %s\n", *arquivoGeo);

            //Concatenando -e com -f para dar o caminho completo do arquivo .geo 
            int tamanho2 = strlen(*entrada)+strlen(*arquivoGeo)+2; //tamanho das duas strings mais '/' e '\0' 
            *caminho_completoGeo = malloc(tamanho2*(sizeof(char)));
            sprintf(*caminho_completoGeo, "%s/%s", *entrada, *arquivoGeo);
        
            char *extensao = strchr(base_geo, '.'); //encontra a primeira ocorrência de '.'
            if (extensao != NULL) 
            {
                *extensao = '\0'; //se houver '.', substitui-o por '\0' para truncar a string
            }
            //printf("Base Geo Depois: %s\n", base_geo);

        }
        else if (strcmp(argv[i], "-q") == 0)
        {
            *arquivoQry = argv[i+1]; 
            base_qry = argv[i+1];
            //printf("Achei o -q: %s\n", *arquivoQry);

            //printf("Arquivo Qry Antes de trabalhar sufixo: %s\n", *arquivoQry);
            int tamanho3 = strlen(*entrada)+strlen(*arquivoQry)+2; //tamanho das duas strings mais '/' e '\0' 
            *caminho_completoQry = malloc(tamanho3*(sizeof(char)));
            sprintf(*caminho_completoQry, "%s/%s", *entrada, *arquivoQry);
            //printf("Caminho completo Qry: %s\n", *caminho_completoQry);


            char *last_sep = strrchr(base_qry, '/'); //encontra a última ocorrência de '/'
            if (last_sep != NULL) 
            {
                base_qry = last_sep + 1; //se houver '/', atualiza dir_name para apontar para a string depois de '/'
            }
            char *extensao = strchr(base_qry, '.'); //encontra a primeira ocorrência de '.'
            if (extensao != NULL) 
            {
                *extensao = '\0'; //se houver '.', substitui-o por '\0' para truncar a string
            }
            //printf("Base Qry Depois: %s\n", base_qry);
            quiet = true;
        }else if (strcmp(argv[i], "-ns") == 0)
        {
            *ns = atoi(argv[i+1]);
        }else if (strcmp(argv[i], "-fd") == 0)
        {
            *p = atof(argv[i+1]);
        }
    }

    //Fazendo tratamento dos NOMES dos arquivos de saída 
    if(quiet)
    {
        int tamanho = strlen(base_geo)+strlen(base_qry)+2; //tamanho das duas strings mais '-' e '\0' 
        *sufixo_completo = malloc(tamanho*sizeof(char)); 
        sprintf(*sufixo_completo, "%s-%s", base_geo, base_qry);
    }else
    {
        *sufixo_completo = strdup(base_geo);
    }
    printf("Sufixo Completo: %s\n", *sufixo_completo);     
}


//Função para escrever no SVG passando apenas pela lista geral 
/** Escreve textos no arquivo SVG, buscando elemento por elemento 
 * de uma única lista geral (banco de dados). Essa lista é passada
 * no parâmetro Lista* formas. A função identifica o tipo de cada
 * item e escreve seu respectivo código no SVG. */
void desenharFotosSvg(Lista* formas, ArqSvg desenho) 
{
    Node* pCurr;

    char formato[5]; 

    // percorre a lista de formas
    for (pCurr = getFirstLst(formas); pCurr != NULL; pCurr = getNextLst(formas, pCurr)) 
    {
        Forma forma = getLst(formas, pCurr);
        strcpy(formato, getFormato(forma)); 
        //printf("********Formato: %s\n", formato);
        
        // identifica o tipo da forma e chama a função de escrita correspondente
        if (strcmp(formato, "r") == 0) 
        {
                Retangulo rect = getObjeto(forma);
                //printf("Retangulo: %d\n", getIdentificadorRetangulo(rect));
                escreveRetanguloSvg(rect, desenho);
        }else if(strcmp(formato, "c") == 0)
        {
            Circulo circle = getObjeto(forma);
            //printf("Circulo: %d\n", getIdentificadorCirculo(circle));
            escreveCirculoSvg(circle, desenho);
        }else if(strcmp(formato, "l") == 0)
        {
            Linha line = getObjeto(forma);
            //printf("Linha: %d\n", getIdentificadorLine(line));
            escreveLinhaSvg(line, desenho);
        }else if(strcmp(formato, "t") == 0)
        {
            Texto text = getObjeto(forma);
            //printf("Texto de id: %d Texto: %s\n", getIdentificadorText(text), getTextoText(text));
            escreveTextoSvg(text, desenho); 
        }else 
        {
            //printf("Formato não encontrado!\n");
        }
    }

    fechaSvg(desenho);
}


/*
 * Função de visita a um nó em uma árvore radial.
 * Verifica se as coordenadas (x, y) do nó estão dentro da região retangular
 * especificada em `aux`, e guarda o item do nó na lista de região.
 *
 * @param i O item associado ao nó visitado.
 * @param x A coordenada x do nó visitado.
 * @param y A coordenada y do nó visitado.
 * @param aux O ponteiro para a estrutura de dados auxiliar (RegiaoLista).
 */
void visitaNoRadial(Info i, double x, double y, void* aux) 
{
    RegiaoLista* regiaoLista = (RegiaoLista*)aux;
    double regiaoX = regiaoLista->regiaoX;
    double regiaoY = regiaoLista->regiaoY;
    double regiaoLargura = regiaoLista->regiaoLargura;
    double regiaoAltura = regiaoLista->regiaoAltura; 
    int idDoNo; 
    
    // Verifica se o nó está dentro da região retangular e guarda o seu item 
    if ((x >= regiaoX) && (x <= regiaoX + regiaoLargura) &&
    (y >= regiaoY) && (y <= regiaoY + regiaoAltura)) 
    {
        regiaoLista->vetor[regiaoLista->id] = i; 
        regiaoLista->id += 1;
    }
}




/*
 * Obtém o peso de uma hortaliça com base no seu identificador.
 *
 * @param hortalicas O vetor de hortaliças.
 * @param tamanho O tamanho do vetor de hortaliças.
 * @param id O identificador da hortaliça desejada.
 * @return Retorna o peso da hortaliça com o identificador fornecido. Se o identificador não for encontrado, retorna -1.
 */
double obterPesoPorId(Hortalicas* hortalicas, int tamanho, int id) 
{
    for (int i = 0; i < tamanho; i++) 
    {
        if (hortalicas[i].id == id) 
        {
            return hortalicas[i].peso;
        }
    }
    return -1;
}


/*
 * Acumula o valor fornecido no nível de praga de uma hortaliça.
 *
 * @param hortalicas A hortaliça na qual o valor será acumulado.
 * @param valorAcumulado O valor a ser acumulado no nível de praga.
 */
void acumularPraga(Hortalicas* hortalicas, double valorAcumulado)
{
    hortalicas->praga += valorAcumulado;
}


/*
 * Acumula o valor fornecido no nível de defensivo de uma hortaliça.
 *
 * @param hortalicas A hortaliça na qual o valor será acumulado.
 * @param valorAcumulado O valor a ser acumulado no nível de defensivo.
 */
void acumularCura(Hortalicas* hortalicas, double valorAcumulado)
{
    hortalicas->defensivo += valorAcumulado;
}


/*
 * Acumula o valor fornecido no nível de adubo de uma hortaliça.
 *
 * @param hortalicas A hortaliça na qual o valor será acumulado.
 * @param valorAcumulado O valor a ser acumulado no nível de adubo.
 */
void acumularAdubo(Hortalicas* hortalicas, double valorAcumulado)
{
    hortalicas->adubo += valorAcumulado;
}


/*
 * Calcula a massa final de uma hortaliça considerando seu peso, níveis de praga, adubo e defensivo.
 *
 * @param hortalica A hortaliça para a qual será calculada a massa final.
 * @return Retorna a massa final da hortaliça.
 */
double calcularMassaFinal(Hortalicas* hortalica) 
{
    double massaComprometida = hortalica->peso * hortalica->praga;
    double massaAdicionada = hortalica->peso * hortalica->adubo;
    double massaRecuperada = massaComprometida * hortalica->defensivo;
    double massaFinal = hortalica->peso - massaComprometida + massaRecuperada + massaAdicionada;
    return massaFinal;
}
