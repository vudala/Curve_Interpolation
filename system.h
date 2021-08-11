#ifndef __SYSTEM__
#define __SYSTEM__

#include "matrix.h"
#include "interpolation.h"

typedef struct {
    unsigned int n; // Dimensão da matriz
    matrix_double A; // Matriz original A
    matrix_double L; // Matriz decomposta L
    matrix_double U; // Matriz decomposta U
    matrix_double B; // Matriz identidade
} System;

// Inicializa um novo sistema
System *new_system (unsigned int n);

// Lê um sistema
System *read_system ();

// Libera o espaço de memória ocupado por um sistema
void free_system (System *sys);

// Realiza a triangularização
void triangularization (const System * restrict sys, const unsigned int piv);

// Calcula a matriz inversa do sistema
void solve_it (System *sys, matrix_double inverse, unsigned int m);


System * build_system (const Interpolation * restrict inter);

#endif