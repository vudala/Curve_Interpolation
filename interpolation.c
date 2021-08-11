#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "interpolation.h"
#include "matrix.h"
#include "utils.h"
#include "system.h"


Interpolation * new_interpol (_cuint n, _cuint m)
{
    Interpolation *new_inter = malloc(sizeof(Interpolation));
    must_alloc(new_inter, __func__);

    new_inter->n = n;
    new_inter->m = m;

    new_inter->values = new_vector(n);

    new_inter->func_values = new_matrix(m, n);

    return new_inter;
}


Interpolation * read_input ()
{
    _uint n, m;
    fscanf(stdin, "%u %u\n", &n, &m);

    Interpolation *inter = new_interpol(n, m);

    _uint i, j;

    for (i = 0; i < n; i++)
        fscanf(stdin, "%lf", &(inter->values[i]));

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            fscanf(stdin, "%lf", &(inter->func_values[i][j]));

    return inter;
}


// Transforma os valores da interpolação em um sistema LU para ser resolvido, em prol do ajuste de curvas
System * setup_curve_adj (Interpolation * inter)
{
    _cuint degree = inter->n;

    System * sys = new_system(degree);

    int k, i, j;
    double element, sum;
    
    // Calcula o valor do somatório da diagonal k e distribui o valor a longo da diagonal
    for (k = 0; k < degree; k++)
    {
        // Calcula o somatório
        sum = 0.0f; 
        for (i = 0; i < inter->n; i++)
            sum += pow(inter->values[i], k);

        
        sys->A[0][k] = sum;
        for (j = k - 1, i = 1; j >= 0; i++, j--)
            sys->A[i][j] = sum;
    }

    // Calcula o valor do somatório da diagonal k e distribui o valor a longo da diagonal
    for (k = degree - 1; k > 0; k--)
    {
        // Calcula o somatório
        sum = 0.0f;
        for (i = 0; i < inter->n; i++)
            sum += pow(inter->values[i], k) * pow(inter->values[i], degree - 1);

        // Distribui o valor pela diagonal
        sys->A[degree - 1][k] = sum;
        for (j = k + 1, i = degree - 2; j < degree; j++, i--)
            sys->A[i][j] = sum;
    }

    sys->U = clone_matrix(sys->A, sys->n, sys->n);
    
    triangularization(sys, 0);

    return sys;
}


// Calcula o vetor de termos independentes do ajuste de curvas
vector_double retrieve_terms (_cuint n, _cuint degree, vector_double values, vector_double funct_values)
{
    vector_double terms = new_vector(degree);

    double sum;

    _uint i, j;
    for (j = 0; j < degree; j++)
    {
        sum = 0.0f;
        for (i = 0; i < n; i++)
            sum += funct_values[i] * pow(values[i], j);
        terms[j] = sum;
    }

    return terms;
}


// Resolve os sistemas lineares do ajuste de curvas
void curve_adjustment (Interpolation * inter, matrix_double results)
{
    System * curve_system = setup_curve_adj(inter);

    _cuint degree = inter->n;

    vector_double terms = NULL;
    
    _uint i;
    for (i = 0; i < inter->m; i++)
    {
        terms = retrieve_terms(inter->n, degree, inter->values, inter->func_values[i]);
        retrossubs(curve_system->L, curve_system->U, terms, results[i], degree);
    }
}