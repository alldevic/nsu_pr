#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define INPUT "in.txt"
#define OUTPUT "out.txt"
#define MAX_VERTEX 5000
#define INT_MAX 2147483647
#define ROOT -1
#define ARG_ERR(statement, code) if (statement) {return code;}
#define BAD_NV_STR "bad number of vertices"
#define BAD_V_STR "bad vertex"
#define BAD_NE_STR "bad number of edges"
#define BAD_LEN_STR "bad length"
#define BAD_NL_STR "bad number of lines"
#define ERR(x) if (x) {perror(__func__); return errno;};

typedef enum {
    BAD_NV = -10, BAD_V, BAD_NE, BAD_LEN, BAD_NL
} ArgError;

typedef struct {
    int n;                 /** Count of vertex */
    int m;                 /** Count of edges */
    int not_connectivity;  /** Flag of connectivity of graph*/
    int *min_tree;         /** The minimum spanning tree */
    unsigned int **edges;  /** Adjacency matrix for graph */
} *Graph;

void prim(Graph gr);

int read_data(Graph gr);

int fread_edges(FILE *file, Graph gr);

int init_arrays(Graph gr);

char *get_err_str(ArgError code);

void fprint_min_tree(FILE *file, Graph gr);

void dfs(Graph gr, int k, int *visited);

int main(void) {
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
    if (!gr->not_connectivity)
        prim(gr);
    fprint_min_tree(file, gr);
    fclose(file);
    return 0;
}

void prim(Graph gr) {
    int weight[gr->n], i = 0, j = 0, min, u = 0;
    char visited[gr->n];

    for (i = 0; i < gr->n; i++) {
        weight[i] = INT_MAX;
        visited[i] = 0;
    }
    weight[0] = 0;
    for (i = 0; i < gr->n - 1; i++) {
        min = INT_MAX;
        u = 0;

        for (j = 0; j < gr->n; j++) {
            if (!visited[j] && weight[j] < min) {
                min = weight[j];
                u = j;
            }
        }

        visited[u] = 1;
        for (j = 0; j < gr->n; j++) {
            if (gr->edges[u][j] && !visited[j] && gr->edges[u][j] < weight[j]) {
                gr->min_tree[j] = u;
                weight[j] = gr->edges[u][j];
            }
        }
    }
}

int read_data(Graph gr) {
    int er; /* Error code for fread_edges */
    FILE *file = fopen(INPUT, "r");
    ERR(file == NULL);

    /*Read 1st line*/
    ERR(fscanf(file, "%d", &(gr->n)) != 1);
    ARG_ERR((gr->n < 0) || (gr->n > MAX_VERTEX), BAD_NV);

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

int fread_edges(FILE *file, Graph gr) {
    int i = 0, src = 0, dest = 0, weight = 0;
    ARG_ERR(!gr->m, 0);
    for (i = 0; ((i < gr->m) && (!feof(file))); i++) {
        if (fscanf(file, "%d %d %d", &src, &dest, &weight) != 3) {
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

    /* Check graph for connectivity */
    int *visited = calloc((size_t) gr->n, sizeof(int));
    dfs(gr, 0, visited);
    gr->not_connectivity = 0;
    for (i = 0; i < gr->n; i++) {
        gr->not_connectivity += (visited[i]) ? 0 : 1;
    }

    return 0;
}

int init_arrays(Graph gr) {
    int i = 0, j = 0;
    ERR((gr->edges = (unsigned int **) malloc(gr->n * sizeof(int *))) == NULL);
    ERR((gr->min_tree = (int *) malloc(gr->n * sizeof(int))) == NULL);

    for (i = 0; i < gr->n; i++) {
        ERR((gr->edges[i] = (unsigned int *) malloc(gr->n * sizeof(int))) == NULL);

        for (j = 0; j < gr->n; j++) {
            gr->edges[i][j] = 0;
        }
        gr->min_tree[i] = i - 1;
    }
    gr->min_tree[0] = ROOT;
    return 0;
}

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
    if ((gr->n == 1) && (gr->m == 0)) {

    } else if ((!gr->n) || (gr->m < (gr->n - 1)) || gr->not_connectivity) {
        fprintf(file, "no spanning tree");
    } else {
        for (i = 1; i < gr->n; i++) {
            fprintf(file, "%d %d\n", gr->min_tree[i] + 1, i + 1);
        }
    }
}

void dfs(Graph gr, int k, int *visited) {
    visited[k] = 1;

    int i = 0;
    for (i = 0; i < gr->n; i++) {
        if (!visited[i] && gr->edges[i][k] != 0)
            dfs(gr, i, visited);
    }
}
