#include <stdio.h>
#include <stdlib.h>
#include "solution.h"

int main(void) {
    FILE *file = fopen(INPUT, "r");
    int *freq = calloc(ALPH_SIZE, sizeof(int));
    char type;
    fscanf(file, "%c", &type);

    if (type == CODE) {
        init_freq(file, freq);
        Tree tree = NULL;
        huffman_tree_init(tree, freq);
        FILE *out = fopen(OUTPUT, "w");
        fprint_tree(out, tree);
        fcode(file, tree, out);
    }

    if (type == DECODE) {
        Tree tree = NULL;
        fscan_tree(file, tree);
        FILE *out = fopen(OUTPUT, "w");
        fdecode(file, tree, out);
    }
    
    return 0;
}
