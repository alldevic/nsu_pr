#include <stdio.h>
#include <stdlib.h>
#include "solution.h"

int main(void) {
    FILE *file = fopen(INPUT, "r"), *out;
    PriorityQueue freq = NULL;
    /*Tree tree = NULL;*/
    char type;
    fscanf(file, "%c\n", &type);

    if (type == CODE) {
        init_freq(file, &freq);
/*        huffman_tree_init(tree, freq);*/
        out = fopen(OUTPUT, "w");
/*        fprint_tree(out, tree);*/
        fseek(file, 0, SEEK_SET);
/*        fcode(file, tree, out);*/
        fclose(out);
    }

    if (type == DECODE) {
/*        fscan_tree(file, tree);*/
        out = fopen(OUTPUT, "bw");
/*        fdecode(file, tree, out);*/
    }

    fclose(file);
    return 0;
}

int freqList_add(PriorityQueue *list, Tree data) {
    PriorityQueue tmp = malloc(sizeof(PriorityQueue));
    tmp->data = data;
    tmp->next = *list;
    *list = tmp;
    return 0;
}

int init_freq(FILE *file, PriorityQueue *freq) {
    char ch;
    Tree tmp_lst;
    int *tmp = calloc(ALPH_SIZE, sizeof(int)), i = 0;
    int j = 0, max = 0, max2 = 0, fl = 1;

    while (!feof(file)) {
        fscanf(file, "%c", &ch);
        tmp[(int) ch]++;
    }

    for (i = 0; (i < ALPH_SIZE) && fl; i++) {
        max = 0, fl = 0;
        for (j = 0; j < ALPH_SIZE; j++) {
            if (max < tmp[j]) {
                max = tmp[j], tmp[j] = 0;
                max2 = j;
                fl = 1;
            }
        }

        if (max) {
            tmp_lst = malloc(sizeof(PriorityQueue));
            tmp_lst->count = max;
            tmp_lst->symbol = (char) max2;
            freqList_add(freq, tmp_lst);
        }
    }
    free(tmp);

    return 0;
}

int tree_init(Tree tree, Tree data, Tree left, Tree right) {
    tree = malloc(sizeof(Tree));
    tree->symbol = data->symbol;
    tree->count = data->count;
    tree->left = left;
    tree->right = right;
    return 0;
}

/*int huffman_tree_init(Tree tree, PriorityQueue freq) {


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
}*/
