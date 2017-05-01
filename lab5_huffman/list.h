#ifndef LAB5_LIST_H
#define LAB5_LIST_H

typedef struct Node {
    size_t size;
    unsigned char ch;

    struct Node *left;
    struct Node *right;
} Node;

typedef struct List {
    Node *node;

    struct List *next;
    struct List *pre;
} List;

void l_push_front(List **tree, Node *l);

Node *l_pop_front(List **tree);

int l_size(List *tree);

Node *combine(Node *left, Node *right);

void l_qsort(List **tree);

#endif /*LAB5_LIST_H*/
