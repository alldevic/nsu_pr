#include <stdio.h>
#include <stdlib.h>
#include "pqueue.h"
#include "huffman.h"
#include "solution.h"

struct QueueNode *buildHuffmanTree(char data[], unsigned int freq[], int size) {
    struct QueueNode *left, *right, *top;
    struct Queue *firstQueue = createQueue(size);
    struct Queue *secondQueue = createQueue(size);
    int i;
    for (i = 0; i < size; ++i)
        enQueue(firstQueue, newNode(data[i], freq[i]));

    while (!(isEmpty(firstQueue) && isSizeOne(secondQueue))) {
        left = findMin(firstQueue, secondQueue);
        right = findMin(firstQueue, secondQueue);

        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        enQueue(secondQueue, top);
    }

    return deQueue(secondQueue);
}

void printCodes(struct QueueNode *root, int arr[], int top) {
    int i = 0;

    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    if (!(root->left) && !(root->right)) {
        printf("%c: ", root->data);
        for (i = 0; i < top; ++i)
            printf("%d", arr[i]);
        printf("\n");
    }
}
