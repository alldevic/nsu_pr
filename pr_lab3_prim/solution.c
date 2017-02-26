#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "solution.h"

#define ERR(x) if (x) {perror(__func__); return errno;};
#define ARG_ERR(statement, code) if (statement) {return code;}

int main() {

    Graph gr = malloc(sizeof(Graph));
    ERR(gr == NULL);
    int er = readData(gr), i = 0;
    char tmp[10] = "";
    ERR(er > 0);
    /*ARG_ERR(er < 0, printAnswer(getStrArgErr((ArgError)er), 1));*/

    if (er < 0)
        return printAnswer(getStrArgErr((ArgError) er), 1);

    prim(gr);
    printAnswer("", 1);
    if ((!gr->n) ||
        (gr->m < (gr->n - 1))
            ) {
        printAnswer("no spanning tree", 0);
    } else {
        for (i = 1; i < gr->n; i++) {
            memset(tmp, 0, strlen(tmp));
            sprintf(tmp, "%d %d\n", gr->minTree[i] + 1, i + 1);
            printAnswer(tmp, 0);
        }
    }

    return 0;
}


int prim(Graph gr) {
    int key[gr->n], count = 0, i = 0, v = 0;
    char mstSet[gr->n];

    for (i = 0; i < gr->n; i++) {
        key[i] = MAX_INT;
        mstSet[i] = 0;
    }

    key[0] = 0;

    for (count = 0; count < gr->n - 1; count++) {
        int min = MAX_INT, u = 0;

        for (v = 0; v < gr->n; v++) {
            if (!mstSet[v] && key[v] < min) {
                min = key[v];
                u = v;
            }
        }

        mstSet[u] = 1;
        for (v = 0; v < gr->n; v++) {
            if (gr->edges[u][v] && !mstSet[v] && gr->edges[u][v] < key[v]) {
                gr->minTree[v] = u;
                key[v] = gr->edges[u][v];
            }
        }
    }
    return 0;
}

int readData(Graph gr) {
    int i = 0, a = 0, b = 0, c = 0;

    FILE *file = fopen(INPUT, "r");
    ERR(file == NULL);

    /*Read 1st line*/
    ERR(fscanf(file, "%d", &(gr->n)) != 1);
    ARG_ERR((gr->n < 0) || (gr->n >= MAX_VERTEX), BAD_NV);

    /*Read 2nd line*/
    ERR(fscanf(file, "%d", &(gr->m)) != 1);
    ARG_ERR((gr->m < 0) || (gr->m > (gr->n * (gr->n + 1) / 2)), BAD_NE);

    /*Read edges data*/
    ERR(initArrays(gr) != 0);
    ARG_ERR(gr->m == 0, 0);
    i = 0;
    while (!feof(file)) {
        ARG_ERR(fscanf(file, "%d %d %d\n", &a, &b, &c) != 3, BAD_NL);
        ARG_ERR(((a < 1) || (a > gr->n)), BAD_V);
        ARG_ERR(((b < 1) || (b > gr->n)), BAD_V);
        ARG_ERR((c < 0) || (c > MAX_INT), BAD_LEN);
        if (a != b) {
            gr->edges[a - 1][b - 1] = (unsigned int) c;
            gr->edges[b - 1][a - 1] = (unsigned int) c;
        }

        i++;
    }
    ARG_ERR(i != (gr->m), BAD_NL);

    ERR(fclose(file) != 0);
    return 0;
}

int initArrays(Graph gr) {
    int i = 0, j = 0;
    ERR((gr->edges = (unsigned int **) malloc(gr->n * sizeof(int *))) == NULL);
    ERR((gr->minTree = (int *) malloc(gr->n * sizeof(int))) == NULL);

    for (i = 0; i < gr->n; i++) {
        ERR((gr->edges[i] = (unsigned int *) malloc(gr->n * sizeof(int))) == NULL);

        for (j = 0; j < gr->n; j++) {
            gr->edges[i][j] = INFTY;
        }
        gr->minTree[i] = 0;
    }
    gr->minTree[1] = ROOT;

    return 0;
}

char *getStrArgErr(ArgError arg) {
    switch (arg) {
        case BAD_NV:
            return "bad number of vertices";
        case BAD_V:
            return "bad vertex";
        case BAD_NE:
            return "bad number of edges";
        case BAD_LEN:
            return "bad length";
        case BAD_NL:
            return "bad number of lines";
    }

    return NULL;
}

int printAnswer(char *str, int fl) {
    FILE *file = fopen(OUTPUT, fl ? "w" : "at");
    ERR(file == NULL);
    ERR(fprintf(file, "%s", str) != strlen(str));
    ERR(fclose(file) != 0);
    return 0;
}
