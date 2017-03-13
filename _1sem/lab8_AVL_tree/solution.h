#ifndef LAB8_SOLUTION_H
#define LAB8_SOLUTION_H

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
 * @def Macros for search max of two values
 */
#define max(a, b) a>b?a:b

/**
 * @enum List of errors:<ul>
 * <li>NO_ERR - successful
 * <li>ERR_OPEN - problem with <b>INPUT</b>
 * <li>ERR_CREATE - problem with <b>OUTPUT</b>
 * <li>ERR_SCAN - can't read from <b>INPUT</b>
 * <li>ERR_PRINT - can't wtite to <b>OUTPUT</b>
 * <li>ERR_CLOSE - can't correct close file
 * <li>ERR_ALLOC - can't allocation memory
 * <li>ERR_NULL - nullable argument
 */
enum ERRORS {
    NO_ERR, ERR_OPEN, ERR_CREATE, ERR_SCAN,
    ERR_PRINT, ERR_CLOSE, ERR_ALLOC, ERR_NULL
};

/**
 * Description of binary search tree and node of BST.
 */
typedef struct bsNode {
    int data;
    struct bsNode *left;
    struct bsNode *right;
} BSNode, *BSTree;

BSTree bsNodeCreate(int value);

int bsGetHeight(BSTree tree);

int bsIsBalanced(BSTree tree);

BSTree bsTreeRotateRight(BSTree tree);

BSTree bsTreeRotateLeft(BSTree tree);

BSTree bsTreeBalancing(BSTree tree);

BSTree bsTreeInsert(BSTree tree, int value);

int bsTreePrint(FILE *file, BSTree node);

int get_input(BSTree *tree);

int set_output(BSTree tree);

#endif /*LAB8_SOLUTION_H*/
