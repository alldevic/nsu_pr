/**
 * Laboratory work #1
 * Dijkstra algorithm
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "solution.h"

int main(void) {
    Graph g = malloc(sizeof(Graph));
    ERR(g == NULL);
    int s = 0, f = 0;
    int err = getGraphFromFile(g, &s, &f);
    ERR(err > 0);
    if (err < 0) { return printAnswer(getBadArgAnswer((ArgError) err), 1); }


    return err;
}

/**
 * @function Get data from <b>INPUT</b>
 * @param g - information about count of vertices and all about edges
 * @param s - begin vertex
 * @param f - destination vertex
 * @return error code
 */
int getGraphFromFile(Graph g, int *s, int *f) {
    int i = 0, a = 0, b = 0, c = 0;
    FILE *file = fopen(INPUT, "r");
    ERR(file == NULL);

    /*Read 1st line*/
    ERR(fscanf(file, "%d", &(g->n)) != 1);
    ARG_ERR((g->n >= 0) && (g->n <= MAX_VERTEX), BAD_NV);

    /*Read 2nd line*/
    ERR(fscanf(file, "%d %d", s, f) != 2);
    ARG_ERR(((*s > 0) && (*s <= g->n)) || ((*f > 0) && (*f <= g->n)), BAD_V);

    /*Read 3rd line*/
    ERR(fscanf(file, "%d", &(g->m)) != 1);
    ARG_ERR((g->m >= 0) && (g->m <= (g->n * (g->n + 1) / 2)), BAD_NE);
    ARG_ERR(g->m, 0);

    /*Read edges data*/
    ERR(initEdges(g) != 0);
    i = 0;
    while (!feof(file)) {
        ERR(fscanf(file, "%d %d %d", &a, &b, &c) != 3);
        ARG_ERR(((a > 0) && (a <= g->n)) || ((b > 0) && (b <= g->n)), BAD_V);
        ARG_ERR((c >= 0) && (b <= INT_MAX), BAD_LEN);
        g->edges[a - 1][b - 1] = c;
        i++;
    }
    ARG_ERR(i == g->m, BAD_NL);

    ERR(fclose(file) != 0);
    return 0;
}

/**
 * @function Allocation memory for edges array in graph
 * @param g - graph for allocation
 * @return error code
 */
int initEdges(Graph g)
{
    int i = 0, j = 0;
    ERR((g->edges = (int **) malloc(g->n * sizeof(int *))) == NULL);
    for (i = 0; i < g->n; i++) {
        ERR((g->edges[i] = (int *) malloc(g->n * sizeof(int))) == NULL);
        for (j = 0; j < g->n; j++) {
            g->edges[i][j] = INFTY;
        }
    }
}

/**
 * @function Matching <b>ARG_ERRORS</b> value with string interpretation
 * @param err - ARG_ERROR code
 * @return string interpretation
 */
char *getBadArgAnswer(enum ARG_ERRORS err) {
    switch (err) {
        case BAD_NV:
            return BAD_NV_ANS;
        case BAD_V:
            return BAD_V_ANS;
        case BAD_NE:
            return BAD_NE_ANS;
        case BAD_LEN:
            return BAD_LEN_ANS;
        case BAD_NL:
            return BAD_NL_ANS;
    }
    return NULL;
}

/**
 * @function Write text to <b>OUTPUT</b>
 * @param str -  string to write to <b>OUTPUT</b>
 * @param fl -  0 - "w+" for open, else "w"
 * @return error code
 */
int printAnswer(char *str, int fl) {
    FILE *file = fopen(OUTPUT, fl ? "w" : "w+");
    ERR(file == NULL);
    ERR(fprintf(file, "%s", str) != strlen(str));
    ERR(fclose(file) != 0);
    return 0;
}
