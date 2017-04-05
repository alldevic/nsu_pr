/**
 * @mainpage Laboratory work #10. Prim’s Minimum Spanning Tree
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "solution.h"

/**
 * @def Macros for catching global errors
 */
#define ERR(x) {if (x) {perror(__func__); return errno;};}

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
    int i = 0, j = 0, u = 0, visited[gr->n], v;
    unsigned int weight[gr->n], min, w;
    EdgeList tmp;

    for (i = 0; i < gr->n; i++) {
        gr->mst[i] = i - 1, weight[i] = INFTY, visited[i] = FALSE;
    }
    weight[0] = 0;

    for (i = 0; i < gr->n - 1; i++) {
        min = INFTY, u = 0;

        for (j = 0; j < gr->n; j++) {
            if (!visited[j] && weight[j] < min) {
                min = weight[j], u = j;
            }
        }

        visited[u] = TRUE;

        for (tmp = gr->data[u]; tmp; tmp = tmp->next) {
            v = tmp->data.vertex, w = tmp->data.weight;

            if (!visited[v] && w < weight[v]) {
                gr->mst[v] = u, weight[v] = w;
            }
        }
    }

    for (i = 0; i < gr->n; i++) {
        gr->not_connectivity += (weight[i] == INFTY);
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

    /* Read 1st line */
    ERR(fscanf(file, "%d", &(gr->n)) != 1);
    ARG_ERR((gr->n < 0) || (gr->n > MAX_VERTEX), BAD_NV);

    /* Read 2nd line */
    ERR(fscanf(file, "%d", &(gr->m)) != 1);
    ARG_ERR((gr->m < 0) || (gr->m > (gr->n * (gr->n + 1) / 2)), BAD_NE);

    ARG_ERR((gr->not_connectivity = (!gr->n) || (gr->m < (gr->n - 1))), 0);

    /* Read edges data */
    ERR((gr->data = (EdgeList *) calloc((size_t) gr->n, sizeof(EdgeList))) == NULL);
    ERR((gr->mst = (int *) malloc(gr->n * sizeof(int))) == NULL);
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
    unsigned int weight = 0;
    ARG_ERR(!gr->m, 0);
    for (i = 0; ((i < gr->m) && (!feof(file))); i++) {
        ARG_ERR(fscanf(file, "%d %d %d", &src, &dest, &weight) != 3, BAD_NL);
        ARG_ERR(((src < 1) || (src > gr->n)), BAD_V);
        ARG_ERR(((dest < 1) || (dest > gr->n)), BAD_V);
        ARG_ERR((weight < 0) || (weight > INT_MAX), BAD_LEN);
        if (src != dest) {
            src--, dest--;
            ERR(edgeList_add(&gr->data[src], (Edge) {dest, weight}));
            ERR(edgeList_add(&gr->data[dest], (Edge) {src, weight}));
        }
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
    if (gr->not_connectivity) {
        fprintf(file, NO_TREE);
    } else if ((gr->n != 1) && (gr->m)) {
        for (i = 1; i < gr->n; i++) {
            fprintf(file, "%d %d\n", gr->mst[i] + 1, i + 1);
        }
    }
}

/**
 * @function Add a new element to list from data
 * @param lst - - destination list
 * @param data - edge to add
 * @return error code
 */
int edgeList_add(EdgeList *lst, Edge data) {
    EdgeList node = (EdgeList) malloc(sizeof(struct edgeList));
    ERR(node == NULL);

    node->data = data;
    node->next = *lst;

    *lst = node;
    return 0;
}
