#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "main.h"
#include "errors.h"

void writeLetter(FILE *file, unsigned char letter, int *pos, int *byte);

int main(void) {
    FILE *fin, *fout, *tabl = NULL;
    unsigned char huffmanFlag[3];

    ERR((fin = fopen(INPUT, "rb")) == NULL);
    ERR((fout = fopen(OUTPUT, "wb")) == NULL);
    fread(huffmanFlag, sizeof(unsigned char), 3, fin);

    if (huffmanFlag[0] == 'c') {
        encoding(fin, fout, tabl);
    } else if (huffmanFlag[0] == 'd') {
        decoding(fin, fout, tabl);
    }

    fclose(fin), fclose(fout);
    return 0;
}

int encoding(FILE *fin, FILE *fout, FILE *tabl) {
    Text table[COUNT_CHAR];
    unsigned char text[BUFFER] = {0};
    size_t size = 0, i, j, k, quantityChar[COUNT_CHAR] = {0};
    int pos = 0, byteOut = 0;

    if ((size = fread(text, sizeof(char), BUFFER, fin))) {
        for (; size; size = fread(text, sizeof(char), BUFFER, fin)) {
            for (i = 0; i < size; i++, quantityChar[text[i - 1]]++);
        }
    } else return 0;
    fseek(fin, 3, SEEK_SET);
    ERR((tabl = fopen(TABLE, "wb")) == NULL);
    write_table(tabl, quantityChar, table);
    while ((size = fread(text, sizeof(char), BUFFER, fin))) {
        for (k = 0; k < size; k++) {
            for (j = 0; j < table[text[k]].size; j++) {
                byteOut = table[text[k]].text[j] ? byteOut << 1 | 0x01 : byteOut << 1;
                byteOut = pos++ == 7 ? pos = fprintf(fout, "%c", byteOut) - 1 : byteOut;
            }
        }
    }
    pos ? fprintf(fout, "%c%c", byteOut << (8 - pos), pos) : fprintf(fout, "%c", 8);
    return 0;
}

int decoding(FILE *fin, FILE *fout, FILE *tabl) {
    Text huffmanTable[COUNT_CHAR];
    unsigned char text[BUFFER] = {0}, byteIn, pos = 0;
    int size = 0, index = 0, i, fl = 0, offset;
    Node *root, *node;

    if (!(size = (int) fread(text, sizeof(char), BUFFER, fin))) {
        return 0;
    }

    ERR((tabl = fopen(TABLE, "rb")) == NULL);
    for (i = 0; i < COUNT_CHAR; i++, huffmanTable[i - 1].size = 0);
    node = root = read_table(tabl, huffmanTable), byteIn = text[index++];

    while (size == BUFFER) {
        for (i = 0; i < 8; i++) {
            node = writeNode(fout, root, node, byteIn);
            byteIn = pos++ == 7 ? text[index++] : byteIn << 1;
            pos = pos == 8 ? (char) 0 : pos;
            index = index == size ? 0 : index;
            if (!(pos || index) &&
                (size = (int) fread(text, sizeof(char), BUFFER, fin)) != BUFFER) {
                break;
            }
        }
    }

    while (index < size - 2) {
        for (i = 0; i < 8; i++, byteIn <<= 1) {
            node = writeNode(fout, root, node, byteIn);
        }
        pos = 0, byteIn = text[index++], fl = 1;
    }

    for (i = pos; (i < 8) && fl; i++, byteIn <<= 1) {
        node = writeNode(fout, root, node, byteIn);
    }

    byteIn = text[size - 2], offset = !text[size - 1] ? 8 : text[size - 1];
    for (i = 0; i < offset; i++, byteIn <<= 1) {
        node = writeNode(fout, root, node, byteIn);
    }
    return 0;
}

int write_table(FILE *tabl, size_t quantityChar[], Text table[]) {
    size_t i, j;
    int pos = 0, byteOut = 0, countChar = 0;
    Text buffer;
    Node *root, *l;
    List *tree = NULL;

    for (i = 0; i < COUNT_CHAR; i++) {
        table[i - 1].size = 0;
        if (quantityChar[i]) {
            ERR((l = calloc(1, sizeof(Node))) == NULL);
            l->ch = (char) i, l->size = quantityChar[i], countChar++;
            l_push_front(&tree, l);
        }
    }

    while (l_size(tree) > 1) {
        Node *left = l_pop_front(&tree), *right = l_pop_front(&tree);
        l_push_front(&tree, combine(left, right));
        l_qsort(&tree);
    }

    buffer.size = 0, root = tree->node;
    if (root->left && root->right) {
        build_table(root, table, &buffer);
    } else {
        table[root->ch].size = 1;
        table[root->ch].text[0] = 0;
    }

    fprintf(tabl, "%c", countChar);
    for (i = 0; i < COUNT_CHAR; i++) {
        if (quantityChar[i]) {
            writeLetter(tabl, (unsigned char) i, &pos, &byteOut);
            writeLetter(tabl, (unsigned char) table[i].size, &pos, &byteOut);
            for (j = 0; j < table[i].size; j++) {
                byteOut <<= 1, pos++;
                byteOut |= table[i].text[j];
                byteOut = pos == 8 ? pos = fprintf(tabl, "%c", byteOut) - 1 : byteOut;
            }
        }
    }
    fprintf(tabl, pos ? "%c" : "", pos ? byteOut << (8 - pos) : '1');
    return 0;
}

Node *read_table(FILE *tabl, Text table[]) {
    char tableDecod[1000], byteIn = 0;
    unsigned char pos = 0, currentSymbol = 0, currentCount;
    size_t tableIndex = 0, countDecod = 0, i, j;
    Node *root, *nodes;

    fread(tableDecod, sizeof(char), 1000, tabl);
    countDecod = (size_t) (!tableDecod[tableIndex++] ? 256 : tableDecod[tableIndex - 1]);
    byteIn = tableDecod[tableIndex++];

    for (i = 0; i < countDecod; i++) {
        currentSymbol = 0;
        for (j = 0; j < 8; j++) {
            currentSymbol <<= 1, currentSymbol |= (byteIn & 0x80) != 0;
            byteIn = pos++ == 7 ? tableDecod[tableIndex++] : byteIn << 1;
            pos = pos == 8 ? (char) 0 : pos;
        }

        currentCount = 0;
        for (j = 0; j < 8; j++) {
            currentCount <<= 1, currentCount |= (byteIn & 0x80) != 0;
            byteIn = pos++ == 7 ? tableDecod[tableIndex++] : byteIn << 1;
            pos = pos == 8 ? (char) 0 : pos;
        }

        table[currentSymbol].size = currentCount;

        for (j = 0; j < currentCount; j++) {
            table[currentSymbol].text[j] <<= 1;
            table[currentSymbol].text[j] = (byteIn & 0x80) != 0;
            byteIn = pos++ == 7 ? tableDecod[tableIndex++] : byteIn << 1;
            pos = pos == 8 ? (char) 0 : pos;
        }
    }

    root = calloc(1, sizeof(Node)), nodes = calloc(512, sizeof(Node));

    for (i = 0; i < COUNT_CHAR; i++) {
        if (table[i].size) {
            Node *node = root;

            for (j = 0; j < table[i].size; j++) {
                node = n_direction(node, table[i].text[j], nodes);
            }
            node->ch = (char) i;
        }
    }

    return root;
}

void build_table(Node *root, Text *table, Text *buffer) {
    size_t i;
    if (root->left) {
        buffer->text[buffer->size++] = 0, build_table(root->left, table, buffer);
    }

    if (root->right) {
        buffer->text[buffer->size++] = 1, build_table(root->right, table, buffer);
    }

    if (!(root->left || root->right)) {
        for (i = 0; i < buffer->size; i++) {
            table[root->ch].text[i] = buffer->text[i];
            table[root->ch].size = buffer->size;
        }
    }

    buffer->size = buffer->size ? buffer->size - 1 : buffer->size;
}

Node *n_direction(Node *root, int direction, Node *nodes) {
    static int index = 0;
    if (direction) {
        return root->right = !root->right ? &nodes[index++] : root->right;
    } else {
        return root->left = !root->left ? &nodes[index++] : root->left;
    }
}

Node *writeNode(FILE *fout, Node *root, Node *node, unsigned char byteIn) {
    node = ((byteIn & 0x80) && node->right) ? node->right :
           !(byteIn & 0x80) && node->left ? node->left : node;
    if (!(node->left || node->right)) {
        fprintf(fout, "%c", node->ch), node = root;
    }
    return node;
}

void writeLetter(FILE *file, unsigned char letter, int *pos, int *byte) {
    int j;
    for (j = 0; j < 8; j++, letter <<= 1) {
        (*byte) <<= 1, (*pos)++;
        (*byte) |= (letter & 0x80) != 0;
        (*byte) = (*pos) == 8 ? (*pos) = fprintf(file, "%c", (*byte)) - 1 : (*byte);
    }
}
