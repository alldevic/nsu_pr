/**
 * @mainpage Laboratory work #11. Topology sort
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "solution.h"

/**
 * @def Macros for catching global errors
 */
#define ERR(x) {if (x) {perror(NULL); return errno;}};

/**
 * @function Entry point. At begin get data from <b>INPUT</b>, then init visited array and
 * run topology sort. Then write answer to <b>OUTPUT</b>
 * @return error code
 */
int main(void) {
    Graph gr = malloc(sizeof(Graph));
    int er = 0, i = 0, *visited;
    FILE *file = fopen(OUTPUT, "w");

    ERR(gr == NULL);
    er = read_data(gr);
    ERR(er > 0);
    ERR(file == NULL);

    if (er < 0) {
        fprintf(file, get_err_str((ArgError) er));
        fclose(file);
        return 0;
    }

    visited = calloc((size_t) gr->n, sizeof(int));
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

/**
 * @function Implementation of recursive Tarjan topological sort algorithm
 * @param gr - graph for sorting
 * @param k - start vertex
 * @param visited - array with color of every vertex
 * @return error code
 */
int top_sort(Graph gr, int k, int *visited) {
    IntList tmp = NULL;
    visited[k] = GRAY;
        for (tmp = gr->data[k]; tmp && gr->is_sorting; tmp = tmp->next) {
        gr->is_sorting = (visited[tmp->data] == GRAY) ? FALSE : gr->is_sorting;

        if (visited[tmp->data] == WHITE) {
            ERR(top_sort(gr, tmp->data, visited));
        }
    }

    visited[k] = BLACK;
    ERR(intList_add(&gr->sorted, k));

    return 0;
}

/**
 * @function Function for getting main information about graph from <b>FILE</b>
 * @param gr - empty graph for setting data
 * @return error code
 */
int read_data(Graph gr) {
    int er;
    FILE *file = fopen(INPUT, "r");
    ERR(file == NULL);

    /* Read 1st line */
    ARG_ERR(fscanf(file, "%d", &(gr->n)) != 1, BAD_NL);
    ARG_ERR((gr->n < 0) || (gr->n > MAX_VERTEX), BAD_NV);

    /* Read 2nd line */
    ARG_ERR(fscanf(file, "%d", &(gr->m)) != 1, BAD_NL);
    ARG_ERR((gr->m < 0) || (gr->m > (gr->n * (gr->n + 1) / 2)), BAD_NE);

    /* Init service variables */
    gr->is_sorting = TRUE;
    ERR((gr->data = (IntList *) calloc((size_t) gr->n, sizeof(IntList))) == NULL);
    gr->sorted = NULL;

    /* Read edges data */
    er = fread_edges(file, gr);
    ERR(er > 0);
    fclose(file);

    return er;
}

/**
 * @function Function for get data from file to the adjacency matrix in graph
 * @param file - opened for reading file
 * @param gr - graph for reading adjacency matrix
 * @return error code
 */
int fread_edges(FILE *file, Graph gr) {
    int i = 0, src = 0, dest = 0;
    ARG_ERR(!gr->m, 0);

    for (i = 0; ((i < gr->m) && (!feof(file))); i++) {
        ARG_ERR(fscanf(file, "%d %d", &src, &dest) != 2, BAD_NL);
        ARG_ERR(((src < 1) || (src > gr->n)), BAD_V);
        ARG_ERR(((dest < 1) || (dest > gr->n)), BAD_V);
        ERR(intList_add(&gr->data[src - 1], dest - 1));
    }

    ARG_ERR(i != (gr->m), BAD_NL);

    return 0;
}

/**
 * @function Service function returns text alias for <b>code/b>
 * @param code - argument error code
 * @return STR code or NULL if <b>code</v> not in ArgError enum
 */
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

/**
 * @function Print sorted vertex of graph
 * @param file - file opened for writing
 * @param gr - graph with tree information
 * @return error code
 */
void fprint_sorted(FILE *file, Graph gr) {
    if (!gr->is_sorting) {
        fprintf(file, IMPOSSIBLE_STR);
        return;
    }

    for (; gr->sorted; gr->sorted = gr->sorted->next) {
        fprintf(file, "%d ", gr->sorted->data + 1);
    }

    return;
}

/**
 * @function Add a new element to list from data
 * @param lst - - destination list
 * @param data - int to add
 * @return error code
 */
int intList_add(IntList *lst, int data) {
    IntList node = (IntList) malloc(sizeof(struct intList));
    ERR(node == NULL);

    node->data = data;
    node->next = *lst;
    *lst = node;

    return 0;
}
