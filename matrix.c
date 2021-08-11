#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "matrix.h"
#include "utils.h"

matrix_double new_matrix (unsigned int m, unsigned int n)
{
    matrix_double new = (matrix_double) malloc(sizeof(double*) * m);
    must_alloc(new, __func__);

    new[0] = (double*) calloc(m * n, sizeof(double));
    must_alloc(new[0], __func__);

    for (unsigned int i = 0; i < m; i++)
        new[i] = new[0] + i * n;

    return new;
}


matrix_double set_identity (matrix_double matrix, unsigned int n)
{
    for (int i = 0; i < n; i++)
        matrix[i][i] = 1;
    return matrix;
}


void print_matrix (matrix_double mat, const unsigned int m, const unsigned int n)
{
    unsigned int i, j;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
            printf("%.19f ", mat[i][j]);
        printf("\n");
    }
}


void free_matrix (matrix_double mat)
{
    free(mat[0]);
    free(mat);
}


matrix_double clone_matrix (matrix_double mat, unsigned int m, unsigned int n)
{   
    matrix_double clone = (matrix_double) malloc(sizeof(double*) * m);
    must_alloc(clone, __func__);
    
    clone[0] = malloc(sizeof(double) * m * n);
    must_alloc(clone[0], __func__);

    for (int i = 0; i < m; i++)
        clone[i] = clone[0] + i * n;

    memcpy(clone[0], mat[0], sizeof(double) * m * n);

    return clone;
}