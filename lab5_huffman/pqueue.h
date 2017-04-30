#ifndef UNTITLED_PQUEUE_H
#define UNTITLED_PQUEUE_H
struct QueueNode {
    char data;
    unsigned freq;
    char code[10];
    struct QueueNode *left, *right;
};

struct Queue {
    int front, rear;
    int capacity;
    struct QueueNode **array;
};

struct QueueNode *newNode(char data, unsigned freq);

struct Queue *createQueue(int capacity);

int isSizeOne(struct Queue *queue);

int isEmpty(struct Queue *queue);

void enQueue(struct Queue *queue, struct QueueNode *item);

struct QueueNode *deQueue(struct Queue *queue);

struct QueueNode *getFront(struct Queue *queue);

struct QueueNode *findMin(struct Queue *firstQueue, struct Queue *secondQueue);

#endif /*UNTITLED_PQUEUE_H*/
