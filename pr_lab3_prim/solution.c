#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "solution.h"

#define ERR(x) if (x) {perror(__func__); return errno;};

int main() {

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


void prim(Graph gr) {
    int key[gr->n], count = 0, i = 0, v = 0;
    char mstSet[gr->n];

    for (i = 0; i < gr->n; i++) {
        key[i] = INT_MAX;
        mstSet[i] = 0;
    }

    key[0] = 0;
    /*for (v = 0; v < gr->n; v++) {
        fprintf(stdout, "%d ", gr->min_tree[i]);
    }
    fprintf(stdout, "\n\n");*/
    for (count = 0; count < gr->n - 1; count++) {
        int min = INT_MAX, u = 0;

        for (v = 0; v < gr->n; v++) {
            if (!mstSet[v] && key[v] < min) {
                min = key[v];
                u = v;
            }
        }

        mstSet[u] = 1;
        for (v = 0; v < gr->n; v++) {
            if (gr->edges[u][v] < INFTY && !mstSet[v] && gr->edges[u][v] < key[v]) {
                gr->min_tree[v] = u;
                key[v] = gr->edges[u][v];
            }
        }
    }
    /*for (v = 0; v < gr->n; v++) {
        fprintf(stdout, "%d ", gr->min_tree[v]);
    }*/
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
    ARG_ERR((gr->n < 0) || (gr->n >= MAX_VERTEX), BAD_NV);

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
int fread_edges(FILE *file, Graph gr) {
    int i = 0, src = 0, dest = 0, weight = 0;
    ARG_ERR(!gr->m, 0);
    for (i = 0; ((i < gr->m) && (!feof(file))); i++) {
        if (fscanf(file, "%d %d %d", &src, &dest, &weight) != 3){
            break;
        }
        ARG_ERR(((src < 1) || (src > gr->n)), BAD_V);
        ARG_ERR(((dest < 1) || (dest > gr->n)), BAD_V);
        ARG_ERR((weight < 0) || (weight > INT_MAX), BAD_LEN);
        if (src != dest) {
            gr->edges[src - 1][dest - 1] = (unsigned int) weight;
            gr->edges[dest - 1][src - 1] = (unsigned int) weight;
        }
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
    int i = 0, j = 0;
    ERR((gr->edges = (unsigned int **) malloc(gr->n * sizeof(int *))) == NULL);
    ERR((gr->min_tree = (int *) malloc(gr->n * sizeof(int))) == NULL);

    for (i = 0; i < gr->n; i++) {
        ERR((gr->edges[i] = (unsigned int *) malloc(gr->n * sizeof(int))) == NULL);

        for (j = 0; j < gr->n; j++) {
            gr->edges[i][j] = INFTY;
        }
        gr->min_tree[i] = 0;
    }
    gr->min_tree[0] = ROOT;
/*    for (i = 0; i < gr->n; i++) {
        fprintf(stdout, "%d ", gr->min_tree[i]);
    }*/
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

void fprint_min_tree(FILE *file, Graph gr) {
    int i = 0;
    if ((!gr->n) || (gr->m < (gr->n - 1))) {
        fprintf(file, "no spanning tree");
    } else {
        for (i = 1; i < gr->n; i++) {
            fprintf(file, "%d %d\n", gr->min_tree[i] + 1, i + 1);
        }
    }
}
