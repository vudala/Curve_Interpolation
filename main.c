#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "system.h"
#include "matrix.h"
#include "utils.h"
#include "input.h"

int main(int argc, char **argv)
{
    Input * input = read_input();

    // Transforma o input em um sistema LU para a interpolação
    System * inter_system = setup_interpolation(input);

    // Matriz para armazenar os resultados da interpolação
    matrix_double inter_results = new_matrix(input->m, input->n);

    // Resolve os sistemas da fatoração LU para realizar a interpolação polinomial
    interpolation(inter_system, inter_results, input->m);

    // Transforma o input em um sistema LU para o ajuste de curvas
    System * adjus_system = setup_curve_adj(input);

    // Matriz para armazenar os resultados do ajuste de curvas
    matrix_double adjus_results = new_matrix(input->m, input->n);

    // Resolve os sistemas da fatoração LU para realizar o ajuste de curvas
    curve_adjustment(adjus_system, input, adjus_results);

    _uint i;
    for (i = 0; i < input->m; i++)
    {
        print_vector(inter_results[i], input->n);
        print_vector(adjus_results[i], input->n);
    }

    return SUCCESS_STATUS_CODE;
}
