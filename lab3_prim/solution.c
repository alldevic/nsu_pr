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
    if (!gr->not_connectivity) {
        prim(gr);
    }
    fprint_min_tree(file, gr);
    fclose(file);
    return 0;
}

/**
 * @function Implementation of Prim's minimum spanning tree algorithm
 * @param gr - graph for searching minimum spanning tree
 */
void prim(Graph gr) {
    int weight[gr->n], i = 0, j = 0, min, u = 0;
    char visited[gr->n];

    for (i = 0; i < gr->n; i++) {
        weight[i] = INT_MAX;
        visited[i] = 0;
    }
    weight[0] = 0;
    for (i = 0; i < gr->n - 1; i++) {
        min = INT_MAX;
        u = 0;

        for (j = 0; j < gr->n; j++) {
            if (!visited[j] && weight[j] < min) {
                min = weight[j];
                u = j;
            }
        }

        visited[u] = 1;
        for (j = 0; j < gr->n; j++) {
            if (gr->edges[u][j] && !visited[j] && gr->edges[u][j] < weight[j]) {
                gr->min_tree[j] = u;
                weight[j] = gr->edges[u][j];
            }
        }
    }
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

    gr->not_connectivity = 1;

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

    /* Check graph for connectivity */
    gr->not_connectivity = 0;
    int *visited = calloc((size_t) gr->n, sizeof(int));
    dfs(gr, 0, visited);
    for (i = 0; i < gr->n; i++) {
        gr->not_connectivity += !visited[i];
    }

    return 0;
}

/**
 * @function Service function for allocating memory for dynamic arrays in graph and setting default
 * values for them
 * @param gr - graph for initialisation
 * @return error code
 */
int init_arrays(Graph gr) {
    int i = 0;
    ERR((gr->edges = (unsigned int **) malloc(gr->n * sizeof(int *))) == NULL);
    ERR((gr->min_tree = (int *) malloc(gr->n * sizeof(int))) == NULL);

    for (i = 0; i < gr->n; i++) {
        ERR((gr->edges[i] = (unsigned int *) calloc((size_t) gr->n, sizeof(int))) == NULL);
        gr->min_tree[i] = i - 1;
    }

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

    } else if ((!gr->n) || (gr->m < (gr->n - 1)) || gr->not_connectivity) {
        fprintf(file, "no spanning tree");
    } else {
        for (i = 1; i < gr->n; i++) {
            fprintf(file, "%d %d\n", gr->min_tree[i] + 1, i + 1);
        }
    }
}

/**
 * @function Service function for finding connectivity in graph. This is a depth-first search
 * implementation
 * @param gr - graph for finding connectivity
 * @param k - current vertex
 * @param visited - array with visited vertex
 */
void dfs(Graph gr, int k, int *visited) {
    visited[k] = 1;

    int i = 0;
    for (i = 0; i < gr->n; i++) {
        if (!visited[i] && gr->edges[i][k] != 0)
            dfs(gr, i, visited);
    }
}
