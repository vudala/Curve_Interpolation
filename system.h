#ifndef __SYSTEM__
#define __SYSTEM__

#include "matrix.h"
#include "interpolation.h"

typedef struct {
    _uint n; // Dimensão da matriz
    matrix_double A; // Matriz original A
    matrix_double L; // Matriz decomposta L
    matrix_double U; // Matriz decomposta U
    matrix_double B; // Matriz identidade
} System;

// Inicializa um novo sistema
System *new_system (_uint n);

// Lê um sistema
System *read_system ();

// Libera o espaço de memória ocupado por um sistema
void free_system (System *sys);

// Realiza a triangularização
void triangularization (const System * restrict sys, _cuint piv);

// Resolve um sistema LU
void solve (System *sys, matrix_double result, _uint m);

void retrossubs (matrix_double L, matrix_double U, vector_double terms, vector_double result, _cuint n);

#endif