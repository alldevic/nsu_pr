#include <stdlib.h>
#include "pqueue.h"

struct QueueNode *newNode(char data, unsigned freq) {
    struct QueueNode *temp =
            (struct QueueNode *) malloc(sizeof(struct QueueNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

struct Queue *createQueue(int capacity) {
    struct Queue *queue = (struct Queue *) malloc(sizeof(struct Queue));
    queue->front = queue->rear = -1;
    queue->capacity = capacity;
    queue->array =
            (struct QueueNode **) malloc(queue->capacity * sizeof(struct QueueNode *));
    return queue;
}

int isSizeOne(struct Queue *queue) {
    return queue->front == queue->rear && queue->front != -1;
}

int isEmpty(struct Queue *queue) {
    return queue->front == -1;
}

void enQueue(struct Queue *queue, struct QueueNode *item) {
    if (queue->rear == queue->capacity - 1)
        return;
    queue->array[++queue->rear] = item;
    if (queue->front == -1)
        ++queue->front;
}

struct QueueNode *deQueue(struct Queue *queue) {
    struct QueueNode *temp;
    if (isEmpty(queue))
        return NULL;
    temp = queue->array[queue->front];
    if (queue->front == queue->rear)
        queue->front = queue->rear = -1;
    else
        ++queue->front;
    return temp;
}

struct QueueNode *getFront(struct Queue *queue) {
    if (isEmpty(queue))
        return NULL;
    return queue->array[queue->front];
}

struct QueueNode *findMin(struct Queue *firstQueue, struct Queue *secondQueue) {
    if (isEmpty(firstQueue))
        return deQueue(secondQueue);

    if (isEmpty(secondQueue))
        return deQueue(firstQueue);

    if (getFront(firstQueue)->freq < getFront(secondQueue)->freq)
        return deQueue(firstQueue);

    return deQueue(secondQueue);
}

