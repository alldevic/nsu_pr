/**
 * @mainpage Laboratory work #11. Topology sort
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "solution.h"

/**
 * @def Macros for catching global errors.
 */
#define ERR(x) if (x) {perror(__func__); return errno;};

int main(void) {
    Graph gr = malloc(sizeof(Graph));
    ERR(gr == NULL);
    int er = read_data(gr), i = 0;
    ERR(er > 0);
    FILE *file = fopen(OUTPUT, "w");
    ERR(file == NULL);
    if (er < 0) {
        fprintf(file, get_err_str((ArgError) er));
        fclose(file);
        return 0;
    }
    int *visited = malloc(gr->n * sizeof(int));
    for (i = 0; i <gr->n; i++)
        visited[i] = WHITE;
    top_sort(gr, 0, visited);
    fprint_sorted(file, gr);
    fclose(file);
    return 0;
}

int intList_init(IntList *node, int data) {
    *node = (IntList) malloc(sizeof(struct intList));
    ERR(node == NULL);
    (*node)->data = data;
    (*node)->next = NULL;
    return 0;
}

void intList_add(IntList *lst, IntList node) {
    node->next = *lst;
    *lst = node;
}

int top_sort(Graph gr, int k, int *visited) {
    IntList sorted_tmp = NULL, tmp = NULL;

    visited[k] = GRAY;
    tmp = gr->data[k];
    while(tmp && !gr->not_sorting)
    {
        if (visited[tmp->data] == GRAY)
            gr->not_sorting = 1;
        if (visited[tmp->data] == WHITE)
            top_sort(gr, tmp->data, visited);
        tmp = tmp->next;
    }
    visited[k] = BLACK;
    ERR(intList_init(&sorted_tmp, k));
    intList_add(&gr->sorted, sorted_tmp);
    return 0;
}

int read_data(Graph gr) {
    int er = 0; /* Error code for fread_edges */
    gr->n = -1; gr->m = -1;
    FILE *file = fopen(INPUT, "r");
    ERR(file == NULL);

    /*Read 1st line*/
    fscanf(file, "%d", &(gr->n));
    ARG_ERR((gr->n < 0) || (gr->n > MAX_VERTEX), BAD_NV);

    /*Read 2nd line*/
    fscanf(file, "%d", &(gr->m));
    ARG_ERR((gr->m < 0) || (gr->m > (gr->n * (gr->n + 1) / 2)), BAD_NL);

    gr->not_sorting = 0;

    /*Read edges data*/
    ERR(init_arrays(gr));
    er = fread_edges(file, gr);
    ERR(er > 0);
    fclose(file);
    return er;
}

int fread_edges(FILE *file, Graph gr) {
    int i = 0, src = 0, dest = 0;
    ARG_ERR(!gr->m, 0);
    for (i = 0; ((i < gr->m) && (!feof(file))); i++) {
        ARG_ERR(fscanf(file, "%d %d", &src, &dest) != 2, BAD_NL);
        ARG_ERR(((src < 1) || (src > gr->n)), BAD_V);
        ARG_ERR(((dest < 1) || (dest > gr->n)), BAD_V);
        IntList node = NULL;
        ERR(intList_init(&node, dest - 1));
        intList_add(&gr->data[src - 1], node);
    }
    ARG_ERR(i != (gr->m), BAD_NL);

    return 0;
}

int init_arrays(Graph gr) {
    ERR((gr->data = (IntList *) calloc((size_t) gr->n, sizeof(IntList))) == NULL);
    gr->sorted = NULL;
    return 0;
}

char *get_err_str(ArgError code) {
    switch (code) {
        case BAD_NV:
            return BAD_NV_STR;
        case BAD_V:
            return BAD_V_STR;
        case BAD_NE:
            return BAD_NE_STR;
        case BAD_NL:
            return BAD_NL_STR;
    }

    return NULL;
}

void fprint_sorted(FILE *file, Graph gr) {
    if (gr->not_sorting) {
        fprintf(file, IMPOSSIBLE_STR);
    } else {
        while(gr->sorted)
        {
            fprintf(file, "%d ", gr->sorted->data + 1);
            gr->sorted = gr->sorted->next;
        }

    }
}
