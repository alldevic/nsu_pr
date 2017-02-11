#ifndef LAB1_SOLUTION_H
#define LAB1_SOLUTION_H

#include <errno.h>

/**
 * @def Macros for trapping errors
 */
#define ERR(x) {if (x) {perror(__func__); return errno;}};

/**
 * @def File with source values
 * Default for problem: <b>"in.txt"</b>
 */
#define INPUT "in.txt"

/**
 * @def File with results<br>
 * Default for problem: <b>"out.txt"</b>
 */
#define OUTPUT "out.txt"

/**
 * @def Max number of vertices in graph
 */
#define MAX_VERTEX 5000

/**
 * @def Length of edge between unreachable vertices
 */
#define INT_MAX 2147483647

/**
 * @def Unreachable destinantion
 */
#define INFTY -1

/**
 * @typedef List of errors of argument:
 * <li> BAD_NV - bad number of vertices
 * <li> BAD_V - bad vertex
 * <li> BAD_NE - bad number of edges
 * <li> BAD_LEN - bad length
 * <li> BAD_NL - bad number of lines
 */
typedef enum ARG_ERRORS
{
    BAD_NV = -10, BAD_V, BAD_NE, BAD_LEN, BAD_NL
} ArgError;

/**
 * @def String interpretation of <b>BAD_NV</b>
 * in <b>ARG_ERRORS</b> enum
 */
#define BAD_NV_ANS "bad number of vertices"

/**
 * @def String interpretation of <b>BAD_V</b>
 * in <b>ARG_ERRORS</b> enum
 */
#define BAD_V_ANS "bad vertex"

/**
 * @def String interpretation of <b>BAD_NE</b>
 * in <b>ARG_ERRORS</b> enum
 */
#define BAD_NE_ANS "bad number of edges"

/**
 * @def String interpretation of <b>BAD_LEN</b>
 * in <b>ARG_ERRORS</b> enum
 */
#define BAD_LEN_ANS "bad length"

/**
 * @def String interpretation of <b>BAD_NL</b>
 * in <b>ARG_ERRORS</b> enum
 */
#define BAD_NL_ANS "bad number of lines"

/**
 * @def Return error code for error statement
 */
#define ARG_ERR(statement, code) if (!(statement)) return code;

/**
 * @typedef Structure for storage information about graph:
 */
typedef struct graph
{
    int n;        /**  Number of vertices */
    int m;        /**  Number of edges */
    int **edges;  /**  Matrix of values */
    int s;        /**  Start point */
    int f;        /**  Finish point */
    int *dest;    /**  Table of destinations from S */
    int *path;    /**  Path from start to finish */
} *Graph;

int dijkstra_dist(Graph g);

int getGraphFromFile(Graph g);

int initEdges(Graph g);

char *getBadArgAnswer(ArgError err);

int printAnswer(char *str, int fl);

int printIntAnswer(int str, int fl);

#endif /*LAB1_SOLUTION_H*/
