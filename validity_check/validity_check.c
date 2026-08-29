#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 5
#define SMNUR_1_SIZE 38
#define SMNUR_5_SIZE 111152
#define SMNUR_10_SIZE 21546920

int srch_sol(int *complete_charges, int *found_charges, int num_complete_sol) {
    /*
        On second thought, having to constantly rearrange
        the rows for a binary-esque search is kinda annoying
        to implement so I've opted to just go back to linear
        searches :(
    */
    for (int i = 0; i < num_complete_sol; i++) {
        if (!memcmp(complete_charges + 18*i, found_charges, 18*sizeof(int))) {
            printf("is on line %d\n", i+1);
            return 1;
        }
    }
    return 0;
}

int *extract_charges(FILE *charges, int num_sol) {
    int *all_charges = malloc(sizeof(int) * num_sol * 18);

    for (int i = 0; i < num_sol; i++) {
        for (int j = 0; j < 18; j++) {
            fscanf(charges, " %d", all_charges + 18*i+j);
        }
    }
    return all_charges;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void add_charges(int *a, int *b, int pos, int len) {
    // Reserve more space if needed
    if (pos >= len) {
        a = realloc(a, sizeof(int) * (len + BUFFER_SIZE));
    }

    for (int i = 0; i < 18; i++) {
        a[18*pos + i] = b[i];
    }
}

int main(int argc, char *argv[]) {
    FILE *complete_sol = fopen(argv[1], "r");
    int num_complete_sol = atoi(argv[2]);
    int *complete_charges = extract_charges(complete_sol, num_complete_sol);

    FILE *found_sol = fopen(argv[3], "r");
    int num_found_sol = atoi(argv[4]);
    int *found_charges = extract_charges(found_sol, num_found_sol);

    int low = 0;
    int upp = num_complete_sol - 1;
    int *missing_charges = malloc(sizeof(int) * BUFFER_SIZE * 18);
    int num_missing_charges = 0;

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

    for (int i = 0; i < num_found_sol; i++) {
        printf("Found solution %d ", i);
        if (!srch_sol(complete_charges, found_charges + 18*i, num_complete_sol)) {
            add_charges(missing_charges, found_charges + 18*i,
                        num_missing_charges, BUFFER_SIZE * (1 + num_missing_charges / 5));
            num_missing_charges++;
            
            printf("is nowhere to be seen??\n");
        }
    }

    printf("Solutions missing from the complete list:\n");
    for (int i = 0; i < num_missing_charges; i++) {
        for (int j = 0; j < 18; j++) {
            printf("% d ", *(missing_charges + 18*i+j));    
        }
        printf("\n");
    }

    fclose(complete_sol);
    fclose(found_sol);

    free(complete_charges);
    free(found_charges);
    free(missing_charges);
}