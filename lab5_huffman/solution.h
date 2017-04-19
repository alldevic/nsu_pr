#ifndef LAB5_SOLUTION_H
#define LAB5_SOLUTION_H

#define INPUT "in.txt"
#define OUTPUT "out.txt"
#define ALPH_SIZE 256
#define CODE 'c'
#define DECODE 'd'
#define LEFT 0
#define RIGHT 1
#define TRUE 1
#define FALSE 0

typedef struct node {
    char symbol;
    int count;
    struct node *left, *right;
} *Tree;

typedef struct list {
    Tree data;
    struct list *next;
} *PriorityQueue;


int init_freq(FILE *file, PriorityQueue *freq);

int huffman_tree_init(Tree tree, PriorityQueue freq);

int fprint_tree(FILE *file, Tree tree);

int fcode(FILE *in, Tree tree, FILE *out);

int fscan_tree(FILE *file, Tree tree);

int fdecode(FILE *in, Tree tree, FILE *out);

#endif /*LAB5_SOLUTION_H*/
