/**
 * Laboratory work #1
 * Dijkstra algorithm
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include "solution.h"

int main(void) {
    int i = 0;
    Graph g = malloc(sizeof(Graph));
    ERR(g == NULL);
    int s = 0, f = 0;
    char answer[20];
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
    int i = 0, j = 0;
    int a = 0, b = 0, c = 0;
    FILE *file = fopen(INPUT, "r");
    ERR(file == NULL);

    /*Read 1st line*/
    ERR(fscanf(file, "%d", &(g->n)) == EOF);
    ARG_ERR((g->n >= 0) && (g->n <= MAX_VERTEX), BAD_NV);

    /*Read 2nd line*/
    ERR(fscanf(file, "%d %d", s, f) != 2);
    ARG_ERR((*s > 0) && (*s <= g->n), BAD_V);
    ARG_ERR((*f > 0) && (*f <= g->n), BAD_V);

    /*Read 3rd line*/
    ERR(fscanf(file, "%d", &(g->m)) != 1);
    ARG_ERR((g->m >= 0) && (g->m <= (g->n * (g->n + 1) / 2)), BAD_NE);

    /*Allocation memory for edges array*/
    g->edges = (int **) malloc(g->n * sizeof(int *));
    ERR(g->edges == NULL);

    for (i = 0; i < g->n; i++) {
        g->edges[i] = (int *) malloc(g->n * sizeof(int));
        ERR(g->edges[i] == NULL);
        for (j = 0; j < g->n; j++) {
            g->edges[i][j] = INFTY;
        }
    }

    /*Read edges data*/
    i = 0;
    while (!feof(file)) {
        ERR(fscanf(file, "%d %d %d", &a, &b, &c) != 3);
        ARG_ERR((a > 0) && (a <= g->n), BAD_V);
        ARG_ERR((b > 0) && (b <= g->n), BAD_V);
        ARG_ERR((c >= 0) && (b <= INT_MAX), BAD_LEN);
        g->edges[a - 1][b - 1] = c;
        i++;
    }
    ARG_ERR(i == g->m, BAD_NL);

    ERR(fclose(file) != 0);
    return 0;
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
 * @return error code
 */
int printAnswer(char *str, int fl) {
    FILE *file = fopen(OUTPUT, fl ? "w" : "w+");
    ERR(file == NULL);
    ERR(fprintf(file, "%s", str) != strlen(str));
    ERR(fclose(file) != 0);
    return 0;
}
