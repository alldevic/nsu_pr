#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "errors.h"

/**
 * Implementation of push operation for list
 * @param list - List for pushing
 * @param l - data
 * @return error code
 */
int l_push(List **list, Node *l) {
    if (!*list) {
        List *lst = calloc(1, sizeof(List));
        ERR(lst == NULL);

        lst->node = l, *list = lst;
    } else {
        List *front = calloc(1, sizeof(List));
        ERR(front == NULL);

        (*list)->pre = front, front->next = *list, front->node = l;
        *list = front;
    }
    return 0;
}

/**
 * Implementation of pop operation for list
 * @param list - List for pop
 * @return Deleted element
 */
Node *l_pop(List **list) {
    Node *l;
    if (!*list) {
        return NULL;
    }

    l = (*list)->node;

    if ((*list)->next) {
        *list = (*list)->next, free((*list)->pre), (*list)->pre = NULL;
    } else {
        free(*list), *list = NULL;
    }

    return l;
}

/**
 * Calculate length of list
 * @param list for calculating
 * @return length
 */
int l_size(List *list) {
    int count = 0;
    for (count = 1; list && list->next; list = list->next, count++);
    return count;
}

/**
 * Function for union of two trees in one tree
 * @param left - new left node
 * @param right - new right node
 * @return new tree with new left and new right nodes
 */
Node *combine(Node *left, Node *right) {
    Node *node;
    if (!(node = calloc(1, sizeof(Node)))) {
        return NULL;
    }

    node->size = left->size + right->size;
    node->left = left, node->right = right;
    return node;
}

/**
 * Quick sort implementation for list
 * @param list - list for sorting
 */
void l_qsort(List **list) {
    List *begin = NULL;
    Node *tmp1, *tmp2;
    for (begin = *list; begin->next; begin = begin->next) {
        tmp1 = begin->node, tmp2 = begin->next->node;

        if (tmp1->size >= tmp2->size) {
            begin->node = tmp2, begin->next->node = tmp1;
        }
    }

    return;
}
