#ifndef __INTERPOL__
#define __INTERPOL__

#include "matrix.h"

typedef struct {
    _uint n; // Número de valores tabelados
    _uint m; // Número de funções sobre valores tabelados
    double *values; // Valores tabelados
    matrix_double func_values; // Funções sobre valores tabelados
} Interpolation;


void curve_adjustment (Interpolation * inter, matrix_double results);

Interpolation *read_input ();

#endif