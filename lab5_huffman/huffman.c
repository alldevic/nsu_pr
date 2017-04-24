#include <stdio.h>
#include <stdlib.h>
#include "pqueue.h"
#include "huffman.h"
#include "solution.h"
#include "binary_io.h"

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


void printCodes( FILE *file, struct QueueNode *root) {
    if (!(root->left || root->right))
    {
        writeBit(file, 1);
        writeCode(file, root->data, 8);
    }
    else
    {
        writeBit(file, 0);
        printCodes(file, root->left);
        printCodes(file, root->right);
    }
}
