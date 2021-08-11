#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "system.h"
#include "matrix.h"
#include "utils.h"
#include "interpolation.h"

#define PIVOTING 1



int main(int argc, char **argv)
{
    Interpolation *inter = read_input();

    System * sys = build_system(inter);
    
    matrix_double result = new_matrix(inter->m, inter->n);
    solve_it(sys, result, inter->m);

    print_matrix(result, inter->m, inter->n);

    return SUCCESS_STATUS_CODE;
}
