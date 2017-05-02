#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "errors.h"

int l_push_front(List **tree, Node *l) {
    if (!*tree) {
        List *list = calloc(1, sizeof(List));
        ERR(list == NULL);

        list->node = l, *tree = list;
    } else {
        List *front = calloc(1, sizeof(List));
        ERR(front == NULL);

        (*tree)->pre = front, front->next = *tree, front->node = l;
        *tree = front;
    }
    return 0;
}

Node *l_pop_front(List **tree) {
    Node *l;
    if (!*tree) {
        return NULL;
    }

    l = (*tree)->node;

    if ((*tree)->next) {
        *tree = (*tree)->next, free((*tree)->pre), (*tree)->pre = NULL;
    } else {
        free(*tree), *tree = NULL;
    }

    return l;
}

int l_size(List *tree) {
    int count = 0;
    for (count = 1; tree && tree->next; tree = tree->next, count++);
    return count;
}

Node *combine(Node *left, Node *right) {
    Node *node;
    if (!(node = calloc(1, sizeof(Node)))) {
        return NULL;
    }

    node->size = left->size + right->size;
    node->left = left, node->right = right;
    return node;
}

void l_qsort(List **tree) {
    List *begin = NULL;
    Node *tmp1, *tmp2;
    for (begin = *tree; begin->next; begin = begin->next) {
        tmp1 = begin->node, tmp2 = begin->next->node;

        if (tmp1->size >= tmp2->size) {
            begin->node = tmp2, begin->next->node = tmp1;
        }
    }

    return;
}
