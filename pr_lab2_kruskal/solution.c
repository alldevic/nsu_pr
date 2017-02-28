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
 * @function Entry point. At begin get data from <b>INPUT</b>, then run Prim's algorithm and print
 * answer to <b>OUTPUT</b>
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
    if (gr->not_connectivity)
        kruskal(gr);
    fprint_min_tree(file, gr);
    fclose(file);
    return 0;
}

void kruskal(Graph *gr) {
    int e = 0; // An index variable, used for result[]
    int i = 0; // An index variable, used for sorted edges
    int x, y;

    qsort(gr->edge, (size_t) gr->m, sizeof(gr->edge[0]), edge_comp);
    Subset *subsets = (Subset *) malloc(gr->n * sizeof(Subset));

    for (i = 0; i < gr->n; ++i) {
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }

    i = 0;
    while (e < gr->n - 1) {
        Edge next_edge = gr->edge[i++];
        x = subset_find(subsets, next_edge.src);
        y = subset_find(subsets, next_edge.dest);
        if (x != y) {
            gr->min_tree[e++] = next_edge;
            subset_union(subsets, x, y);
        }
    }
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
int fread_edges(FILE *file, Graph *gr) {
    int i = 0, src = 0, dest = 0, weight = 0;
    ARG_ERR(!gr->m, 0);
    for (i = 0; ((i < gr->m) && (!feof(file))); i++) {
        if (fscanf(file, "%d %d %d", &src, &dest, &weight) != 3) {
            break;
        }
        ARG_ERR(((src < 1) || (src > gr->n)), BAD_V);
        ARG_ERR(((dest < 1) || (dest > gr->n)), BAD_V);
        ARG_ERR((weight < 0) || (weight > INT_MAX), BAD_LEN);
        if (src != dest) {
            gr->edge[i].src = src - 1;
            gr->edge[i].dest = dest - 1;
            gr->edge[i].weight = weight;
        }
    }
    ARG_ERR(i != (gr->m), BAD_NL);

    /* Check graph for connectivity */
    int *visited = calloc((size_t) gr->n, sizeof(int));
    dfs(gr, 0, visited);
    gr->not_connectivity = 0;
    for (i = 0; i < gr->n; i++) {
        gr->not_connectivity += (visited[i]) ? 0 : 1;
    }
    return 0;
}

/**
 * @function Service function for allocating memory for dynamic arrays in graph and setting default
 * values for them
 * @param gr - graph for initialisation
 * @return error code
 */
int init_arrays(Graph *gr) {
    ERR((gr->edge = (Edge *) malloc(gr->m * sizeof(Edge))) == NULL);
    ERR((gr->min_tree = (Edge *) malloc((gr->n) * sizeof(Edge))) == NULL);
    int i = 0;
    for (i = 0; i < gr->n; i++) {
        gr->edge[i].src = 0;
        gr->edge[i].dest = 0;
        gr->edge[i].weight = 0;
        gr->min_tree[i].src = i - 1;
        gr->min_tree[i].dest = i;
    }
    gr->min_tree[0].dest = -1;
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
    if ((gr->n == 1) && (gr->m == 0)) {

    } else if ((!gr->n) || (gr->m < (gr->n - 1))) {
    /*} else if ((!gr->n) || (gr->m < (gr->n - 1)) || gr->not_connectivity) {*/
        fprintf(file, "no spanning tree");
    } else {
        for (i = 1; i < gr->n; i++) {
            fprintf(file, "%d %d\n", gr->min_tree[i].src + 1, gr->min_tree[i].dest + 1);
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
void dfs(Graph *gr, int k, int *visited) {
    visited[k] = 1;

    int i = 0;
    for (i = 0; i < gr->n; i++) {
        if (!visited[i] && gr->min_tree[i].weight != 0)
            dfs(gr, i, visited);
    }
}

int subset_find(Subset *subsets, int i) {
    if (subsets[i].parent != i) {
        subsets[i].parent = subset_find(subsets, subsets[i].parent);
    }
    return subsets[i].parent;
}

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

int edge_comp(const void *a, const void *b) {
    Edge *a1 = (Edge *) a;
    Edge *b1 = (Edge *) b;
    return a1->weight > b1->weight;
}
