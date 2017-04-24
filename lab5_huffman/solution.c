#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solution.h"
#include "binary_io.h"
#include "huffman.h"
#include "pqueue.h"

int main(void) {
    FILE *in_file = fopen(INPUT, "rb");
    FILE *out_file = fopen(OUTPUT, "wb");

    char type = (char) readByte(in_file);
    readByte(in_file); /*CR*/
    readByte(in_file); /*LF*/

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


int fencode(FILE *in, FILE *out) {
    int size = 0, k = 0, i, j, g = 0;
    unsigned char ch;
    unsigned int tmp[256] = {0};
    char *curCode;
    while (!feof(in)) {
        ch = (unsigned char) readByte(in);
        if (!tmp[(int) ch]) {
            size++;
        }
        tmp[(int) ch]++;
        g++;
    }
    char data[size], max2 = 0;
    unsigned int freq[size], max, fl = 1;
    for (i = 0; i < size && fl; i++) {
        max = 0, fl = 0;
        for (j = 0; j < ALPH_SIZE; j++) {
            if (max < tmp[j]) {
                max = tmp[j], tmp[j] = 0;
                max2 = (char) j;
                fl = 1;
            }
        }

        if (max) {
            data[k] = max2;
            freq[k] = max;
            k++;
        }

    }
    struct QueueNode *root = buildHuffmanTree(data, freq, size);

    writeInt(out, size);
    printCodes(out, root);
    writeCode(out, 0, 8 - getCount());
    writeCode(out, '\n', 8);
    for (i = 0; i < g; i++) {
        curCode = getCode(root, (unsigned char) fgetc(in));
        writeCode(out, convertCode(curCode), (int) strlen(curCode));
    }
    writeCode(out, 0, 8 - getCount());
    return 0;
}

int fdecode(FILE *in, FILE *out) {
    return 0;
}



