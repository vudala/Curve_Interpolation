#ifndef __INPUT__
#define __INPUT__

#include "matrix.h"

typedef struct {
    _uint n; // Número de valores tabelados
    _uint m; // Número de funções sobre valores tabelados
    vector_double values; // Valores tabelados
    matrix_double func_values; // Funções sobre valores tabelados
} Input;

// Lê a entrada
Input *read_input ();

// Libera o espaço de memória ocupado por uma estrutura Input
void free_input(Input * inp);

#endif