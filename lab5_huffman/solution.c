#include <stdio.h>
#include <stdlib.h>
#include "solution.h"
#include "binary_io.h"
#include "huffman.h"

int main(void) {
    FILE *in_file = fopen(INPUT, "rb");
    FILE *out_file = fopen(OUTPUT, "wb");

    char type;
    fscanf(in_file, "%c\n", &type);

    if (type == ENCODE) {
        fencode(in_file, out_file);
    }

    if (type == DECODE) {
        fdecode(in_file, out_file);
    }

    fclose(in_file);
    fclose(out_file);
    return 0;
}




