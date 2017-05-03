#ifndef LAB5_LIST_H
#define LAB5_LIST_H
/**
 * Implementation of tree structure
 */
typedef struct Node {
    size_t size;
    unsigned char ch;

    struct Node *left;
    struct Node *right;
} Node;

/**
 * Implementation of list structure with tree nodes as data
 */
typedef struct List {
    Node *node;

    struct List *next;
    struct List *pre;
} List;

int l_push(List **list, Node *l);

Node *l_pop(List **list);

int l_size(List *list);

Node *combine(Node *left, Node *right);

void l_qsort(List **list);

#endif /*LAB5_LIST_H*/
