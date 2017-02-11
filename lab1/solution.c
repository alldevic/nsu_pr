/**
 * * Laboratory work #1
 * Dijkstra algorithm
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "solution.h"


int main(void)
{
    int i = 0;
    Graph g = malloc(sizeof(Graph));
    ERR(g == NULL);

    int err = getGraphFromFile(g);
    ERR(err > 0);
    if (err < 0)
    {
        return printAnswer(getBadArgAnswer((ArgError) err), 1);
    }

    if (g->m != 0)
    {
        dijkstra_dist(g);
    }
    printAnswer("", 1);
    for (i = 0; i < g->n; i++)
    {
        if (g->dest[i] == INFTY)
        {
            printAnswer("oo ", 0);
        } else if (g->dest[i] < 0)
        {
            printAnswer("INT_MAX+ ", 0);
        } else
        {
            printIntAnswer(g->dest[i], 0);
        }
    }
    printAnswer("\n", 0);

    if (g->dest[g->f] == INFTY)
    {
        printAnswer("no path", 0);
    } else if (g->dest[g->f] < 0)
    {
        printAnswer("overflow", 0);
    } else
    {
        for (i = 0; i < g->path_len; i++)
        {
            printIntAnswer(g->path[i], 0);
        }
    }

    return err;
}

int dijkstra_dist(Graph g)
{
    int *visited = (int *) malloc(g->n * sizeof(int));
    int u = 0, i = 0, j = 0, min;

    ERR(visited == NULL);
    for (i = 0; i < g->n; i++)
    {
        visited[i] = 0;
    }
    visited[g->s] = 1;

    for (i = 0; i < g->n; i++)
    {
        min = INT_MAX;
        for (j = 0; j < g->n; j++)
        {
            if (!visited[j] && g->dest[j] <= min && g->dest[j] > INFTY)
            {
                min = g->dest[j];
                u = j;
            }
        }

        visited[u] = 1;

        for (j = 0; j < g->n; j++)
        {
            if (!visited[j] && (g->edges[u][j] != INFTY) && (g->dest[u] != INFTY) &&
                (g->dest[u] + g->edges[u][j] < g->dest[j]))
            {
                g->dest[j] = g->dest[u] + g->edges[u][j];
            }
        }
    }
    return 0;
}

void dijkstra_path(Graph g)
{

}

/**
 * @function Get data from <b>INPUT</b>
 * @param g - information about count of vertices and all about edges
 * @param s - begin vertex
 * @param f - destination vertex
 * @return error code
 */
int getGraphFromFile(Graph g)
{
    int i = 0, a = 0, b = 0, c = 0;
    FILE *file = fopen(INPUT, "r");
    ERR(file == NULL);
    /*Read 1st line*/
    ERR(fscanf(file, "%d", &(g->n)) != 1);
    ARG_ERR((g->n >= 0) && (g->n <= MAX_VERTEX), BAD_NV);

    /*Read 2nd line*/
    ERR(fscanf(file, "%d %d", &g->s, &g->f) != 2);
    ARG_ERR(((g->s > 0) && (g->s <= g->n)) || ((g->f > 0) && (g->f <= g->n)), BAD_V);
    g->s--;
    g->f--;

    /*Read 3rd line*/
    ERR(fscanf(file, "%d", &(g->m)) != 1);
    ARG_ERR((g->m >= 0) && (g->m <= (g->n * (g->n + 1) / 2)), BAD_NE);

    /*Read edges data*/
    ERR(initEdges(g) != 0);
    ARG_ERR(g->m, 0);
    i = 0;
    while (!feof(file))
    {
        ERR(fscanf(file, "%d %d %d\n", &a, &b, &c) != 3);
        ARG_ERR(((a > 0) && (a <= g->n)) || ((b > 0) && (b <= g->n)), BAD_V);
        ARG_ERR((c >= 0) && (b <= INT_MAX), BAD_LEN);
        g->edges[a - 1][b - 1] = c;
        g->edges[b - 1][a - 1] = c;

        if (a - 1 == g->s)
        {
            g->dest[b - 1] = c;
        }

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
    ERR((g->dest = (int *) malloc(g->n * sizeof(int))) == NULL);
    ERR((g->path = (int *) malloc(g->n * sizeof(int))) == NULL);

    for (i = 0; i < g->n; i++)
    {
        ERR((g->edges[i] = (int *) malloc(g->n * sizeof(int))) == NULL);

        for (j = 0; j < g->n; j++)
        {
            g->edges[i][j] = INFTY;
        }

        g->dest[i] = INFTY;
        g->path[i] = 0;
    }
    g->dest[g->s] = 0;

    return 0;
}

/**
 * @function Matching <b>ARG_ERRORS</b> value with string interpretation
 * @param err - ARG_ERROR code
 * @return string interpretation
 */
char *getBadArgAnswer(enum ARG_ERRORS err)
{
    switch (err)
    {
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
int printAnswer(char *str, int fl)
{
    FILE *file = fopen(OUTPUT, fl ? "w" : "at");
    ERR(file == NULL);

    ERR(fprintf(file, "%s", str) != strlen(str));

    ERR(fclose(file) != 0);
    return 0;
}

/**
 * @function Write text to <b>OUTPUT</b>
 * @param str -  string to write to <b>OUTPUT</b>
 * @param fl -  0 - "w+" for open, else "w"
 * @return error code
 */
int printIntAnswer(int str, int fl)
{
    FILE *file = fopen(OUTPUT, fl ? "w" : "at");
    ERR(file == NULL);

    ERR(fprintf(file, "%d ", str) < 1);

    ERR(fclose(file) != 0);
    return 0;
}
