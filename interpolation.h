#ifndef __INTERPOL__
#define __INTERPOL__

#include "matrix.h"

typedef struct {
    unsigned int n; // Número de valores tabelados
    unsigned int m; // Número de funções sobre valores tabelados
    double *values; // Valores tabelados
    matrix_double func_values; // Funções sobre valores tabelados
} Interpolation;


#define GX_DEGREE 3

void do_magic(Interpolation * inter, double * funct_values);
Interpolation *read_input ();

#endif