#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "solution.h"

#define ERR(x) if (x) {perror(__func__); return errno;};
#define ARG_ERR(statement, code) if (statement) {return code;}

int main() {
    Graph *gr = malloc(sizeof(Graph));
    ERR(gr == NULL);
    int er = readData(gr), i = 0;
    char tmp[10] = "";
    ERR(er > 0);
    /*ARG_ERR(er < 0, printAnswer(getStrArgErr((ArgError)er), 1));*/

    if (er < 0)
        return printAnswer(getStrArgErr((ArgError) er), 1);

    kruskal(gr);
    printAnswer("", 1);
    for (i = 0; i < gr->n - 1; i++) {
        memset(tmp, 0, strlen(tmp));
        sprintf(tmp, "%d %d\n", gr->minTree[i].src, gr->minTree[i].dest);
        printAnswer(tmp, 0);
    }
    return 0;
}

int kruskal(Graph *gr) {
    int j = 0, i = 0, v = 0;
    qsort(gr->edges, (size_t) gr->m, sizeof(gr->edges[0]), myComp);
    Subset *subsets = (Subset *) malloc(gr->m * sizeof(Subset));

    for (v = 0; v < gr->n; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    while (j < gr->n - 1) {
        Edge next_edge = gr->edges[i++];

        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);

        if (x != y) {
            gr->minTree[j++] = next_edge;
            Union(subsets, x, y);
        }
    }

    return 0;
}

void Union(Subset *subsets, int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

int find(Subset *subsets, int x) {


    if (subsets[x].parent != x)
        subsets[x].parent = find(subsets, subsets[x].parent);

    return subsets[x].parent;
}

int myComp(const void *a, const void *b) {
    Edge *a1 = (Edge *) a;
    Edge *b1 = (Edge *) b;
    return a1->weight > b1->weight;
}

int readData(Graph *gr) {
    int i = 0, a = 0, b = 0, c = 0;

    FILE *file = fopen(INPUT, "r");
    ERR(file == NULL);

    /*Read 1st line*/
    ERR(fscanf(file, "%d", &(gr->n)) != 1);
    ARG_ERR((gr->n < 0) || (gr->n >= MAX_VERTEX), BAD_NV);

    /*Read 3rd line*/
    ERR(fscanf(file, "%d", &(gr->m)) != 1);
    ARG_ERR((gr->m < 0) || (gr->m > (gr->n * (gr->n + 1) / 2)), BAD_NE);

    /*Read edges data*/
    ERR((gr->edges = (Edge *) malloc(gr->n * sizeof(Edge))) == NULL);
    ERR((gr->minTree = (Edge *) malloc(gr->n * sizeof(Edge))) == NULL);
    ARG_ERR(gr->m == 0, 0);
    i = 0;
    while (!feof(file)) {
        ERR(fscanf(file, "%d %d %d\n", &a, &b, &c) != 3);
        ARG_ERR(((a < 1) || (a > gr->n)), BAD_V);
        ARG_ERR(((b < 1) || (b > gr->n)), BAD_V);
        ARG_ERR((c < 0) || (c > MAX_INT), BAD_LEN);
        if (a != b) {
            gr->edges[i].src = a;
            gr->edges[i].dest = b;
            gr->edges[i].weight = (unsigned int) c;
        }

        i++;
    }
    ARG_ERR(i != (gr->m), BAD_NL);

    ERR(fclose(file) != 0);
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
