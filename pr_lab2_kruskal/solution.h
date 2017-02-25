#ifndef LAB2_SOLUTION_H
#define LAB2_SOLUTION_H

#define INPUT "in.txt"
#define OUTPUT "out.txt"


#define MAX_VERTEX 5000
#define MAX_INT 2147483647
#define INFTY (2U*MAX_INT)+1
#define NO_PATH -1

typedef enum {
    BAD_NV = -10, BAD_V, BAD_NE, BAD_LEN, BAD_NL
} ArgError;

typedef struct {
    int src, dest;
    unsigned int weight;
} Edge;

typedef struct {
    int n, m;
    Edge *edges, *minTree;
} Graph;

typedef struct {
    int parent;
    int rank;
} Subset;


int readData(Graph *gr);

int kruskal(Graph *gr);

int myComp(const void* a, const void* b);

int find(Subset *subsets, int i);

void Union(Subset *subsets, int x, int y);

char *getStrArgErr(ArgError arg);

int printAnswer(char *str, int fl);

#endif /*LAB2_SOLUTION_H*/