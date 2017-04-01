/**
 * @mainpage Laboratory work #9. Kruskal’s Minimum Spanning Tree
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
 * @function Entry point. At begin get data from <b>INPUT</b>, then run Kruskal’s algorithm and
 * print answer to <b>OUTPUT</b>
 * @return error code
 */
int main(void) {
    Graph *gr = malloc(sizeof(Graph));
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
    ERR(kruskal(gr));
    fprint_min_tree(file, gr);
    fclose(file);
    return 0;
}

/**
 * @function Implementation of Kruskal's minimum spanning tree algorithm
 * @param gr - graph for searching minimum spanning tree
 * @return error code
 */
int kruskal(Graph *gr) {
    int e = 0; // An index variable, used for min_tree[]
    int i = 0; // An index variable, used for sorted edges
    int x, y;

    qsort(gr->edge, (size_t) gr->m, sizeof(gr->edge[0]), edge_comp);
    Subset *subsets = (Subset *) malloc(gr->n * sizeof(Subset));
    ERR(subsets == NULL);
    for (i = 0; i < gr->n; ++i) {
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }

    i = 0;
    while ((e < gr->n - 1) && (i < gr->m)) {
        Edge next_edge = gr->edge[i++];
        x = subset_find(subsets, next_edge.src);
        y = subset_find(subsets, next_edge.dest);
        if (x != y) {
            gr->min_tree[e++] = next_edge;
            subset_union(subsets, x, y);
        }
    }

    gr->not_connectivity += (e == gr->n - 1) ? 0 : 1;
    return 0;
}

/**
 * @function Function for getting main information about graph from <b>FILE</b>
 * @param gr - empty graph for setting data
 * @return error code
 */
int read_data(Graph *gr) {
    int er; /* Error code for fread_edges */
    FILE *file = fopen(INPUT, "r");
    ERR(file == NULL);

    /*Read 1st line*/
    ERR(fscanf(file, "%d", &(gr->n)) != 1);
    ARG_ERR((gr->n < 0) || (gr->n > MAX_VERTEX), BAD_NV);

    /*Read 2nd line*/
    ERR(fscanf(file, "%d", &(gr->m)) != 1);
    ARG_ERR((gr->m < 0) || (gr->m > (gr->n * (gr->n + 1) / 2)), BAD_NE);
    gr->not_connectivity = 0;

    /* Allocating memory for arrays*/
    ERR((gr->edge = (Edge *) calloc((size_t) gr->m, sizeof(Edge))) == NULL);
    ERR((gr->min_tree = (Edge *) calloc((size_t) gr->n, sizeof(Edge))) == NULL);

    /*Read edges data*/
    er = fread_edges(file, gr);
    ERR(er > 0);
    fclose(file);
    return er;
}

/**
 * @function Function for get data from file to the edges list in graph
 * @param file - opened for reading file
 * @param gr - graph for reading adjacency matrix
 * @return error code
 */
int fread_edges(FILE *file, Graph *gr) {
    int i = 0, src = 0, dest = 0, weight = 0;

    ARG_ERR(!gr->m, 0);
    for (i = 0; ((i < gr->m) && (!feof(file))); i++) {
        ARG_ERR((fscanf(file, "%d %d %d", &src, &dest, &weight) != 3), BAD_NL);
        ARG_ERR(((src < 1) || (src > gr->n)), BAD_V);
        ARG_ERR(((dest < 1) || (dest > gr->n)), BAD_V);
        ARG_ERR((weight < 0) || (weight > INT_MAX), BAD_LEN);
        if (src != dest) {
            gr->edge[i].src = src - 1;
            gr->edge[i].dest = dest - 1;
            gr->edge[i].weight = weight;
        }
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
void fprint_min_tree(FILE *file, Graph *gr) {
    int i = 0;
    if ((!gr->n) || (gr->m < (gr->n - 1)) || gr->not_connectivity) {
        fprintf(file, IMPOSSIBLE_STR);
    } else {
        for (i = 0; i < gr->n - 1; i++) {
            fprintf(file, "%d %d\n", gr->min_tree[i].src + 1, gr->min_tree[i].dest + 1);
        }
    }
}

/**
 * @function Implementation of find function of disjoint-set data structure
 * @param subsets - array with disjoint-set
 * @param i - element of set
 * @return root of set <b>i</b>
 */
int subset_find(Subset *subsets, int i) {
    if (subsets[i].parent != i) {
        subsets[i].parent = subset_find(subsets, subsets[i].parent);
    }
    return subsets[i].parent;
}

/**
 * @function Implementation of union operation for disjoint-set data structure
 * @param subsets - array with sets
 * @param x - the member of first class, root of new class
 * @param y - the member of second class
 */
void subset_union(Subset *subsets, int x, int y) {
    int xroot = subset_find(subsets, x);
    int yroot = subset_find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank) {
        subsets[xroot].parent = yroot;
    } else if (subsets[xroot].rank > subsets[yroot].rank) {
        subsets[yroot].parent = xroot;
    } else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

/**
 * @function Comparator of <b>Edges</b> for qsort function
 * @param a - first edge
 * @param b - second edge
 * @return a > b or a<=b
 */
int edge_comp(const void *a, const void *b) {
    return ((Edge *) a)->weight > ((Edge *) b)->weight;
}
