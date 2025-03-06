#ifndef _QUICK_SORT_H
#define _QUICK_SORT_H

#include <stdbool.h>

/*
 * Este contrato fornece operações básicas para implementar o algoritmo Quick Sort.
 *
 * O Quick Sort é um algoritmo de ordenação que divide uma lista em partes menores,
 * utilizando um pivô para reorganizar os elementos. Ele repete esse processo recursivamente
 * até que a lista esteja totalmente ordenada, proporcionando uma classificação rápida
 * na maioria dos casos.
 */

/**
 * Ordena um vetor de ponteiros de elementos utilizando o algoritmo Quick Sort.
 *
 * O parâmetro vetor é um vetor de ponteiros para os elementos a serem ordenados.
 * O parâmetro inicio é um número inteiro que representa o índice de início do intervalo de ordenação.
 * O parâmetro fim é um inteiro que representa o índice de fim do intervalo de ordenação.
 */
void quick_sort(void **vetor, int inicio, int fim);

#endif
