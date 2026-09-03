#ifndef UTILS_H
#define UTILS_H

#define BUFFER_SIZE 5

#define SMNUR_1_SIZE 38
#define SMNUR_5_SIZE 111152
#define SMNUR_10_SIZE 21546920

#include <stdio.h>

static void _swap(int *a, int *b);

int *extract_charges(FILE *sol, int num_sol);
void add_charges(int *a, int *b, int pos, int len);

void sort(int *charges, int num_sol);
void sort_abs(int *charges, int num_sol);
int srch(int *a, int *b, int num_sol_a);
int is_multiple(int *a, int *b, int num_sol_a);


#endif