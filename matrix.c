#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "matrix.h"
#include "utils.h"

float **new_matrix (unsigned int n)
{
    float **new = (float**) malloc(sizeof(float*) * n);
    must_alloc(new, __func__);

    new[0] = (float*) calloc(n * n, sizeof(float));
    must_alloc(new[0], __func__);

    for (int i = 0; i < n; i++)
        new[i] = new[0] + i * n;

    return new;
}


float **set_identity (float **matrix, unsigned int n)
{
    for (int i = 0; i < n; i++)
        matrix[i][i] = 1;
    return matrix;
}


void print_matrix (FILE *output_file, float **m, unsigned int n, unsigned int inverse)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
            if (inverse)
                fprintf(output_file, "%f ", m[j][i]);
            else
                fprintf(output_file, "%f ", m[i][j]);
        fprintf(output_file, "\n");
    }
}


void free_matrix (float **mat)
{
    free(mat[0]);
    free(mat);
}


float **clone_matrix (float **m, unsigned int n)
{   
    float **clone = (float**) malloc(sizeof(float*) * n);
    must_alloc(clone, __func__);
    
    clone[0] = malloc(sizeof(float) * n * n);
    must_alloc(clone[0], __func__);

    for (int i = 0; i < n; i++)
        clone[i] = clone[0] + i * n;

    memcpy(clone[0], m[0], sizeof(float) * n * n);

    return clone;
}