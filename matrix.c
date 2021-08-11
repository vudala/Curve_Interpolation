#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "matrix.h"
#include "utils.h"

matrix_double new_matrix (_cuint m, _cuint n)
{
    matrix_double matrix = (matrix_double) malloc(sizeof(double*) * m);
    must_alloc(matrix, __func__);

    matrix[0] = (double*) calloc(m * n, sizeof(double));
    must_alloc(matrix[0], __func__);

    for (_uint i = 0; i < m; i++)
        matrix[i] = matrix[0] + i * n;

    return matrix;
}


vector_double new_vector (_cuint n)
{
    vector_double vector = malloc(sizeof(double) * n);
    must_alloc(vector, __func__);
    return vector;
}


matrix_double set_identity (matrix_double matrix, _uint n)
{
    for (int i = 0; i < n; i++)
        matrix[i][i] = 1;
    return matrix;
}


void print_matrix (matrix_double mat, _cuint m, _cuint n)
{
    _uint i, j;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
            printf("%.19f ", mat[i][j]);
        printf("\n");
    }
}

void print_vector (vector_double vec, _cuint n)
{
    _uint j;
    for (j = 0; j < n; j++)
        printf("%.19f ", vec[j]);
    printf("\n");
}


void free_matrix (matrix_double mat)
{
    free(mat[0]);
    free(mat);
}


matrix_double clone_matrix (matrix_double mat, _uint m, _uint n)
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