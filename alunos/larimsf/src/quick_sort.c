#include <stdio.h>

void troca(void **a, void **b) 
{
    void *aux = *a;
    *a = *b;
    *b = aux;
}


int divide_vetor_pivo(void **vetor, int inicio, int fim)
{
    int pivo = *(int *)vetor[inicio]; // Convertendo o ponteiro void para int
    int i = inicio + 1;
    int j = fim;

    while (i <= j)
    {
        if (*(int *)vetor[i] <= pivo) // Convertendo o ponteiro void para int
        {
            i++;
        }
        else if (*(int *)vetor[j] > pivo) // Convertendo o ponteiro void para int
        {
            j--;
        }
        else if (i <= j)
        {
            troca(&vetor[i], &vetor[j]);
            i++;
            j--;
        }
    }

    troca(&vetor[inicio], &vetor[j]);
    return j;
}


void quick_sort(void **vetor, int inicio, int fim)
{
    if (inicio < fim) 
    {
        int j = divide_vetor_pivo(vetor, inicio, fim);
        quick_sort(vetor, inicio, j - 1);
        quick_sort(vetor, j + 1, fim);
    }
}


