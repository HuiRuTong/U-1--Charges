#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int SM5NUSOL = 111152; 

int *quarter_srch(int *nums, int val, int low, int high) {
    /*
        Performs a quarternary search over a
        sorted arr to find the lower and
        upper indices that contain val

        A binary search would've been more
        straightforward but since the arr in
        question can be 10 bilion elements long,
        this is basically the only time I get
        to utilise this atrocious algorithm
    */
    int temp_high = high;   // To avoid having to pass len as an arg
    int *idx = malloc(sizeof(int) * 2);

    idx[0] = -1;
    idx[1] = -1;

    while (low <= high) {
        int q1 = (3*low + high) / 4;
        int q2 = (low + high) / 2;
        int q3 = (low + 3*high) / 4;

        if (nums[q1] == val) {
            idx[0] = q1;
            high = q1 - 1;
        } else if (nums[q2] == val) {
            idx[0] = q2;
            high = q2 - 1;
        } else if (nums[q3] == val) {
            idx[0] = q3;
            high = q3 - 1;
        } else {
            if (nums[q1] > val) {
                high = q1 - 1;
            } else if (nums[q2] > val) {
                low = q1 = 1;
                high = q2 - 1;
            } else if (nums[q3] > val) {
                low = q2 + 1;
                high = q3 - 1;
            } else {
                low = q3 + 1;
            }
        }
    }

    low = idx[0];
    high = temp_high;

    while (low <= high) {
        int q1 = (3*low + high) / 4;
        int q2 = (low + high) / 2;
        int q3 = (low + 3*high) / 4;

        if (nums[q1] == val) {
            idx[1] = q1;
            low = q1 + 1;
        } else if (nums[q2] == val) {
            idx[1] = q2;
            low = q2 + 1;
        } else if (nums[q3] == val) {
            idx[1] = q3;
            low = q3 + 1;
        } else {
            if (nums[q1] > val) {
                high = q1 - 1;
            } else if (nums[q2] > val) {
                low = q1 = 1;
                high = q2 - 1;
            } else if (nums[q3] > val) {
                low = q2 + 1;
                high = q3 - 1;
            } else {
                low = q3 + 1;
            }
        }
    }

    return idx;
}

int *extract_charges(FILE *charges, int num_sol) {
    int *all_charges = malloc(sizeof(int) * num_sol * 18);

    int i = 0;
    while (!feof(charges)) {
        for (int j = 0; j < 18; j++)
            fscanf(charges, " %d", all_charges + 18*i+j);
        i++;
    }
    return all_charges;
}

static void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(int argc, char *argv[]) {
    FILE *complete_sol = fopen(argv[1], "r");
    int num_complete_sol = atoi(argv[2]);
    int *complete_charges = extract_charges(complete_sol, num_complete_sol);

    FILE *found_sol = fopen(argv[3], "r");
    int num_found_sol = atoi(argv[4]);
    int *found_charges = extract_charges(found_sol, num_found_sol);

    int *idx_bounds;
    int low = 0;
    int high = num_complete_sol - 1;

    int *invalid_charges = malloc(sizeof(int) * 5 * 18);
    /*
    for (int i = 0; i < num_found_sol*18; i++) {
        printf("%d ", *(found_charges+i));
        if (i && (i % 18 == 17)) {
            printf("\n");
        }
    }
    */

    // Sorts the found solutions in increasing order
    for (int i = 0; i < num_found_sol*18; i+=3) {
        if (found_charges[i] > found_charges[i+1]) {
            swap(found_charges + i, found_charges + i+1);
        }
        if (found_charges[i+1] > found_charges[i+2]) {
            swap(found_charges+ i+1, found_charges + i+2);
            if (found_charges[i] > found_charges[i+1]) {
                swap(found_charges + i, found_charges+ i+1);
            }
        }
    }

    for (int i = 0; i < num_complete_sol; i) {
        for (int j = 0; j < 18; j++) {
            idx_bounds = quarter_srch(complete_charges, found_charges[18*i+j], low, high);

            if (idx_bounds[0] == idx_bounds[1]) {
                break;
            }
        }
    }

    fclose(complete_sol);
    fclose(found_sol);

    free(complete_charges);
    free(found_charges);
    free(invalid_charges);
}