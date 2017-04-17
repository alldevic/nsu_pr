#ifndef LAB5_SOLUTION_H
#define LAB5_SOLUTION_H

#define INPUT "in.txt"
#define OUTPUT "out.txt"
#define ALPH_SIZE 256
#define CODE 'c'
#define DECODE 'd'

typedef struct tree {

} *Tree;

int init_freq(FILE *file, int *freq);

int huffman_tree_init(Tree tree, int *freq);

int fprint_tree(FILE *file, Tree tree);

int fcode(FILE *in, Tree tree, FILE *out);

int fscan_tree(FILE *file, Tree tree);

int fdecode(FILE *in, Tree tree, FILE* out);

#endif //LAB5_SOLUTION_H
