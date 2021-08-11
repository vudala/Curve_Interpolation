#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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


void do_magic(Interpolation * inter, double * funct_values)
{
    unsigned int degree = 3;
    matrix_double coef = new_matrix(degree, degree);
    int k, i, j;
    double element = 0;
    double sum;
    
    // Calcula o valor do somatório da diagonal k e distribui o valor a longo da diagonal
    for (k = 0; k < degree; k++)
    {
        
        sum = 0.0f; 
        for (i = 0; i < inter->n; i++)
            sum += pow(inter->values[i], k);

        coef[0][k] = sum;
        for (j = k - 1, i = 1; j >= 0; i++, j--)
            coef[i][j] = sum;
    }

    // Calcula o valor do somatório da diagonal k e distribui o valor a longo da diagonal
    for (k = degree - 1; k > 0; k--)
    {
        sum = 0.0f;
        for (i = 0; i < inter->n; i++)
            sum += pow(inter->values[i], k) * pow(inter->values[i], degree - 1);;
        coef[degree - 1][k] = sum;

        for (j = k + 1, i = degree - 2; j < degree; j++, i--)
            coef[i][j] = sum;
    }

    double * terms = malloc(sizeof(double) * degree);
    must_alloc(terms, __func__);

    for (j = 0; j < degree; j++)
    {
        sum = 0.0f;
        for (i = 0; i < inter->n; i++)
            sum += funct_values[i] * pow(inter->values[i], j);
        terms[j] = sum;
    }
}