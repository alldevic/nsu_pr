#ifndef LAB2_SOLUTION_H
#define LAB2_SOLUTION_H

/**
 * @def File with source data
 */
#define INPUT "in.txt"

/**
 * @def File for write answer
 */
#define OUTPUT "out.txt"

/**
 * @def The max count vertex in graph
 */
#define MAX_VERTEX 5000

/**
 * @def The max weight edge in graph
 */
#define INT_MAX 2147483647

/**
* @enum Code errors for wrong arguments
 * BAD_NV - bad number of vertices
 * BAD_V - bad vertex
 * BAD_NE - bad number of edges
 * BAD_LEN - bad length
 * BAD_NL - bad number of lines
 */
typedef enum {
    BAD_NV = -10, BAD_V, BAD_NE, BAD_LEN, BAD_NL
} ArgError;

/**
 * @def Macros for throw exception of wrong arguments.
 * @param statement - statement for throw
 * @param code - returns value if statement is true
 */
#define ARG_ERR(statement, code) if (statement) {return code;}

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
 * @def String alias for BAD_LEN (for write answer)
 */
#define BAD_LEN_STR "bad length"

/**
 * @def String alias for BAD_NL (for write answer)
 */
#define BAD_NL_STR "bad number of lines"


typedef struct {
    int parent;
    int rank;
} Subset;

int subset_find(Subset *subsets, int i);

void subset_union(Subset *subsets, int x, int y);


typedef struct {
    int src, dest, weight;
} Edge;

int edge_comp(const void *a, const void *b);

typedef struct {
    int n, m;
    int not_connectivity;
    Edge *min_tree;
    Edge *edge;
} Graph;

void kruskal(Graph *gr);

int read_data(Graph *gr);

int fread_edges(FILE *file, Graph *gr);

int init_arrays(Graph *gr);

char *get_err_str(ArgError code);

void fprint_min_tree(FILE *file, Graph *gr);

void dfs(Graph *gr, int k, int *visited);

#endif /*LAB2_SOLUTION_H*/
