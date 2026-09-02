#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UTILS.h"

int main(int argc, char *argv[]) {
    FILE *sol_1 = fopen(argv[1], "r");
    int num_sol_1 = atoi(argv[2]);
    int *charges_1 = extract_charges(sol_1, num_sol_1);
    

    FILE *sol_2 = fopen(argv[3], "r");
    int num_sol_2 = atoi(argv[4]);
    int *charges_2 = extract_charges(sol_2, num_sol_2);

    FILE *duplicate_sol = fopen("./duplicates.txt", "w");
    int num_duplicates = 0;
    int *duplicate_charges = malloc(sizeof(int) * BUFFER_SIZE * 18);

    for (int i = 0; i < num_sol_2; i++) {
        int found_at = is_multiple(charges_1, charges_2 + 18*i, num_sol_1);
        if (found_at != -1) {
            num_duplicates++;
            add_charges(duplicate_charges, charges_2 + 18*i,
                        num_duplicates-1, BUFFER_SIZE * (1 + num_duplicates / BUFFER_SIZE));
            
            for (int j = 0; j < 18; j++) {
                fprintf(duplicate_sol, "  % d", *(duplicate_charges + 18*(num_duplicates-1)+j));
            }
            fprintf(duplicate_sol, "\n");
            
            printf("Solution %d in file 2 is a duplicate found at line %d of file 1\n", i+1, found_at+1);
        }
    }

    fclose(sol_1);
    fclose(sol_2);
    fclose(duplicate_sol);

    free(charges_1);
    free(charges_2);
    free(duplicate_charges);
}