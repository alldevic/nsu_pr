#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "solution.h"

#define ERR(x) if (x) {perror(__func__); return errno;};
#define ARG_ERR(statement, code) if (statement) {return code;}

int main() {
    int i = 0, ov = 0;
    char tmp[10] = "";
    Graph gr = malloc(sizeof(Graph));
    ERR(gr == NULL);
    int er = readData(gr);
    ERR(er > 0);
    /*ARG_ERR(er < 0, printAnswer(getStrArgErr((ArgError)er), 1));*/

    if (er < 0) {
        return printAnswer(getStrArgErr((ArgError) er), 1);
    }

    dijkstra(gr);
    printAnswer("", 1);

    for (i = 0; i < gr->n; i++) {
        if (gr->dest[i] == INFTY) {
            printAnswer("oo ", 0);
        } else if (gr->dest[i] > MAX_INT) {
            printAnswer("INT_MAX+ ", 0);
        } else {
            ov += (gr->dest[i] == MAX_INT) ? 1 : 0;
            sprintf(tmp, "%u ", gr->dest[i]);
            printAnswer(tmp, 0);
        }
    }
    printAnswer("\n", 0);


    if (gr->dest[gr->f] == NO_PATH) {
        printAnswer("no path", 0);
    } else if ((gr->dest[gr->f] > MAX_INT) && (ov > 1)) {
        printAnswer("overflow", 0);
    } else {
        if (gr->f == gr->s) {
            memset(tmp, 0, strlen(tmp));
            sprintf(tmp, "%d ", gr->f + 1);
            printAnswer(tmp, 0);
        } else {
/*            for (i = 0; i < gr->n; i++)
                fprintf(stdout, "%d ", gr->path[i]);*/
            memset(tmp, 0, strlen(tmp));
            sprintf(tmp, "%d ", gr->f + 1);
            printAnswer(tmp, 0);

            while ((gr->f = gr->path[gr->f]) != NO_PATH) {
                memset(tmp, 0, strlen(tmp));
                sprintf(tmp, "%d ", gr->f + 1);
                printAnswer(tmp, 0);
            };
            memset(tmp, 0, strlen(tmp));
            sprintf(tmp, "%d", gr->s + 1);
            printAnswer(tmp, 0);

        }
    }


    return 0;
}

void dijkstra(Graph gr) {
    int i = 0, j = 0, u = 0, min;
    char *visited = (char *) malloc(gr->n * sizeof(char));
    for (i = 0; i < gr->n; i++) {
        visited[i] = 0;
    }

    visited[gr->s] = 1;
    for (i = 0; i < gr->n; i++) {
        min = MAX_INT;
        for (j = 0; j < gr->n; j++) {
            if (!visited[j] && gr->dest[j] <= min) {
                min = gr->dest[j];
                u = j;
            }
        }
        visited[u] = 1;

        for (j = 0; j < gr->n; j++) {
            if (!visited[j] && (gr->edges[u][j] + gr->dest[u] < gr->dest[j]) &&
                (gr->edges[u][j] <= MAX_INT) && (gr->dest[u] <= MAX_INT)) {
                gr->dest[j] = gr->edges[u][j] + gr->dest[u];
                gr->path[j] = u;
            }
        }
    }
}

int readData(Graph gr) {
    int i = 0, a = 0, b = 0, c = 0;

    FILE *file = fopen(INPUT, "r");
    ERR(file == NULL);

    /*Read 1st line*/
    ERR(fscanf(file, "%d", &(gr->n)) != 1);
    ARG_ERR((gr->n < 0) || (gr->n >= MAX_VERTEX), BAD_NV);

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
    ERR(initArrays(gr) != 0);
    ARG_ERR(gr->m == 0, 0);
    for (i = 0; ((i < gr->m) && (!feof(file))); i++) {
        ERR(fscanf(file, "%d %d %d", &a, &b, &c) != 3);
        ARG_ERR(((a < 1) || (a > gr->n)), BAD_V);
        ARG_ERR(((b < 1) || (b > gr->n)), BAD_V);
        ARG_ERR((c < 0) || (c > MAX_INT), BAD_LEN);
        if (a != b) {
            gr->edges[a - 1][b - 1] = (unsigned int) c;
            gr->edges[b - 1][a - 1] = (unsigned int) c;

            gr->dest[a - 1] = (b - 1 == gr->s) ? (unsigned int) c : gr->dest[a - 1];
            gr->dest[b - 1] = (a - 1 == gr->s) ? (unsigned int) c : gr->dest[b - 1];
        }
    }
    ARG_ERR(i != (gr->m), BAD_NL);

    ERR(fclose(file) != 0);
    return 0;
}

int initArrays(Graph gr) {
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
