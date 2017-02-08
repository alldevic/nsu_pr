#ifndef LAB1_SOLUTION_H
#define LAB1_SOLUTION_H

/**
 * #def File with source values
 * Default for problem: <b>"in.txt"</b>
 */
#define INPUT "in.txt"

/**
 * @def File with results<br>
 * Default for problem: <b>"out.txt"</b>
 */
#define OUTPUT "out.txt"

/**
 * @def Max number of vertexes in graph
 */
#define MAX_VERTEX 5000

/**
 * @def Length of edge between unreachable vertexes
 */
#define INT_MAX 2147483647

/**
 * @enum List of errors:<ul>
 * <li>ERR_NO - successful
 * <li>ERR_OPEN - problem with <b>INPUT</b>
 * <li>ERR_CREATE - problem with <b>OUTPUT</b>
 * <li>ERR_SCAN - can't read from <b>INPUT</b>
 * <li>ERR_PRINT - can't write to <b>OUTPUT</b>
 * <li>ERR_CLOSE - can't correct close file
 * <li>ERR_ALLOC - can't allocation memory
 * <li>ERR_NULL - nullable argument
 * <li>ERR_ARG - bad argument
 */
typedef enum ERRORS {
    ERR_NO, ERR_OPEN, ERR_CREATE, ERR_SCAN,
    ERR_PRINT, ERR_CLOSE, ERR_ALLOC, ERR_NULL,
    ERR_ARG
} Error;

typedef enum ARG_ERRORS {
    BAD_NV = 1, BAD_V, BAD_NE, BAD_LEN, BAD_NL
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
 * @def Macros for trapping errors from <b>ERRORS</b> enum
 */
#define ERR(x, y) if (!(x)) return y;

/**
 * @typedef Structure for storage information about graph:
 */
typedef struct graph {
    int n; /**Number of vertices*/
    int m; /**Number of edges*/
    int **edges; /**Matrix of values*/
} *Graph;

int getGraphFromFile(Graph g, int *s, int *f);

char *getBadArgAnswer(ArgError err);

#endif /*LAB1_SOLUTION_H*/
