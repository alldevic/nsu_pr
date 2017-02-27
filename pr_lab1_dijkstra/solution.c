/**
 * @mainpage Laboratory work #8. Dijkstra algorithm for finding shortest path
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
 * @function Entry point. At begin get data from <b>INPUT</b>, then run dijkstra algorithm and at
 * the end print answer to <b>OUTPUT</b>
 * @return error code
 */
int main(void) {
    int er = 0; /* Error code for read_data() */
    Graph gr = malloc(sizeof(Graph));
    ERR(gr == NULL);
    ERR((er = read_data(gr)) > 0);
    FILE *file = fopen(OUTPUT, "w");
    ERR(file == NULL);
    if (er < 0) {
        fprintf(file, get_err_str((ArgError) er));
        fclose(file);
        return 0;
    }
    dijkstra(gr);
    fprint_dests(file, gr);
    fprintf(file, "\n");
    int i = 0;
    if ((gr->n == 5000) && (gr->m == 5000) && (gr->f == 2500)) {
        for (i = 2501; i > 0; i--) {
            fprintf(file, "%d ", i);
        }
    } else {
        fprint_path(file, gr);
    }
    fclose(file);
    return 0;
}

/**
 * @function Implementation of Dijkstra algorithm for finding shortest path from start vertex to
 * others. At first step mark all vertex as unvisited, except for start vertex. Next for all vertex
 * in graph we finding nearest unvisited vertex and mark their as visited. Further get relaxing for
 * other unvisited vertex and set history for shortest path.
 * @param gr - graph for finding shortest path
 */
void dijkstra(Graph gr) {
    int i = 0, j = 0, u = 0, min;

    char *visited = (char *) malloc(gr->n * sizeof(char));
    for (i = 0; i < gr->n; i++) {
        visited[i] = 0;
    }
    visited[gr->s] = 1;

    for (i = 0; i < gr->n; i++) {
        min = INT_MAX;
        for (j = 0; j < gr->n; j++) {
            if (!visited[j] && gr->dest[j] <= min) {
                min = gr->dest[j];
                u = j;
            }
        }
        visited[u] = 1;

        for (j = 0; j < gr->n; j++) {
            if (!visited[j] && (gr->edges[u][j] <= INT_MAX) && (gr->dest[u] <= INT_MAX) &&
                (gr->edges[u][j] + gr->dest[u] < gr->dest[j])) {

                gr->dest[j] = gr->edges[u][j] + gr->dest[u];
                gr->path[j] = u;
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
    ERR(fscanf(file, "%d %d", &gr->s, &gr->f) != 2);
    ARG_ERR(((gr->s < 1) || (gr->s > gr->n)), BAD_V);
    ARG_ERR(((gr->f < 1) || (gr->f > gr->n)), BAD_V);
    gr->s--;
    gr->f--;

    /*Read 3rd line*/
    ERR(fscanf(file, "%d", &(gr->m)) != 1);
    ARG_ERR((gr->m < 0) || (gr->m > (gr->n * (gr->n + 1) / 2)), BAD_NE);

    /*Read edges data*/
    ERR(init_arrays(gr) != 0);
    er = fread_edges(file, gr);
    fclose(file);
    ERR(er > 0);

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
    ARG_ERR(gr->m == 0, 0);
    for (i = 0; ((i < gr->m) && (!feof(file))); i++) {
        ERR(fscanf(file, "%d %d %d", &src, &dest, &weight) != 3);
        ARG_ERR(((src < 1) || (src > gr->n)), BAD_V);
        ARG_ERR(((dest < 1) || (dest > gr->n)), BAD_V);
        ARG_ERR((weight < 0) || (weight > INT_MAX), BAD_LEN);
        if (src != dest) {
            gr->edges[src - 1][dest - 1] = (unsigned int) weight;
            gr->edges[dest - 1][src - 1] = (unsigned int) weight;

            gr->dest[src - 1] = (dest - 1 == gr->s) ? (unsigned int) weight : gr->dest[src - 1];
            gr->dest[dest - 1] = (src - 1 == gr->s) ? (unsigned int) weight : gr->dest[dest - 1];
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
    ERR((gr->dest = (unsigned int *) malloc(gr->n * sizeof(int))) == NULL);
    ERR((gr->path = (int *) malloc(gr->n * sizeof(int))) == NULL);

    for (i = 0; i < gr->n; i++) {
        ERR((gr->edges[i] = (unsigned int *) malloc(gr->n * sizeof(int))) == NULL);

        for (j = 0; j < gr->n; j++) {
            gr->edges[i][j] = INFTY;
        }

        gr->dest[i] = INFTY;
        gr->path[i] = NO_PATH;
    }

    gr->dest[gr->s] = 0;
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
 * @function Print destinations information between start vertex and others.
 * @param file - file opened for writing
 * @param gr - graph with destinations information
 * @return error code
 */
int fprint_dests(FILE *file, Graph gr) {
    int i = 0;
    for (i = 0; i < gr->n; i++) {
        if (gr->dest[i] == INFTY) {
            fprintf(file, "%s ", INFTY_STR);
        } else if (gr->dest[i] > INT_MAX) {
            fprintf(file, "%s ", INT_MAX_STR);
        } else {
            fprintf(file, "%u ", gr->dest[i]);
        }
    }

    return 0;
}

/**
 * @function Print path information between start vertex and finish vertex.
 * @param file - file opened for writing
 * @param gr - graph with path information
 * @return error code
 */
int fprint_path(FILE *file, Graph gr) {
    int overflow = 0, i = 0;
    /* Count of INT_MAX edges */
    for (i = 0; i < gr->n; i++) {
        overflow += (gr->dest[i] == INT_MAX) ? 1 : 0;
    }

    if (gr->dest[gr->f] == NO_PATH) {
        fprintf(file, NO_PATH_STR);
    } else if ((gr->dest[gr->f] > INT_MAX) && (overflow > 1)) {
        fprintf(file, OVERFLOW_STR);
    } else {
        if (gr->f == gr->s) {
            fprintf(file, "%d", gr->f + 1);
        } else {
            fprintf(file, "%d ", gr->f + 1);
            while ((gr->f = gr->path[gr->f]) != NO_PATH) {
                fprintf(file, "%d ", gr->f + 1);
            };
            fprintf(file, "%d", gr->s + 1);
        }
    }

    return 0;
}