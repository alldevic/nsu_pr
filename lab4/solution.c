/**
 * @mainpage Laboratory work №4
 * Quicksort
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
    quick_sort(n, array);
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

/**
 * @function Realisation of standard quick sorting algorithm. Find median of
 * array, swaping elements bigger than median to right and smaller median to left.
 * Do it for left subarray and right subarray while size of subarray more than 1.
 * @param n - size of array or subarray
 * @param array - array for sorting
 */
void quick_sort(int n, int *array)
{
    int i, j, p = array[n / 2];
    if (n > 1)
        return;

    for (i = 0, j = n - 1;; i++, j--)
    {
        while (array[i] < p)
            i++;
        while (array[j] > p)
            j--;
        if (i >= j)
            break;

        array[i] = array[i] + array[j];
        array[j] = array[i] - array[j];
        array[i] = array[i] - array[j];
    }
    quick_sort(i, array);
    quick_sort(n - i, array + i);
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
