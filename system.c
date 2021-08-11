#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "system.h"
#include "matrix.h"
#include "utils.h"
#include "interpolation.h"

System *new_system (unsigned int n)
{
    System *new = (System*) malloc(sizeof(System));
    must_alloc(new, __func__);

    new->A = new_matrix(n, n);
    new->L = new_matrix(n, n);
    new->n = n;

    return new;
}

System * build_system (const Interpolation * restrict inter)
{   
    System * sys = new_system(inter->n);

    unsigned int i, j;
    for (i = 0; i < inter->n; i++)
        for (j = 0; j < inter->n; j++)
            sys->A[i][j] = pow(inter->values[i], j);

    sys->U = clone_matrix(sys->A, sys->n, sys->n);
    sys->B = clone_matrix(inter->func_values, inter->m, inter->n);

    triangularization(sys, 0);

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
void pivoting (const System * restrict sys)
{
    const unsigned int n = sys->n;
    const matrix_double L = sys->L;
    const matrix_double U = sys->U;
    const matrix_double B = sys->B;

    double *L_row_k = NULL;
    double *U_row_k = NULL;
    double *B_row_k = NULL;

    unsigned int i, k, c, max_index;
    double max, aux;

    for (k = 0; k < n - 1; k++)
    {
        max_index = k;
        max = fabs(U[k][k]);
        for (i = k + 1; i < n; i++)
            if (fabs(U[i][k]) > max)
                max_index = i;

        if (max_index != k)
        { // Se terminou em um índice diferente de onde começou, troca
            L_row_k = L[k];
            U_row_k = U[k];
            B_row_k = B[k];

            for (c = 0; c < n; c++)
            {
                aux = L_row_k[c];
                L_row_k[c] = L[max_index][c];
                L[max_index][c] = aux;

                aux = U_row_k[c];
                U_row_k[c] = U[max_index][c];
                U[max_index][c] = aux;
            }
        }
    }
}

/* 
Otimizações feitas:
- Uso de constantes em ponteiros e valores imutáveis ao longo do código
- Localização de variáveis para serem armazenadas em registrador
- Cálculos de índice são guardados em variáveis para não precisarem ser recalculados
- Uso de unsigned int ao invés de int
- Uso de restrict nos ponteiros
- Funções auxiliares também foram otimizadas
*/
void triangularization (const System * restrict sys, const unsigned int piv)
{
    const unsigned int n = sys->n;
    const matrix_double L = sys->L;
    const matrix_double U = sys->U;

    unsigned int i, k, j;
    double m;
    double *U_row_i = NULL;
    double *U_row_k = NULL;
    double element;

    for (k = 0; k < n - 1; k++)
    {
        U_row_k = U[k];
        element = U_row_k[k];
        
        if (piv)
            pivoting(sys);
            
        for (i = k + 1; i < n; i++)
        {
            U_row_i = U[i];

            m = U_row_i[k] / element;
            check_exception(m, __func__);

            L[i][k] = m;
            U_row_i[k] = 0.0f;

            for (j = k + 1; j < n; j++)
            {
                U_row_i[j] -= (m * U_row_k[j]);
                check_exception(U_row_i[j], __func__);
            }
        }
    }

    set_identity(L, n);
}


// Realiza a retrossubstituição para resolver um sistema
void retrosubs_downward (matrix_double A, double *x, double *b, unsigned int n)
{
    matrix_double clone = clone_matrix(A, n, n);

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
void retrosubs_upward (matrix_double A, double *x, double *b, unsigned int n)
{
    matrix_double clone = clone_matrix(A, n, n);

    for (int i = n - 1; i >= 0; i--)
    {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++)
            x[i] -= clone[i][j] * x[j];
        x[i] /= clone[i][i];
    }

    free_matrix(clone);
}


void solve_it (System *sys, matrix_double inverse, unsigned int m)
{
    double *column = NULL;

    double *y = malloc(sizeof(double) * sys->n);
    must_alloc(y, __func__);

    for (int i = 0; i < m; i++)
    {
        retrosubs_downward(sys->L, y, sys->B[i], sys->n);

        retrosubs_upward(sys->U, inverse[i], y, sys->n);
    }

    column = NULL;
}