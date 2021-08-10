#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "system.h"
#include "matrix.h"
#include "utils.h"

System *new_system (unsigned int n)
{
    System *new = (System*) malloc(sizeof(System));
    must_alloc(new, __func__);

    new->L = new_matrix(n);
    new->A = new_matrix(n);
    new->B = set_identity(new_matrix(n), n);
    new->n = n;

    return new;
}


System *read_system ()
{
    unsigned int n;
    fscanf(stdin, "%u\n", &n);

    System *sys = new_system(n);
    sys->n = n;

    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            fscanf(stdin, "%f", &(sys->A[i][j]));

    sys->U = clone_matrix(sys->A, sys->n);

    return sys;
}


void free_system (System *sys)
{
    free_matrix(sys->A);
    free_matrix(sys->L);
    free_matrix(sys->U);
    free_matrix(sys->B);
    free(sys);
}


// Realiza o pivoteamente de um sistema linear e suas partes
void pivoting (float **A, float *b, float **B, float **L, unsigned int n)
{
    float max, aux;
    unsigned int max_index;
    int i, k;

    for (k = 0; k < n - 1; k++)
    {
        max_index = k;
        max = fabs(A[k][k]);
        for (i = k + 1; i < n; i++)
            if (fabs(A[i][k]) > max)
                max_index = i;

        if (max_index != k)
        { // Se terminou em um índice diferente de onde começou, troca
            for (int c = 0; c < n; c++)
            {
                aux = A[k][c];
                A[k][c] = A[max_index][c];
                A[max_index][c] = aux;
                
                if (L && B)
                {
                    aux = L[k][c];
                    L[k][c] = L[max_index][c];
                    L[max_index][c] = aux;

                    aux = B[k][c];
                    B[k][c] = B[max_index][c];
                    B[max_index][c] = aux;
                }
            }

            if (b)
            {
                aux = b[k];
                b[k] = b[max_index];
                b[max_index] = aux;
            }
        }
    }
}


double triangularization (System *sys, unsigned int piv)
{
    double time_tri = timestamp();
    int i, k, j;
    double m;

    for (k = 0; k < sys->n - 1; k++)
    {
        if (piv)
            pivoting(sys->U, NULL, sys->B, sys->L, sys->n);
            
        for (i = k + 1; i < sys->n; i++)
        {
            m = sys->U[i][k] / sys->U[k][k];
            check_exception(m, __func__);

            sys->L[i][k] = m;
            sys->U[i][k] = 0.0f;

            for (j = k + 1; j < sys->n; j++)
            {
                sys->U[i][j] -= (m * sys->U[k][j]);
                check_exception(sys->U[i][j], __func__);
            }
        }
    }

    set_identity(sys->L, sys->n);

    return timestamp() - time_tri;
}


// Realiza a retrossubstituição para resolver um sistema
void retrosubs_downward (float **A, float *x, float *b, unsigned int n)
{
    float **clone = clone_matrix(A, n);

    for (int i = 0; i < n; i++)
    {
        x[i] = b[i];
        for (int j = 0; j < i; j++)
            x[i] -= clone[i][j] * x[j];
        x[i] /= clone[i][i];
    }

    free_matrix(clone);
}

// Realiza a retrossubstituição para resolver um sistema
void retrosubs_upward (float **A, float *x, float *b, unsigned int n)
{
    float **clone = clone_matrix(A, n);

    for (int i = n - 1; i >= 0; i--)
    {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++)
            x[i] -= clone[i][j] * x[j];
        x[i] /= clone[i][i];
    }

    free_matrix(clone);
}


float *get_column(float **mat, unsigned int n, unsigned int col)
{
    float *column = malloc(sizeof(float) * n);
    must_alloc(column, __func__);

    for (int i = 0; i < n; i++)
        column[i] = mat[i][col];

    return column;
}


void invert (System *sys, float **x, double *x_total_time, double *y_total_time)
{
    double time_y, time_x;
    float *column = NULL;

    float *y = malloc(sizeof(float) * sys->n);
    must_alloc(y, __func__);

    for (int i = 0; i < sys->n; i++)
    {
        time_y = timestamp();

        column = get_column(sys->B, sys->n, i);
        retrosubs_downward(sys->L, y, column, sys->n);
        free(column);
        time_y = timestamp() - time_y;
        
        time_x = timestamp();
        retrosubs_upward(sys->U, x[i], y, sys->n);
        time_x = timestamp() - time_x;

        *y_total_time += time_y;
        *x_total_time += time_x;
    }

    *y_total_time /= sys->n;
    *x_total_time /= sys->n;

    column = NULL;
}


// Calcula o resíduo de um sistema linear e sua solução
float *residue (System *sys, float *b, float *x)
{
    float *res = malloc(sys->n * sizeof(float));
    must_alloc(res, __func__);

    int i, k;
    double ax;

    for (i = 0; i < sys->n; i++)
    {
        ax = 0.0f;
        for (k = 0; k < sys->n; k++)
        {
            ax += sys->A[i][k] * x[k];
            check_exception(ax, __func__);
        }
        res[i] = b[i] - ax;
    }
    
    return res;
}


// Calcula a norma do resíduo
float residue_norm (float *res, unsigned int n)
{
    float sum = 0.0f;

    for (int i = 0; i < n; i++)
        sum += pow(res[i], 2.0f);

    return sqrt(sum);
}


// Imprime as normas dos residuos
void print_residues (FILE *output_file, System *sys, float **x)
{
    float norm;
    float *res = NULL;

    float **identity = set_identity(new_matrix(sys->n) , sys->n);

    for (int i = 0; i < sys->n; i++)
    {
        res = residue(sys, identity[i], x[i]);
        norm = residue_norm(res, sys->n);
        
        free(res);
        fprintf(output_file, "%f ", norm);
    }

    fprintf(output_file, "\n");

    free_matrix(identity);
    res = NULL;
}


void print_result (FILE *output_file, System *sys, float **inverse, double time_tri, double time_y, double time_x)
{
    fprintf(output_file, "%u\n", sys->n);
    print_matrix(output_file, sys->A, sys->n, 0);

    fprintf(output_file, "#\n");

    print_matrix(output_file, inverse, sys->n, 1);

    fprintf(output_file, "###########\n");

    fprintf(output_file, "# Tempo Triangularização: %1.9f\n", time_tri);
    fprintf(output_file, "# Tempo cálculo de Y: %1.9f\n", time_y);
    fprintf(output_file, "# Tempo cálculo de X: %1.9f\n", time_x);
    fprintf(output_file, "# Norma L2 do Residuo: ");
    print_residues(output_file, sys, inverse);

    fprintf(output_file, "###########\n");
}
