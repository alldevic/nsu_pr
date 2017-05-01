#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void l_push_front(List **tree, Node *l) {
    if ((*tree) == NULL) {
        List *list = calloc(1, sizeof(List));
        if (list == NULL) {
            printf("Error memory.");
            exit(1);
        }

        list->node = l;
        *tree = list;
    } else {
        List *front = calloc(1, sizeof(List));
        if (front == NULL) {
            printf("Error memory.");
            exit(1);
        }

        (*tree)->pre = front;
        front->next = *tree;
        front->node = l;

        *tree = front;
    }
}

Node *l_pop_front(List **tree) {
    Node *l;
    if (*tree == NULL) {
        return NULL;
    }

    l = (*tree)->node;

    if ((*tree)->next) {
        *tree = (*tree)->next;
        free((*tree)->pre);
        (*tree)->pre = NULL;
    } else {
        free(*tree);
        *tree = NULL;
    }

    return l;
}

int l_size(List *tree) {
    int count = 0;

    if (!tree) {
        return count;
    }

    for (count = 1; tree->next; tree = tree->next, count++);
    return count;
}

Node *combine(Node *left, Node *right) {
    Node *node = calloc(1, sizeof(Node));

    node->size = left->size + right->size;
    node->left = left;
    node->right = right;

    return node;
}

void l_qsort(List **tree) {
    List *begin = NULL;
    for (begin = *tree; begin->next; begin = begin->next) {
        if (begin->node->size >= begin->next->node->size) {
            Node *tmp = begin->node;
            begin->node = begin->next->node;
            begin->next->node = tmp;
        }
    }

    return;
}
