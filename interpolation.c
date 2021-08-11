#include <stdio.h>
#include <stdlib.h>

#include "interpolation.h"
#include "matrix.h"
#include "utils.h"


Interpolation *new_interpol(const unsigned int n, const unsigned int m)
{
    Interpolation *new_inter = malloc(sizeof(Interpolation));
    must_alloc(new_inter, __func__);

    new_inter->n = n;
    new_inter->m = m;

    new_inter->values = malloc(sizeof(double) * n);
    must_alloc(new_inter->values, __func__);

    new_inter->func_values = new_matrix(m, n);

    return new_inter;
}

Interpolation *read_input ()
{
    unsigned int n, m;
    fscanf(stdin, "%u %u\n", &n, &m);

    Interpolation *inter = new_interpol(n, m);

    unsigned int i, j;

    for (i = 0; i < n; i++)
        fscanf(stdin, "%lf", &(inter->values[i]));

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            fscanf(stdin, "%lf", &(inter->func_values[i][j]));

    return inter;
}