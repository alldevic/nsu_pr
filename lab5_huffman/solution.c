#include <stdio.h>
#include <stdlib.h>
#include "solution.h"

int main(void) {
    FILE *file = fopen(INPUT, "r");

    char type;
    fscanf(file, "%c\n", &type);
    FreqList freq = NULL;
    if (type == CODE) {
        init_freq(file, &freq);
        fclose(file);
        Tree tree = NULL;
        huffman_tree_init(tree, freq);
        FILE *out = fopen(OUTPUT, "w");
        fprint_tree(out, tree);
        file = fopen(INPUT, "r");
        fcode(file, tree, out);
        fclose(out);
    }

    if (type == DECODE) {
        Tree tree = NULL;
        fscan_tree(file, tree);
        FILE *out = fopen(OUTPUT, "bw");
        fdecode(file, tree, out);
    }

    fclose(file);
    return 0;
}

int freqList_add(FreqList *list, char symbol, int count) {
    FreqList tmp = malloc(sizeof(FreqList));
    tmp->symbol = symbol;
    tmp->count = count;
    tmp->next = *list;
    *list = tmp;
    return 0;
}

int init_freq(FILE *file, FreqList *freq) {
    char ch;
    int *tmp = calloc(ALPH_SIZE, sizeof(int)), i = 0;
    while (!feof(file)) {
        fscanf(file, "%c", &ch);
        tmp[ch]++;
    }

    for (i = 0; i < ALPH_SIZE; i++) {
        if (tmp[i]) {
            freqList_add(freq, (char) i, tmp[i]);
        }
    }
    free(tmp);

    fflush(file);
    return 0;
}

int huffman_tree_init(Tree tree, FreqList freq) {
    return 0;
}

int fprint_tree(FILE *file, Tree tree) {
    return 0;
}

int fcode(FILE *in, Tree tree, FILE *out) {
    return 0;
}

int fscan_tree(FILE *file, Tree tree) {
    return 0;
}

int fdecode(FILE *in, Tree tree, FILE *out) {
    return 0;
}
