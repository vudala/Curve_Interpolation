#ifndef __SYSTEM__
#define __SYSTEM__

#include "matrix.h"
#include "input.h"

typedef struct {
    _uint n; // Dimensão da matriz
    matrix_double A; // Matriz original A
    matrix_double L; // Matriz decomposta L
    matrix_double U; // Matriz decomposta U
    matrix_double B; // Matriz de termos independentes
} System;

// Libera o espaço de memória ocupado por um sistema
void free_system (System *sys);

System * setup_interpolation (const Input * restrict input);

// Resolve um sistema LU
void interpolation (System *sys, matrix_double result, _uint m);

System * setup_curve_adj (const Input * input);

void curve_adjustment (System * sys, Input * input, matrix_double results);

#endif