#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UTILS.h"

static void _swap(int *a, int *b) {
    /*
        Swaps the values stored at
        a and b
    */
   
    int temp = *a;
    *a = *b;
    *b = temp;
}

int *extract_charges(FILE *sol, int num_sol) {
    int *all_charges = malloc(sizeof(int) * num_sol * 18);

    for (int i = 0; i < num_sol; i++) {
        for (int j = 0; j < 18; j++) {
            fscanf(sol, " %d", all_charges + 18*i+j);
        }
    }
    return all_charges;
}

void add_charges(int *a, int *b, int pos, int len) {
    /*
        Appends all elements of b onto a

        To be more specific: a is a
        flattened N x 18 array while
        b is an array with 18 elements

        pos :
            Index of the first unfilled row
        len :
            The number of rows a can actually hold
    */

    // Reserve more space if needed
    if (pos >= len) {
        a = realloc(a, sizeof(int) * (len + BUFFER_SIZE) * 18);
    }

    for (int i = 0; i < 18; i++) {
        a[18*pos + i] = b[i];
    }
}

void sort(int *charges, int num_sol) {
    /*
        Sorts charges in increasing order
        to match the paper's charge
        arrangements
    */
    for (int i = 0; i < num_sol*18; i+=3) {
        if (charges[i] > charges[i+1]) {
            _swap(charges + i, charges + i+1);
        }
        if (charges[i+1] > charges[i+2]) {
            _swap(charges+ i+1, charges + i+2);
            if (charges[i] > charges[i+1]) {
                _swap(charges + i, charges+ i+1);
            }
        }
    }
}

int srch(int *a, int *b, int num_sol_a) {
    /*
        Searches for any rows in a that
        match b exactly

        To be more specific: a is a
        flattened N x 18 array while
        b is an array with 18 elements
    */

    for (int i = 0; i < num_sol_a; i++) {
        if (!memcmp(a + 18*i, b, 18*sizeof(int))) {
            return i;
        }
    }
    return -1;
}

int is_multiple(int *a, int *b, int num_sol_a) {
    int dot = 0;
    int a_sqr = 0;
    int b_sqr = 0;
    
    for (int i = 0; i < num_sol_a; i++) {
        for (int j = 0; j < 18; j++) {
            dot += a[i] * b[i];
            a_sqr += *(a + 18*i+j)*(*(a + 18*i+j));
            b_sqr += *(b + j)*(*(b + j));
        }
        if (dot*dot == a_sqr * b_sqr) {
            return i;
        }
        return -1;
    }
}