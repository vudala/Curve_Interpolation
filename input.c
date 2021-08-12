#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "input.h"
#include "matrix.h"
#include "utils.h"


Input * new_input (_cuint n, _cuint m)
{
    Input *new_input = malloc(sizeof(Input));
    must_alloc(new_input, __func__);

    new_input->n = n;
    new_input->m = m;
    new_input->values = new_vector(n);
    new_input->func_values = new_matrix(m, n);

    return new_input;
}


Input * read_input ()
{
    _uint n, m;
    if (scanf("%u %u\n", &n, &m));

    Input *input = new_input(n, m);

    _uint i, j;

    for (i = 0; i < n; i++)
        if (scanf("%Lf", &(input->values[i])));

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            if (scanf("%Lf", &(input->func_values[i][j])));

    return input;
}


void free_input(Input * inp)
{
    if (inp)
    {
        free(inp->values);
        free_matrix(inp->func_values);
        free(inp);
    }
}