#ifndef LAB1_SOLUTION_H
#define LAB1_SOLUTION_H

#define INPUT "in.txt"
#define OUTPUT "out.txt"

#define MAX_VERTEX 5000
#define MAX_INT 2147483647
#define INFTY (2U*MAX_INT)+1
#define NO_PATH -1

#define INFTY_STR "oo "
#define MAX_INT_STR "INT_MAX+ "
#define NO_PATH_STR "no path"
#define OVERFLOW_STR "overflow"

typedef enum {
    BAD_NV = -10, BAD_V, BAD_NE, BAD_LEN, BAD_NL
} ArgError;

#define BAD_NV_STR "bad number of vertices"
#define BAD_V_STR "bad vertex"
#define BAD_NE_STR "bad number of edges"
#define BAD_LEN_STR "bad length"
#define BAD_NL_STR "bad number of lines"

typedef struct {
    int n, m, s, f;
    int *path;
    unsigned int *dest;
    unsigned int **edges;
} *Graph;


int readData(Graph gr);

int initArrays(Graph gr);

char *getStrArgErr(ArgError arg);

void dijkstra(Graph gr);

int fprint_dests(FILE *file, Graph gr);

int fprint_path(FILE *file, Graph gr);

int fread_edges(FILE *file, Graph gr);
#endif /*LAB1_SOLUTION_H*/
