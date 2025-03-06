#include "radialtree.h"
#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "quick_sort.h"
#define M_PI 3.14

struct radialtree;



struct radialtree
{
    struct node *raiz; //raiz da árvore radial 
    int totalRemovidos;
    int totalNos;
    int totalFilhos;
    double fatorDegAtual; // Fator de Degradação atual 
    double fatorDegMaximo;
};

struct node
{
    struct radialtree *arvoreAtual;
    bool excluido; //Verifica se foi deletado 
    struct node **filhos;
    struct node *pai;
    double x;  
    double y;
    Item item; 
    int totalFilhosDoNo;
    int id; 
};



RadialTree newRadialTree(int numSetores, double fd)
{
    struct radialtree *arvore = (struct radialtree *)malloc(sizeof(struct radialtree));
    arvore->raiz = NULL;
    arvore->fatorDegAtual = 0;
    fd = fd > 0 ? fd : 0;    // fd deve ser maior que 0
    fd = fd < 1 ? fd : 0.75; // fd deve ser menor que 1
    arvore->fatorDegMaximo = fd;
    arvore->totalFilhos = numSetores;
    arvore->totalNos = 0;
    arvore->totalRemovidos = 0;
    return arvore;
}


int getNumSetores(RadialTree T)
{
    struct radialtree *arvore = (struct radialtree *)T;
    return arvore->totalFilhos;
}

Node obterRaizArvore(RadialTree tree) 
{
    struct radialtree* arvore = (struct radialtree*)tree;
    return arvore->raiz;
}



void visitaProfundidadeRadialT(RadialTree t, FvisitaNo f, void *aux)
{
    struct radialtree *arvore = (struct radialtree *)t;
    struct node *node = arvore->raiz;
    if (node != NULL)
    {
        Fila fila = criaFila();
        enqueue(fila, node);
        while (!filaVazia(fila))
        {
            node = (struct node *)dequeue(fila);
            f(node->item, node->x, node->y, aux);
            for (int i = 0; i < arvore->totalFilhos; i++)
            {
                if (node->filhos[i] != NULL)
                    enqueue(fila, node->filhos[i]);
            }
        }
    }
}

void visitaLarguraRadialT(RadialTree t, FvisitaNo f, void *aux, int *count)
{
    struct radialtree *arvore = (struct radialtree *)t;
    struct node *node = arvore->raiz;
    
    if (node != NULL)
    {
        Fila fila = criaFila();
        enqueue(fila, node);

        while (!filaVazia(fila))
        {
            node = (struct node *)dequeue(fila);
            f(node->item, node->x, node->y, aux);
            if (count != NULL) 
            {
                (*count)++;
            }
            for (int i = 0; i < arvore->totalFilhos; i++)
            {
                if (node->filhos[i] != NULL)
                    enqueue(fila, node->filhos[i]);
            }
        }
    }
}



int checkSetor(Node n, double x, double y)
{
    struct node *node = (struct node *)n;

    // Calcula a inclinação da reta entre o nó e o ponto (x,y)
    double inclinacao = atan2(y - node->y, x - node->x) * 180 / M_PI;
    if (inclinacao < 0)
    {
        inclinacao += 360;
    }
    // Calcula o número do setor a partir da inclinação
    int aux = (node->arvoreAtual->totalFilhos);
    int setor = (int)(inclinacao / 360 * aux);
    // Retorna o número do setor
    //printf("Setor: %d\n", setor);
    return setor;
}

double calculaFd(RadialTree t)
{
    struct radialtree *arvore = (struct radialtree *)t;
    return (double)arvore->totalRemovidos / (double)arvore->totalNos;
}


Node _criaNode(double x, double y, Info i, RadialTree t)
{
    struct node *node = (struct node *)malloc(sizeof(struct node));
    struct radialtree *arvore = (struct radialtree *)t;
    node->item = i;
    node->x = x;
    node->y = y;
    node->excluido = false;
    node->arvoreAtual = t;
    node->pai = NULL;
    node->totalFilhosDoNo = 0;
    node->filhos = (struct node **)malloc(sizeof(struct node *) * arvore->totalFilhos);
    for (int j = 0; j < arvore->totalFilhos; j++)
    {
        node->filhos[j] = NULL;
    }
    return node;
}


 
Node insertRadialT(RadialTree t, double x, double y, Info i, int identificador)
{
    struct node *node = (struct node *)malloc(sizeof(struct node));
    struct radialtree *arvore = (struct radialtree *)t;
    node->x = x;
    node->y = y;
    node->excluido = false;
    node->arvoreAtual = t;
    node->item = i;
    node->id = identificador;
    node->filhos = (struct node **)malloc(sizeof(struct node *) * arvore->totalFilhos);
    
    for (int i = 0; i < arvore->totalFilhos; i++)
    {
        node->filhos[i] = NULL;
    }
    
    if (arvore->raiz == NULL)
    {
        arvore->raiz = node;
        arvore->totalNos++;
        return node;
    }
    
    int setorRaiz = checkSetor(arvore->raiz, x, y);
    struct node *current = arvore->raiz;
    
    while (current->filhos[setorRaiz] != NULL)
    {
        current = current->filhos[setorRaiz];
        setorRaiz = checkSetor(current, x, y);
    }
    
    current->filhos[setorRaiz] = node;
    node->pai = current;
    node->arvoreAtual = arvore;
    arvore->totalNos++;

    // Incrementa o totalFilhosDoNo do nó pai
    current->totalFilhosDoNo++;

    return node;
}




Node _getRecursivo(Node n, double x, double y, double epsilon, double minX, double minY, double maxX, double maxY)
{
    struct node *node = (struct node *)n;

    if (node == NULL || node->excluido)
        return NULL;

    double nodeX = node->x;
    double nodeY = node->y;

    // Verificar se o nó está dentro do retângulo de busca
    if (nodeX >= minX && nodeX <= maxX && nodeY >= minY && nodeY <= maxY)
    {
        // O nó está dentro do retângulo de busca, verificar se está dentro da região circular
        double distancia = sqrt(pow(nodeX - x, 2) + pow(nodeY - y, 2));
        if (distancia <= epsilon)
            return node;
    }

    // Verificar em qual setor o ponto de busca está em relação ao nó atual
    int setor = checkSetor(node, x, y);

    // Determinar quais setores devem ser explorados com base na interseção com o retângulo de busca
    int setorMinX = checkSetor(node, minX, nodeY);
    int setorMaxX = checkSetor(node, maxX, nodeY);
    int setorMinY = checkSetor(node, nodeX, minY);
    int setorMaxY = checkSetor(node, nodeX, maxY);

    // Verificar se o setor atual está dentro do retângulo de busca
    if (setor >= setorMinX && setor <= setorMaxX && setor >= setorMinY && setor <= setorMaxY)
    {
        // Explorar o setor atual
        struct node *encontrado = _getRecursivo(node->filhos[setor], x, y, epsilon, minX, minY, maxX, maxY);
        if (encontrado != NULL)
            return encontrado;
    }

    return NULL;
}




Node getNodeRadialT(RadialTree t, double x, double y, double epsilon)
{
    struct radialtree *arvore = (struct radialtree *)t;
    double minX = x - epsilon;
    double minY = y - epsilon;
    double maxX = x + epsilon;
    double maxY = y + epsilon;
    return _getRecursivo(arvore->raiz, x, y, epsilon, minX, minY, maxX, maxY);
}



void obterFilhosDoNo(Node n, Node** vetorDeFilhos)
{
    struct node* node = (struct node*)n;

    int totalFilhos = 0;
    struct node** filhos = node->filhos;

    for (int i = 0; i < node->totalFilhosDoNo; i++)
    {
        vetorDeFilhos[totalFilhos++] = (Node*)(*filhos);
        filhos++;
    }

    // Define as posições não preenchidas como NULL
    for (; totalFilhos < node->arvoreAtual->totalFilhos; totalFilhos++)
    {
        vetorDeFilhos[totalFilhos] = NULL;
    }
}




void _transferePraArvoreRecursivo(struct radialtree *old, struct radialtree *nova, Node n)
{
    struct node *node = (struct node *)n;
    if (node != NULL)
    {
        if (!node->excluido)
        {
            insertRadialT(nova, node->x, node->y, node->item, node->id);
        }
        for (int i = 0; i < old->totalFilhos; i++)
        {
            if (node->filhos[i] != NULL)
                _transferePraArvoreRecursivo(old, nova, node->filhos[i]);
        }
        free(node);
    }
}


// Função de comparação para calcular a distância euclidiana entre dois nós
struct ComparacaoDistanciaParams 
{
    double centerX;
    double centerY;
};

struct ComparacaoDistanciaParams parametros;

int compararDistanciaEuclidiana(const void *a, const void *b)
{
    struct node *noA = *(struct node **)a;
    struct node *noB = *(struct node **)b;

    double distanciaA = sqrt(pow(noA->x - parametros.centerX, 2) + pow(noA->y - parametros.centerY, 2));
    double distanciaB = sqrt(pow(noB->x - parametros.centerX, 2) + pow(noB->y - parametros.centerY, 2));

    if (distanciaA < distanciaB) {
        return -1;
    } else if (distanciaA > distanciaB) {
        return 1;
    } else {
        return 0;
    }
}

void _coletarNosValidos(struct node *node, struct node **nosValidos, int *contadorNosValidos)
{
    if (node != NULL)
    {
        if (!node->excluido)
        {
            nosValidos[(*contadorNosValidos)++] = node;
        }

        for (int i = 0; i < node->arvoreAtual->totalFilhos; i++)
        {
            _coletarNosValidos(node->filhos[i], nosValidos, contadorNosValidos);
        }
    }
}



void removeNoRadialT(RadialTree t, Node n)
{
    struct radialtree *arvore = (struct radialtree *)t;
    struct node *node = (struct node *)n;

    //Armazenando nós válidos em uma lista antes de remover o nó
    struct node **nosValidos = (struct node **)malloc(sizeof(struct node *) * arvore->totalNos);
    int contadorNosValidos = 0;

    //Percorrendo a árvore e coletando nós válidos
    _coletarNosValidos(arvore->raiz, nosValidos, &contadorNosValidos);

    //Calculando o centro do retângulo que envolve os nós válidos
    double minX = nosValidos[0]->x;
    double minY = nosValidos[0]->y;
    double maxX = nosValidos[0]->x;
    double maxY = nosValidos[0]->y;

    for (int i = 1; i < contadorNosValidos; i++) {
        double x = nosValidos[i]->x;
        double y = nosValidos[i]->y;

        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }

    double centerX = (minX + maxX) / 2.0;
    double centerY = (minY + maxY) / 2.0;
    parametros.centerX = centerX;
    parametros.centerY = centerY;


    //Ordenando a lista de nós válidos pela distância euclidiana em relação ao centro
    qsort(nosValidos, contadorNosValidos, sizeof(struct node *), compararDistanciaEuclidiana);

    //Removendo o nó especificado e atualizando a árvore
    node = (struct node *)_getRecursivo(arvore->raiz, node->x, node->y, 0.01, minX, minY, maxX, maxY);
    if (node != NULL) 
    {
        node->excluido = true;
        arvore->totalRemovidos++;
        arvore->fatorDegAtual = calculaFd(t);

        if (arvore->fatorDegAtual > arvore->fatorDegMaximo) 
        {
            struct radialtree *novaArvore = newRadialTree(arvore->totalFilhos, arvore->fatorDegMaximo);
            _transferePraArvoreRecursivo(arvore, novaArvore, arvore->raiz);
            arvore->raiz = novaArvore->raiz;
            arvore->totalNos = novaArvore->totalNos;
            arvore->totalRemovidos = novaArvore->totalRemovidos;
            arvore->fatorDegAtual = 0;
            free(novaArvore);
        }else
        {
            // Decrementando o totalFilhosDoNo do nó pai, caso exista
            if (node->pai != NULL)
                node->pai->totalFilhosDoNo--;
        }
    }

    //Criando uma nova árvore radial
    struct radialtree *novaArvore = newRadialTree(arvore->totalFilhos, arvore->fatorDegMaximo);

    //Percorrendo a lista ordenada de nós válidos e inserindo-os na nova árvore
    for (int i = 0; i < contadorNosValidos; i++) 
    {
        struct node *no = nosValidos[i];
        insertRadialT(novaArvore, no->x, no->y, no->item, no->id);
    }

    //Atualizando a árvore original com a nova árvore
    arvore->raiz = novaArvore->raiz;
    arvore->totalNos = novaArvore->totalNos;
    arvore->totalRemovidos = novaArvore->totalRemovidos;
    arvore->fatorDegAtual = novaArvore->fatorDegAtual;

    free(novaArvore);
    free(nosValidos);

}




Info getInfoRadialT(RadialTree t, Node n)
{
    struct node *node = (struct node *)n;
    return node->item;
}



bool getNodesDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2, Lista L)
{
    struct radialtree *arvore = (struct radialtree *)t;
    struct node *node = (struct node *)arvore->raiz;
    bool achou = false;
    if (node != NULL)
    {
        if (node->x >= x1 && node->x <= x2 && node->y >= y1 && node->y <= y2)
        {
            insertLst(L, node->item);
            achou = true;
        }
        double anguloinicial = atan2(y1 - node->y, x1 - node->x) * 180 / M_PI;
        double angulofinal = atan2(y2 - node->y, x2 - node->x) * 180 / M_PI;
        if (anguloinicial < 0) 
        {
            anguloinicial += 360;
        }

        if (angulofinal < 0) 
        {
            angulofinal += 360;
        }
        double incremento = 360 / arvore->totalFilhos;
        for (int i = 0; i < arvore->totalFilhos; i++)
        {
            double anguloFilho = incremento * i;

            // Verifica se o ângulo do filho está dentro do intervalo definido pelos ângulos inicial e final
            if (anguloinicial <= angulofinal)
            {
                if (anguloFilho >= anguloinicial && anguloFilho <= angulofinal)
                {
                    achou = achou || getNodesDentroRegiaoRadialT(t, x1, y1, x2, y2, L);
                }
            }
            else
            {
                if (anguloFilho >= anguloinicial || anguloFilho <= angulofinal)
                {
                    achou = achou || getNodesDentroRegiaoRadialT(t, x1, y1, x2, y2, L);
                }
            }
        }

    }
    return achou;
}


bool getInfosDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2,
                                 FdentroDeRegiao f, Lista L, Lista *formas)
{
    struct radialtree *arvore = (struct radialtree *)t;
    struct node *node = (struct node *)arvore->raiz;
    bool achou = false;
    
    
    if (node != NULL)  
    {  
        if (f(node->item, x1, y1, x2, y2, formas))
        {
            achou = true;
            insertLst(L, node->item);  
        }

        double anguloinicial = atan2(y1 - node->y, x1 - node->x) * 180 / M_PI;
        double angulofinal = atan2(y2 - node->y, x2 - node->x) * 180 / M_PI;
        double incremento = 360 / arvore->totalFilhos;

    }
    return achou;
}

bool getInfosAtingidoPontoRadialT(RadialTree t, double x, double y, FpontoInternoAInfo f, Lista L, Lista *formas)
{
    struct radialtree *arvore = (struct radialtree *)t;
    struct node *node = (struct node *)arvore->raiz;
    bool achou = false; 
    if (node != NULL)
    {
        if (f(node->item, x, y, *formas))
        {
            achou = true;
            insertLst(L, node->item);
            printf("Achei!\n");
        }
    }
    return achou;
}



void liberaNo(struct node *node) 
{
    if (node == NULL) {
        return;
    }
    
    for (int i = 0; i < node->arvoreAtual->totalFilhos; i++) {
        liberaNo(node->filhos[i]);
    }
    
    free(node->filhos); // Libera o array de filhos
    
    free(node); // Libera o nó em si
}

void liberaRadialT(RadialTree tree) 
{
    struct radialtree *arvore = (struct radialtree *)tree;
    if (arvore == NULL) {
        return;
    }
    
    liberaNo(arvore->raiz); // Libera os nós da árvore radial
    
    free(arvore); // Libera a estrutura da árvore radial em si
}





Item getItemByNode(Node node) 
{
    struct node *n = (struct node *)malloc(sizeof(struct node));

    if (node != NULL) 
    {
        return n->item;
    } else 
    {
        return NULL;
    }
}

bool getNodeExcluido(Node n)
{
    struct node *node = (struct node *)n;
    if(node->excluido == true)
    {
        return true;
    }else
    {
        return false;
    }
}

double getXdoNode(Node n)
{
    struct node *node = (struct node *)n;
    return node->x;
}

double getYdoNode(Node n)
{
    struct node *node = (struct node *)n;
    return node->y;
}

int getIDdoNode(Node n)
{
    struct node *node = (struct node *)n;
    return node->id;
}

Node getRaiz(RadialTree tree) 
{
    struct radialtree *arvore = (struct radialtree *)tree;
    if (arvore == NULL) 
    {
        return NULL;
    }
        
    return arvore->raiz; // Libera a estrutura da árvore radial em si
}




typedef void (*FsearchNo)(int identificador, double x, double y, void *aux);
void killRadialTreeRecursive(Node n);



void printDotRadialTreeRecursive(RadialTree tree, Node n, FILE *file) 
{
    struct node *node = (struct node *)n;
    struct radialtree *arvore = (struct radialtree *)tree;

    if (node != NULL)
    {
        Fila fila = criaFila();
        enqueue(fila, node);
        while (!filaVazia(fila))
        {
            node = (struct node *)dequeue(fila);

            bool hasChildren = false;
            for (int i = 0; i < arvore->totalFilhos; i++)
            {
                if (node->filhos[i] != NULL)
                {
                    if (hasChildren)
                        fprintf(file, " ");
                    fprintf(file, "\"%d\"", getIDdoNode(node->filhos[i]));
                    hasChildren = true;
                    
                    enqueue(fila, node->filhos[i]);
                }
            }
            
            // Verifica se o nó atual tem filhos e se não é o nó raiz
            if (hasChildren && node != arvore->raiz)
            {
                fprintf(file, "\"%d\" -> {", getIDdoNode(node));
                for (int i = 0; i < arvore->totalFilhos; i++)
                {
                    if (node->filhos[i] != NULL)
                    {
                        if (i > 0)
                            fprintf(file, " ");
                        fprintf(file, "\"%d\"", getIDdoNode(node->filhos[i]));
                    }
                }
                fprintf(file, "}\n");
            }
        }
    }
}



void killRadialTree(RadialTree tree) 
{
    struct radialtree *t = (struct radialtree *)tree;
    if (t == NULL || t->raiz == NULL) 
    {
        return;
    }

    killRadialTreeRecursive(t->raiz);
    free(t);
}

void killRadialTreeRecursive(Node n) 
{
    struct node *node = (struct node *)n;
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < node->arvoreAtual->totalFilhos; i++) 
    {
        killRadialTreeRecursive(node->filhos[i]);
    }

    free(node->filhos);
    free(node);
}


void procuraNode(RadialTree t, Item item)
{
    struct radialtree *arvore = (struct radialtree *)t;
    struct node *node = arvore->raiz;
    if (node != NULL)
    {
        Fila fila = criaFila();
        enqueue(fila, node);
        while (!filaVazia(fila))
        {
            node = (struct node *)dequeue(fila);
            Item nodeItem = getItemByNode(node);
            
            if (nodeItem == item)
            {
                if (!node->excluido)
                {
                    printf("Achei o nó: %p\n", node);
                }
            }
            
            for (int i = 0; i < arvore->totalFilhos; i++)
            {
                if (node->filhos[i] != NULL)
                    enqueue(fila, node->filhos[i]);
            }
        }
    }
}


Node procuraNoRadialT(RadialTree t, FsearchNo f, void *aux)
{
    struct radialtree *arvore = (struct radialtree *)t;
    struct node *node = arvore->raiz;
    
    if (node != NULL)
    {
        Fila fila = criaFila();
        enqueue(fila, node);

        while (!filaVazia(fila))
        {
            node = (struct node *)dequeue(fila);
            f(node->id, node->x, node->y, aux);
            
            for (int i = 0; i < arvore->totalFilhos; i++)
            {
                if (node->filhos[i] != NULL)
                    enqueue(fila, node->filhos[i]);
            }
        }
    }
}