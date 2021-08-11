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

    do_magic(inter, inter->func_values[0]);
    // System * sys = build_system(inter);
    
    // matrix_double result = new_matrix(inter->m, inter->n);
    // solve_it(sys, result, inter->m);

    // print_matrix(result, inter->m, inter->n);

    return SUCCESS_STATUS_CODE;
}

// 11 1
// 1872 1890 1900 1920 1940 1950 1960 1970 1980 1991 1996
// 9.9 14.3 17.4 30.6 41.2 51.9 70.2 93.1 119.0 141.2 157.1
