/**
 * @mainpage Laboratory work №4
 * Quicksort
 */

#include <stdio.h>
#include <malloc.h>
#include "solution.h"


int main(void) {
    int n = 0, *array;
    int err = get_input(&n, &array);
    quick_sort(n, array);
    err = !err ? set_output(n, array) : err;
    free(array);
    return err;
}


/**
 * @function Get <b>n</b> and <b>array</b> from INPUT
 * @param n - pointer to variable of size of input array
 * @param array - pointer of mamory area for arrat size of <b>n</b>
 * @return err - error code
 */
int get_input(int *n, int **array) {
    FILE *f_in;
    int err = NO_ERR, i = 0;

    if ((f_in = fopen(INPUT, "r")) != NULL) {
        err = (fscanf(f_in, "%d", n) < 0) ? ERR_SCAN : err;

        if ((*array = calloc((size_t) *n, sizeof(int))) == NULL)
            err = ERR_ALLOC;

        for (i = 0; !err && (i < *n); i++)
            err = (fscanf(f_in, "%d", *array + i) < 0) ? ERR_SCAN : err;

        err = (!err && (fclose(f_in) == EOF)) ? ERR_CLOSE : err;
    } else
        err = ERR_OPEN;
    return err;
}

void quick_sort(int n, int *a) {
    int i, j, p, t;
    if (n < 2)
        return;
    p = a[n / 2];
    for (i = 0, j = n - 1;; i++, j--) {
        while (a[i] < p)
            i++;
        while (p < a[j])
            j--;
        if (i >= j)
            break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
    quick_sort(i, a);
    quick_sort(n - i, a + i);
}

int set_output(int n, int *array) {
    int err = NO_ERR, i = 0;
    FILE *f_out;
    err = ((f_out = fopen(OUTPUT, "w")) == NULL) ? ERR_CREATE : err;

    for (i = 0; !err && (i < n); i++)
        err = (fprintf(f_out, (!i) ? "%d" : " %d", array[i]) < 0) ? ERR_PRINT : err;

    return ((fclose(f_out) == EOF) && !err) ? ERR_CLOSE : err;
}
