#ifndef LAB1_SOLUTION_H
#define LAB1_SOLUTION_H

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
    int n, m, s, f;
    int *path;
    unsigned int *dest;
    unsigned int **edges;
} *Graph;


int readData(Graph gr);

int initArrays(Graph gr);

char *getStrArgErr(ArgError arg);

int printAnswer(char *str, int fl);

void dijkstra(Graph gr);

#endif /*LAB1_SOLUTION_H*/
