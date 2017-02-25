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
    int er = readData(gr);
    ERR(er > 0);
    /*ARG_ERR(er < 0, printAnswer(getStrArgErr((ArgError)er), 1));*/

    if (er < 0)
        return printAnswer(getStrArgErr((ArgError) er), 1);

    printAnswer("", 1);

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
        ERR(fscanf(file, "%d %d %d\n", &a, &b, &c) != 3);
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
    
    for (i = 0; i < gr->n; i++) {
        ERR((gr->edges[i] = (unsigned int *) malloc(gr->n * sizeof(int))) == NULL);

        for (j = 0; j < gr->n; j++){
            gr->edges[i][j] = INFTY;
        }

    }
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