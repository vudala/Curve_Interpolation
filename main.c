#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "system.h"
#include "matrix.h"
#include "utils.h"

int main(int argc, char **argv)
{
    FILE *output_file = stdout;

    int opt, pivoting = 0;

    while ((opt = getopt(argc, argv, "po:")) != -1)
        switch (opt)
        {
            case 'o':
                output_file = fopen(optarg, "w");
                break;
            case 'p':
                pivoting = 1;
                break;
        }

    while (!feof(stdin))
    {
        System *sys = read_system();

        // Separa o sistema em L e U
        double time_tri = triangularization(sys, pivoting);
        
        float **inverse = new_matrix(sys->n);

        // Inverte a matriz e registra o tempo tomado
        double time_y = 0.0f, time_x = 0.0f;
        invert(sys, inverse, &time_y, &time_x);
        
        // Imprime os resultados
        print_result(output_file, sys, inverse, time_tri, time_y, time_x);

        free_system(sys);
        free_matrix(inverse);

        sys = NULL;
        inverse = NULL;
    }

    fclose(output_file);

    return SUCCESS_STATUS_CODE;
}
