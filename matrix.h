#ifndef __MATRIX__
#define __MATRIX__

typedef long double *  vector_double;
typedef long double ** matrix_double;

#include "utils.h"

// Inicializa uma matrix
matrix_double new_matrix (_cuint m, _cuint n);

// Inicializa um vetor
vector_double new_vector (_cuint n);

// Escreve 1's na diagonal principal da matriz
matrix_double set_identity (matrix_double matrix, _uint n);

// Imprime a matriz em stdout
void print_matrix (matrix_double mat, _cuint m, _cuint n);

// Imprime um vetor em stdout
void print_vector (vector_double vec, _cuint n);

// Libera o espaço de memória ocupado por uma matriz
void free_matrix (matrix_double mat);

// Cria uma cópia de uma matriz
matrix_double clone_matrix (matrix_double mat, _uint m, _uint n);

#endif