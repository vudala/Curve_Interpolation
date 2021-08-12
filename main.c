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

      ///////////////////////////////////////////////////////
     //                     Interpolação                  //
    ///////////////////////////////////////////////////////

    // Transforma o input em um sistema LU para a interpolação
    System * inter_system = setup_interpolation(input);

    //print_matrix(inter_system->A, input->n, input->n);

    //print_matrix(inter_system->A, inter_system->n, inter_system->n);

    // Matriz para armazenar os resultados da interpolação
    matrix_double inter_results = new_matrix(input->m, input->n);

    // Calcula a interpolação através da fatoração LU
    interpolation(inter_system, inter_results, input->m);


      ///////////////////////////////////////////////////////
     //                  Ajuste de curvas                 //
    ///////////////////////////////////////////////////////

    // Transforma o input em um sistema LU para o ajuste de curvas
    System * adjus_system = setup_curve_adj(input);

    // Matriz para armazenar os resultados do ajuste de curvas
    matrix_double adjus_results = new_matrix(input->m, input->n);

    // Calcula o ajuste de curvas através da fatoração LU
    curve_adjustment(adjus_system, input, adjus_results);


      ///////////////////////////////////////////////////////
     //                    Resultados                     //
    ///////////////////////////////////////////////////////

    _uint i;
    for (i = 0; i < input->m; i++)
    {
        print_vector(inter_results[i], input->n);
        print_vector(adjus_results[i], input->n);
    }

    free_system(inter_system);
    free_system(adjus_system);
    free_input(input);

    return SUCCESS_STATUS_CODE;
}
