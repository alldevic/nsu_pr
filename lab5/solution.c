/**
 * @mainpage Laboratory work №5
 * Heapsort
 */

#include <stdio.h>
#include <malloc.h>
#include "solution.h"

/**
 * @function Entry point to program. Reading array from INPUT, sorting and write
 * to OUTPUT.
 * @return err - error code
 */
int main(void)
{
    int n = 0, *array;
    int err = get_input(&n, &array);
    heap_sort(array, n);
    err = !err ? set_output(&n, array) : err;
    free(array);
    return err;
}

/**
 * @function Get <b>n</b> and <b>array</b> from INPUT
 * @param n - pointer to variable of size of input array
 * @param array - pointer of mamory area for arrat size of <b>n</b>
 * @return err - error code
 */
int get_input(int *n, int **array)
{
    FILE *f_in;
    int err = NO_ERR, i = 0;

    if ((f_in = fopen(INPUT, "r")) != NULL)
    {
        err = (fscanf(f_in, "%d", n) < 0) ? ERR_SCAN : err;

        if ((*array = calloc((size_t) *n, sizeof(int))) == NULL)
            err = ERR_ALLOC;

        for (i = 0; !err && (i < *n); i++)
            err = (fscanf(f_in, "%d", *array + i) < 0) ? ERR_SCAN : err;

        err = (!err && (fclose(f_in) == EOF)) ? ERR_CLOSE : err;
    }
    else
        err = ERR_OPEN;
    return err;
}

int max (int *a, int n, int i, int j, int k) {
    int m = i;
    if (j < n && a[j] > a[m])
        m = j;
    if (k < n && a[k] > a[m])
        m = k;

    return m;
}

void down_heap(int *a, int n, int i) {
    int t;
    while (1) {
        int j = max(a, n, i, 2 * i + 1, 2 * i + 2);
        if (j == i)
            break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
        i = j;
    }
}

void heap_sort(int *a, int n) {
    int i;
    for (i = (n - 2) / 2; i >= 0; i--) {
        down_heap(a, n, i);
    }
    for (i = 0; i < n; i++) {
        int t = a[n - i - 1];
        a[n - i - 1] = a[0];
        a[0] = t;
        down_heap(a, n - i - 1, 0);
    }
}

/**
 * @function Write sorted array <b>array</b> to OUTPUT
 * @param n - size of array
 * @param array - sorted array
 * @return err - error code
 */
int set_output(int *n, int *array)
{
    int err = NO_ERR, i = 0;
    FILE *f_out;
    err = ((f_out = fopen(OUTPUT, "w")) == NULL) ? ERR_CREATE : err;

    for (i = 0; !err && (i < *n); i++)
        err = (fprintf(f_out, (!i) ? "%d" : " %d", array[i]) < 0) ? ERR_PRINT : err;

    return ((fclose(f_out) == EOF) && !err) ? ERR_CLOSE : err;
}
