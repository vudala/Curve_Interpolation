#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "system.h"
#include "matrix.h"
#include "utils.h"
#include "interpolation.h"

int main(int argc, char **argv)
{
    Interpolation *inter = read_input();

    System * inter_system = build_system(inter);
    matrix_double inter_results = new_matrix(inter->m, inter->n);
    solve(inter_system, inter_results, inter->m);

    
    matrix_double adjus_results = new_matrix(inter->m, inter->n);
    curve_adjustment(inter, adjus_results);


    _uint i;
    for (i = 0; i < inter->m; i++)
    {
        print_vector(inter_results[i], inter->n);
        print_vector(adjus_results[i], inter->n);
    }

    return SUCCESS_STATUS_CODE;
}
