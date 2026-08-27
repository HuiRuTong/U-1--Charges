#include <stdio.h>
#include <stdlib.h>
/*
    1. Convert charges to 18 different arrays
    2. For each element in found_charges, search for
       the lower and upper indices of each element
       until only 1 or no index remains
*/

int *quarter_srch(int *nums, int val, int len) {
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
    int low = 0;
    int high = len - 1;
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
    high = len - 1;

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

int main() {
    int arr[10] = {1, 2, 3, 3, 4, 4, 5, 5, 5, 6};

    int *b = quarter_srch(arr, 2, 10);
    printf("%d %d\n", b[0], b[1]);

    free(b);
}