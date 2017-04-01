//TODO: docs

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
#define ERR(x) {if (x) {perror(__func__); return errno;}};

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

    int *visited = calloc((size_t) gr->n, sizeof(int));
    ERR(visited == NULL);
    for (i = 0; i < gr->n; i++) {
        if (visited[i] == WHITE) {
            ERR(top_sort(gr, i, visited));
        }
    }

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

int intList_add_int(IntList *lst, int data) {
    IntList node = NULL;
    ERR(intList_init(&node, data));

    node->next = *lst;
    *lst = node;

    return 0;
}

int top_sort(Graph gr, int k, int *visited) {
    visited[k] = GRAY;

    IntList tmp = gr->data[k];
    while (tmp && gr->is_sorting) {
        gr->is_sorting = (visited[tmp->data] == GRAY) ? 0 : gr->is_sorting;

        if (visited[tmp->data] == WHITE) {
            ERR(top_sort(gr, tmp->data, visited));
        }

        tmp = tmp->next;
    }

    visited[k] = BLACK;
    ERR(intList_add_int(&gr->sorted, k));

    return 0;
}


int read_data(Graph gr) {
    FILE *file = fopen(INPUT, "r");
    ERR(file == NULL);

    /* Read 1st line */
    ARG_ERR(fscanf(file, "%d", &(gr->n)) != 1, BAD_NL);
    ARG_ERR((gr->n < 0) || (gr->n > MAX_VERTEX), BAD_NV);

    /* Read 2nd line */
    ARG_ERR(fscanf(file, "%d", &(gr->m)) != 1, BAD_NL);
    ARG_ERR((gr->m < 0) || (gr->m > (gr->n * (gr->n + 1) / 2)), BAD_NE);

    /* Init service variables */
    gr->is_sorting = 1;
    ERR((gr->data = (IntList *) calloc((size_t) gr->n, sizeof(IntList))) == NULL);
    gr->sorted = NULL;

    /* Read edges data */
    int er = fread_edges(file, gr);
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
        intList_add_int(&gr->data[src - 1], dest - 1);
    }

    ARG_ERR(i != (gr->m), BAD_NL);

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
    if (!gr->is_sorting) {
        fprintf(file, IMPOSSIBLE_STR);
        return;
    }

    while (gr->sorted) {
        fprintf(file, "%d ", gr->sorted->data + 1);
        gr->sorted = gr->sorted->next;
    }

    return;
}
