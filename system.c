#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "system.h"
#include "matrix.h"
#include "utils.h"
#include "input.h"


System *new_system (_uint n)
{
    System *new = (System*) malloc(sizeof(System));
    must_alloc(new, __func__);

    new->A = new_matrix(n, n);
    new->L = new_matrix(n, n);
    new->U = NULL;
    new->B = NULL;
    new->n = n;

    return new;
}


void free_system (System * sys)
{
    if (sys)
    {
        free_matrix(sys->A);
        free_matrix(sys->L);
        free_matrix(sys->U);
        free_matrix(sys->B);
        free(sys);
    }
}

// Realiza o pivoteamente de um sistema linear e suas partes
void pivoting (const System * restrict sys)
{
    _cuint n = sys->n;
    const matrix_double A = sys->A;
    const matrix_double L = sys->L;
    const matrix_double U = sys->U;
    const matrix_double B = sys->B;

    vector_double A_row_k = NULL;
    vector_double L_row_k = NULL;
    vector_double U_row_k = NULL;
    vector_double B_row_k = NULL;

    _uint i, k, c, max_index;
    long double max, aux;

    for (k = 0; k < n - 1; k++)
    {
        max_index = k;
        max = fabs(A[k][k]);
        for (i = k + 1; i < n; i++)
            if (fabs(A[i][k]) > max)
                max_index = i;

        if (max_index != k)
        { // Se terminou em um índice diferente de onde começou, troca
            A_row_k = A[k];
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
- Uso de _uint e _cuint ao invés de int
- Uso de restrict nos ponteiros
- Funções auxiliares também foram otimizadas
*/
void triangularization (System * restrict sys)
{
    _cuint n = sys->n;
    sys->U = clone_matrix(sys->A, n, n);

    const matrix_double L = sys->L;
    const matrix_double U = sys->U;

    _uint i, k, j;
    long double m;
    vector_double U_row_i = NULL;
    vector_double U_row_k = NULL;
    long double element;

    for (k = 0; k < n - 1; k++)
    {
        //pivoting(sys);

        U_row_k = U[k];
        element = U_row_k[k];
            
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


// Realiza a retrossubstituição de cima para baixo para resolver um sistema
void retrosubs_downward (matrix_double A, vector_double x, vector_double b, _uint n)
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


// Realiza a retrossubstituição de baixo para cima para resolver um sistema
void retrosubs_upward (matrix_double A, vector_double x, vector_double b, _uint n)
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


// Realiza a retrossubstituição para resolver um sistema decomposto LU
void retrossubs (matrix_double L, matrix_double U, vector_double terms, vector_double result, _cuint n)
{
    vector_double aux = new_vector(n);

    retrosubs_downward(L, aux, terms, n);
    retrosubs_upward(U, result, aux, n);

    free(aux);
    aux = NULL;
}


System * setup_interpolation (const Input * restrict input)
{   
    System * sys = new_system(input->n);

    _uint i, j;
    for (i = 0; i < input->n; i++)
        for (j = 0; j < input->n; j++)
            sys->A[i][j] = powl(input->values[i], (long double) j);

    sys->B = clone_matrix(input->func_values, input->m, input->n);

    triangularization(sys);

    return sys;
}


// Resolve um sistema linear decomposto em LU
void interpolation (System *sys, matrix_double result, _uint m)
{
    for (_uint i = 0; i < m; i++)
        retrossubs(sys->L, sys->U, sys->B[i], result[i], sys->n);
}

// Calcula a matriz de coeficientes utilizada no ajuste de curvas
/* 
Otimizações feitas:
- O cálculo dos coeficientes de uma diagonal é feito apenas uma vez e distribuido ao longo da sua diagonal,
orientada da direita pra esquerda
- Uso de constantes em ponteiros e valores imutáveis ao longo do código
- Localização de variáveis para serem armazenadas em registrador
- Uso de _uint e _cuint ao invés de int
- Uso de restrict nos ponteiros
*/
void calc_coef (const matrix_double restrict coef, const Input * restrict input)
{
    _cuint degree = input->n, n = input->n;

    const vector_double values = input->values;
    long double sum;

    int k, i, j;

    // Calcula o valor do somatório da diagonal k e distribui o valor a longo da diagonal respectiva
    for (k = 0; k < degree; k++)
    {
        // Calcula o somatório
        sum = 0.0f;
        for (i = 0; i < n; i++)
            sum += pow(values[i], k);

        // Distribui o valor pela diagonal
        coef[0][k] = sum;
        for (j = k - 1, i = 1; j >= 0; i++, j--)
            coef[i][j] = sum;
    }

    // Calcula o valor do somatório da diagonal k e distribui o valor a longo da diagonal respectiva
    for (k = degree - 1; k > 0; k--)
    {
        // Calcula o somatório
        sum = 0.0f;
        for (i = 0; i < n; i++)
            sum += pow(values[i], k) * pow(values[i], degree - 1);

        // Distribui o valor pela diagonal
        coef[degree - 1][k] = sum;
        for (j = k + 1, i = degree - 2; j < degree; j++, i--)
            coef[i][j] = sum;
    }
}


// Transforma os valores do input em um sistema LU para ser resolvido
System * setup_curve_adj (const Input * input)
{
    _cuint degree = input->n;

    System * sys = new_system(degree);
    
    calc_coef(sys->A, input);

    sys->U = clone_matrix(sys->A, sys->n, sys->n);
    
    triangularization(sys);

    return sys;
}


// Calcula o vetor de termos independentes do ajuste de curvas
vector_double retrieve_terms (_cuint n, _cuint degree, vector_double values, vector_double funct_values)
{
    vector_double terms = new_vector(degree);

    long double sum;

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
void curve_adjustment (System * sys, Input * input, matrix_double results)
{
    _cuint degree = input->n;

    vector_double terms = NULL;

    _uint i;
    for (i = 0; i < input->m; i++)
    {
        terms = retrieve_terms(input->n, degree, input->values, input->func_values[i]);
        retrossubs(sys->L, sys->U, terms, results[i], degree);
        free(terms);
    }

    terms = NULL;
}