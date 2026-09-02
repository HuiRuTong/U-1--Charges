#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UTILS.h"

int main(int argc, char *argv[]) {
    FILE *complete_sol = fopen(argv[1], "r");
    int num_complete_sol = atoi(argv[2]);
    int *complete_charges = extract_charges(complete_sol, num_complete_sol);

    FILE *found_sol = fopen(argv[3], "r");
    int num_found_sol = atoi(argv[4]);
    int *found_charges = extract_charges(found_sol, num_found_sol);

    FILE *missing_sol = fopen("./missing_charges.txt", "w");

    int low = 0;
    int upp = num_complete_sol - 1;
    int *missing_charges = malloc(sizeof(int) * BUFFER_SIZE * 18);
    int num_missing = 0;

    sort(found_charges, num_found_sol);
    for (int i = 0; i < num_found_sol; i++) {
        printf("Found solution %d ", i+1);

        int found_at = srch(complete_charges, found_charges + 18*i, num_complete_sol);
        if (found_at == -1) {
            num_missing++;
            add_charges(missing_charges, found_charges + 18*i,
                        num_missing-1, BUFFER_SIZE * (1 + num_missing / BUFFER_SIZE));

            for (int j = 0; j < 18; j++) {
                fprintf(missing_sol, "  % d", *(missing_charges + 18*(num_missing-1)+j));
            }
            fprintf(missing_sol, "\n");

            printf("is nowhere to be seen??\n");
        } else {
            printf("is on line %d\n", found_at+1);
        }
    }

    fclose(complete_sol);
    fclose(found_sol);
    fclose(missing_sol);

    free(complete_charges);
    free(found_charges);
    free(missing_charges);
}