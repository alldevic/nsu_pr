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
    int s = 0, f = 0;
    char *answer = "";
    int err = getGraphFromFile(g, &s, &f);
    if (err != ERR_NO) {
        if (err % ERR_ARG != 0) {
            return err;
        }
        ERR((answer = getBadArgAnswer((ArgError) (err / ERR_ARG))) != NULL, ERR_NULL);
        return printAnswer(answer);
    }


    printAnswer(answer);
    return ERR_NO;
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
    ERR(file != NULL, ERR_OPEN);
    /*Read 1s line*/
    ERR(fscanf(file, "%d", &(g->n)) == 1, ERR_SCAN);
    ERR((g->n >= 0) && (g->n <= MAX_VERTEX), BAD_NV * ERR_ARG);

    /*Read 2nd line*/
    ERR(fscanf(file, "%d %d", s, f) == 2, ERR_SCAN);
    ERR((*s > 0) && (*s <= g->n), BAD_V * ERR_ARG);
    ERR((*f > 0) && (*f <= g->n), BAD_V * ERR_ARG);

    /*Read 3rd line*/
    ERR(fscanf(file, "%d", &(g->m)) == 1, ERR_SCAN);
    ERR((g->m >= 0) && (g->m <= (g->n * (g->n + 1) / 2)), BAD_NE * ERR_ARG);

    /*Allocation memory for edges array*/
    g->edges = (int **) malloc(g->n * sizeof(int *));
    ERR(g->edges != NULL, ERR_ALLOC);

    for (i = 0; i < g->n; i++) {
        g->edges[i] = (int *) malloc(g->n * sizeof(int));
        ERR(g->edges[i] != NULL, ERR_ALLOC);
        for (j = 0; j < g->n; j++) {
            g->edges[i][j] = INT_MAX;
        }
    }

    /*Read edges data*/
    i = 0;
    while (!feof(file)) {
        ERR(fscanf(file, "%d %d %d", &a, &b, &c) == 3, ERR_SCAN);
        ERR((a > 0) && (a <= g->n), BAD_V * ERR_ARG);
        ERR((b > 0) && (b <= g->n), BAD_V * ERR_ARG);
        ERR((c >= 0) && (b <= INT_MAX), BAD_LEN * ERR_ARG);
        g->edges[a - 1][b - 1] = c;
        i++;
    }
    ERR(i == g->m, BAD_NL * ERR_ARG);

    ERR(fclose(file) == 0, ERR_CLOSE);
    return ERR_NO;
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
int printAnswer(char *str) {
    FILE *file = fopen(OUTPUT, "w");
    ERR(file != NULL, ERR_CREATE);
    ERR(fprintf(file, "%s", str) == strlen(str), ERR_PRINT);
    ERR(fclose(file) == 0, ERR_CLOSE);
    return ERR_NO;
}
