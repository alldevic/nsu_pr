/**
 * Laboratory work N8
 * Binary search tree sorting
 */
#include<stdio.h>
#include<stdlib.h>
#include "solution.h"

/**
 * @function Creating new BST, get data from INPUT and writing sorted for OUTPUT
 * @return error code
 */
int main(void) {
    int err;
    BSTree *newTree = malloc(sizeof(BSTree));
    err = get_input(newTree);
    err = (err != NO_ERR) ? err : set_output(*newTree);
    return err;
}

/**
 * @function Create new node for binary search tree with <b>value</b>
 * @param value - initialisation value for node
 * @return Address of new node or NULL if error.
 */
BSTree bsNodeCreate(int value) {
    BSTree new_node = calloc(1, sizeof(BSNode));
    (*new_node) = (BSNode) {value, NULL, NULL};
    return new_node;
}

/**
 * @function Calculating height of subtree
 * @param tree - node of tree for calculating
 * @return height
 */
int bsGetHeight(BSTree tree) {
    return tree ? max(bsGetHeight(tree->left), bsGetHeight(tree->right)) + 1 : 0;
}

/**@function Return the balancing statement
 * @param tree - subtree for calculating
 * @return diverse of heights
 */
int bsIsBalanced(BSTree tree) {
    return tree ? bsGetHeight(tree->right) - bsGetHeight(tree->left) : 0;
}

/**
 * @function Small right rotate
 * @param tree for rotating
 * @return rotated tree
 */
BSTree bsTreeRotateRight(BSTree tree) {
    BSTree q = tree->left;
    tree->left = q->right;
    q->right = tree;
    return q;
}

/**
 * @function Small left rotate
 * @param tree for rotating
 * @return rotated tree
 */
BSTree bsTreeRotateLeft(BSTree tree) {
    BSTree q = tree->right;
    tree->right = q->left;
    q->left = tree;
    return q;
}

/**
 * @function Balancing tree: small or big rotation
 * @param tree for balancing
 * @return balanced tree
 */
BSTree bsTreeBalancing(BSTree tree) {
    if (bsIsBalanced(tree) == 2) {
        if (bsIsBalanced(tree->right) < 0)
            tree->right = bsTreeRotateRight(tree->right);
        return bsTreeRotateLeft(tree);
    }

    if (bsIsBalanced(tree) == -2) {
        if (bsIsBalanced(tree->left) > 0)
            tree->left = bsTreeRotateLeft(tree->left);
        return bsTreeRotateRight(tree);
    }

    return tree;
}

/**
 * @function Adding node with value to binary search tree
 * @param tree for inserting
 * @param value -- integer data for saving
 * @return address of tree
 */
BSTree bsTreeInsert(BSTree tree, int value) {
    if (!tree) {
        return bsNodeCreate(value);
    }

    if (value < tree->data)
        tree->left = bsTreeInsert(tree->left, value);
    else
        tree->right = bsTreeInsert(tree->right, value);

    return bsTreeBalancing(tree);
}

/**
 * @function Printing tree to OUTPUT by infix tree traversal
 * @param file -- file for output
 * @param node -- root of tree
 * @return
 */
int bsTreePrint(FILE *file, BSTree node) {
    int err;
    if (!node)
        return NO_ERR;
    err = bsTreePrint(file, node->left);
    if (err == NO_ERR)
        err = fprintf(file, "%d ", node->data) < 0 ? ERR_PRINT : NO_ERR;
    if (err == NO_ERR)
        err = bsTreePrint(file, node->right);
    return err;
}

/**
* @function Fill the <b>tree</b> from INPUT.
* @param tree - binary search tree
* @return err - error code
*/
int get_input(BSTree *tree) {
    FILE *file;
    int err = NO_ERR, i = 0, n = 0, m = 0;
    if ((file = fopen(INPUT, "r")) != NULL) {
        err = (fscanf(file, "%d", &n) < 0) ? ERR_SCAN : err;
        for (i = 0; !err && (i < n); i++) {
            err = (fscanf(file, "%d", &m) < 0) ? ERR_SCAN : err;
            *tree = bsTreeInsert(*tree, m);
        }
        err = (!err && (fclose(file) == EOF)) ? ERR_CLOSE : err;
    } else
        err = ERR_OPEN;
    return err;
}

/**
 * @function Write binary search tree <b>tree</b> to OUTPUT
 * @param tree - binary search tree
 * @return err - error code
 */
int set_output(BSTree tree) {
    FILE *file;
    int err = ((file = fopen(OUTPUT, "w")) == NULL) ? ERR_CREATE : NO_ERR;
    if (err == NO_ERR)
        err = bsTreePrint(file, tree);
    return ((fclose(file) == EOF) && !err) ? ERR_CLOSE : err;
}
