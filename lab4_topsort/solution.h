//TODO: docs

#ifndef LAB4_SOLUTION_H
#define LAB4_SOLUTION_H

/**
 * @def File with source data
 */
#define INPUT "in.txt"
#define OUTPUT "out.txt"
#define MAX_VERTEX 1000
#define IMPOSSIBLE_STR "impossible to sort"


typedef enum {
    WHITE = 0, GRAY, BLACK
} VertexColor;

/**
* @enum Code errors for wrong arguments
 * BAD_NV - bad number of vertices
 * BAD_V - bad vertex
 * BAD_NE - bad number of edges
 * BAD_NL - bad number of lines
 */
typedef enum {
    BAD_NV = -10, BAD_V, BAD_NE, BAD_NL
} ArgError;

/**
 * @def Macros for throw exception of wrong arguments.
 * @param statement - statement for throw
 * @param code - returns value if statement is true
 */
#define ARG_ERR(statement, code) {if (statement) {return code;}}

/**
 * @def String alias for BAD_NV (for write answer)
 */
#define BAD_NV_STR "bad number of vertices"

/**
 * @def String alias for BAD_V (for write answer)
 */
#define BAD_V_STR "bad vertex"

/**
 * @def String alias for BAD_NE (for write answer)
 */
#define BAD_NE_STR "bad number of edges"

/**
 * @def String alias for BAD_NL (for write answer)
 */
#define BAD_NL_STR "bad number of lines"

typedef struct intList {
    int data;
    struct intList *next;
} *IntList;

/**
 * @typedef Data structure for saving graph data
 */
typedef struct {
    int n;                 /** Count of vertex */
    int m;                 /** Count of edges */
    int is_sorting;       /** Flag of connectivity of graph */
    IntList *data;
    IntList sorted;
} *Graph;

int top_sort(Graph gr, int k, int *visited);

int read_data(Graph gr);

int fread_edges(FILE *file, Graph gr);

char *get_err_str(ArgError code);

void fprint_sorted(FILE *file, Graph gr);

#endif /*LAB4_SOLUTION_H*/
