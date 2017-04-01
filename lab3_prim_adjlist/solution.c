/**
 * @mainpage Laboratory work #10. Prim’s Minimum Spanning Tree
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "solution.h"

/**
 * @def Macros for catching global errors.
 */
#define ERR(x) if (x) {perror(__func__); return errno;};

/**
 * @function Entry point. At begin get data from <b>INPUT</b>, then run Prim's algorithm and print
 * answer to <b>OUTPUT</b>
 * @return error code
 */
int main(void) {
    Graph gr = malloc(sizeof(Graph));
    ERR(gr == NULL);
    int er = read_data(gr);
    ERR(er > 0);
    FILE *file = fopen(OUTPUT, "w");
    ERR(file == NULL);
    if (er < 0) {
        fprintf(file, get_err_str((ArgError) er));
        fclose(file);
        return 0;
    }

    prim(gr);
    fprint_min_tree(file, gr);
    fclose(file);
    return 0;
}

/**
 * @function Implementation of Prim's minimum spanning tree algorithm
 * @param gr - graph for searching minimum spanning tree
 */
void prim(Graph gr) {

}

/**
 * @function Function for getting main information about graph from <b>FILE</b>
 * @param gr - empty graph for setting data
 * @return error code
 */
int read_data(Graph gr) {
    int er; /* Error code for fread_edges */
    FILE *file = fopen(INPUT, "r");
    ERR(file == NULL);

    /*Read 1st line*/
    ERR(fscanf(file, "%d", &(gr->n)) != 1);
    ARG_ERR((gr->n < 0) || (gr->n > MAX_VERTEX), BAD_NV);

    /*Read 2nd line*/
    ERR(fscanf(file, "%d", &(gr->m)) != 1);
    ARG_ERR((gr->m < 0) || (gr->m > (gr->n * (gr->n + 1) / 2)), BAD_NE);

    gr->is_connectivity = 1;

    /*Read edges data*/
    ERR(init_arrays(gr));
    er = fread_edges(file, gr);
    ERR(er > 0);
    fclose(file);
    return er;
}

/**
 * @function Function for get data from file to the adjacency matrix in graph
 * @param file - opeened for reading file
 * @param gr - graph for reading adjacency matrix
 * @return error code
 */
int fread_edges(FILE *file, Graph gr) {
    int i = 0, src = 0, dest = 0, weight = 0;
    ARG_ERR(!gr->m, 0);
    for (i = 0; ((i < gr->m) && (!feof(file))); i++) {
        ARG_ERR(fscanf(file, "%d %d %d", &src, &dest, &weight) != 3, BAD_NL);
        ARG_ERR(((src < 1) || (src > gr->n)), BAD_V);
        ARG_ERR(((dest < 1) || (dest > gr->n)), BAD_V);
        ARG_ERR((weight < 0) || (weight > INT_MAX), BAD_LEN);
        gr->edges[src - 1][dest - 1] = (unsigned int) weight;
        gr->edges[dest - 1][src - 1] = (unsigned int) weight;

    }
    ARG_ERR(i != (gr->m), BAD_NL);

    return 0;
}

/**
 * @function Service function for allocating memory for dynamic arrays in graph and setting default
 * values for them
 * @param gr - graph for initialisation
 * @return error code
 */
int init_arrays(Graph gr) {
    ERR((gr->data = (EdgeList *) calloc((size_t) gr->n, sizeof(EdgeList))) == NULL);
    gr->min_tree = NULL;

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
        case BAD_LEN:
            return BAD_LEN_STR;
        case BAD_NL:
            return BAD_NL_STR;
    }

    return NULL;
}

/**
 * @function Print edges of minimum spanning tree
 * @param file - file opened for writing
 * @param gr - graph with tree information
 * @return error code
 */
void fprint_min_tree(FILE *file, Graph gr) {
    int i = 0;
    if ((gr->n == 1) && (gr->m == 0)) {

    } else if ((!gr->n) || (gr->m < (gr->n - 1)) || !gr->is_connectivity) {
        fprintf(file, "no spanning tree");
    } else {
        while (gr->min_tree) {
            fprintf(file, "%d %d\n", gr->min_tree->data.vertex + 1, gr->min_tree->data.weight +1);
            gr->min_tree = gr->min_tree->next;
        }
    }
}

