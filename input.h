#ifndef __INPUT__
#define __INPUT__

#include "matrix.h"

typedef struct {
    _uint n; // Número de valores tabelados
    _uint m; // Número de funções sobre valores tabelados
    vector_double values; // Valores tabelados
    matrix_double func_values; // Funções sobre valores tabelados
} Input;


Input *read_input ();

void free_input(Input * inp);

#endif