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

char *getCode(struct QueueNode *root, int ch, char arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        getCode(root->left, ch, arr, top + 1);
    }

    if (root->right) {
        arr[top] = 1;
        getCode(root->right, ch, arr, top + 1);
    }

    if (!(root->left) && !(root->right) && (root->data == ch)) {
        return arr;
    }
    return arr;
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
    fseek(in, 3, SEEK_SET);
    int l = 0;
    char arr[ALPH_SIZE], top = 0;
    for (i = 0; i < g; i++) {
        for (l = 0; l < ALPH_SIZE; l++) {
            arr[l] = 0;
        }
        top = 0;
        curCode = getCode(root, readByte(in), arr, top);
        writeCode(out, convertCode(curCode), (int) strlen(curCode));
    }
    writeCode(out, 0, 8 - getCount());
    return 0;
}

struct QueueNode* readHuffmanTree(FILE* file)
{
    struct QueueNode *node = (struct QueueNode*) malloc(sizeof(struct QueueNode));

    if (readBit(file))
    {
        node->data = (char) readByte(file);
        node->left = NULL;
        node->right = NULL;
    }
    else
    {
        node->data = -1;
        node->left = readHuffmanTree(file);
        node->right = readHuffmanTree(file);
    }
    return node;
}

void generateCodes(struct QueueNode *root)
{
    if (root->left != NULL)
    {
        strcpy(root->left->code, root->code);
        strcat(root->left->code, "0");
        generateCodes(root->left);
    }
    if (root->right != NULL)
    {
        strcpy(root->right->code, root->code);
        strcat(root->right->code, "1");
        generateCodes(root->right);
    }
}

int decodeByte(FILE* in, struct QueueNode *tree)
{
    if (tree->data != -1)
    {
        return tree->data;
    }
    else if (readBit(in))
    {
        return decodeByte(in, tree->right);
    }
    else
    {
        return decodeByte(in, tree->left);
    }
}

int fdecode(FILE *in, FILE *out) {
    int i, length = readInt(in);
    struct QueueNode *pTree = readHuffmanTree(in);
    generateCodes(pTree);
    setCount(0);
    for (i = 0; i < length; i++) {
        fprintf(out, "%c", decodeByte(in, pTree));
    }
    return 0;
}
