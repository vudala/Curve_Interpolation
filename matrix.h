#ifndef __MATRIX__
#define __MATRIX__

typedef double** matrix_double;

// Inicializa uma matrix
matrix_double new_matrix (unsigned int m, unsigned int n);

// Escreve 1's na diagonal principal da matriz
matrix_double set_identity (matrix_double matrix, unsigned int n);

// Imprime a matriz em stdout
void print_matrix (matrix_double mat, const unsigned int m, const unsigned int n);

// Libera o espaço de memória ocupado por uma matriz
void free_matrix (matrix_double mat);

// Cria uma cópia de uma matriz
matrix_double clone_matrix (matrix_double mat, unsigned int m, unsigned int n);

#endif