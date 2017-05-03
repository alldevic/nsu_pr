#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "main.h"
#include "errors.h"

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

int encoding(FILE *in, FILE *out, FILE *tab) {
    Text table[COUNT_CHAR];
    unsigned char text[BUFF] = {0};
    size_t size = 0, i, j, k, quantityChar[COUNT_CHAR] = {0};
    int pos = 0, byteOut = 0;

    if ((size = fread(text, sizeof(char), BUFF, in))) {
        for (; size; size = fread(text, sizeof(char), BUFF, in)) {
            for (i = 0; i < size; i++, quantityChar[text[i - 1]]++);
        }
    } else return 0;
    fseek(in, 3, SEEK_SET);
    ERR((tab = fopen(TABLE, "wb")) == NULL);
    write_table(tab, quantityChar, table);
    while ((size = fread(text, sizeof(char), BUFF, in))) {
        for (k = 0; k < size; k++) {
            for (j = 0; j < table[text[k]].size; j++) {
                byteOut = table[text[k]].text[j] ? byteOut << 1 | 0x01 : byteOut << 1;
                byteOut = pos++ == 7 ? pos = fprintf(out, "%c", byteOut) - 1 : byteOut;
            }
        }
    }
    pos ? fprintf(out, "%c%c", byteOut << (8 - pos), pos) : fprintf(out, "%c", 8);
    return 0;
}

int decoding(FILE *in, FILE *out, FILE *tab) {
    Text huffmanTable[COUNT_CHAR];
    unsigned char txt[BUFF] = {0}, byte, pos = 0;
    int size = 0, index = 0, i, fl = 0, offset;
    Node *root, *nd;

    if (!(size = (int) fread(txt, sizeof(char), BUFF, in))) {
        return 0;
    }

    ERR((tab = fopen(TABLE, "rb")) == NULL);
    for (i = 0; i < COUNT_CHAR; i++, huffmanTable[i - 1].size = 0);
    nd = root = read_table(tab, huffmanTable), byte = txt[index++];
    while (size == BUFF) {
        for (i = 0; i < 8; i++) {
            nd = writeNode(out, root, nd, byte);
            byte = pos++ == 7 ? txt[index++] : byte << 1;
            pos = pos == 8 ? (char) 0 : pos, index = index == size ? 0 : index;
            if (!(pos || index) &&
                (size = (int) fread(txt, sizeof(char), BUFF, in)) != BUFF) {
                break;
            }
        }
    }

    for (; index < size - 2; pos = 0, byte = txt[index++], fl = 1) {
        for (i = 0; i < 8; i++, nd = writeNode(out, root, nd, byte), byte <<= 1);
    }
    for (i = pos; (i < 8) && fl; i++, nd = writeNode(out, root, nd, byte), byte <<= 1);
    byte = txt[size - 2], offset = !txt[size - 1] ? 8 : txt[size - 1];
    for (i = 0; i < offset; i++, nd = writeNode(out, root, nd, byte), byte <<= 1);
    return 0;
}

int write_table(FILE *tab, size_t quantityChar[], Text table[]) {
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
            l_push(&tree, l);
        }
    }

    while (l_size(tree) > 1) {
        Node *left = l_pop(&tree), *right = l_pop(&tree);
        l_push(&tree, combine(left, right));
        l_qsort(&tree);
    }

    buffer.size = 0, root = tree->node;
    if (root->left && root->right) {
        build_table(root, table, &buffer);
    } else {
        table[root->ch].size = 1;
        table[root->ch].text[0] = 0;
    }

    fprintf(tab, "%c", countChar);
    for (i = 0; i < COUNT_CHAR; i++) {
        if (quantityChar[i]) {
            writeLetter(tab, (unsigned char) i, &pos, &byteOut);
            writeLetter(tab, (unsigned char) table[i].size, &pos, &byteOut);
            for (j = 0; j < table[i].size; j++) {
                byteOut <<= 1, pos++;
                byteOut |= table[i].text[j];
                byteOut = pos == 8 ? pos = fprintf(tab, "%c", byteOut) - 1 : byteOut;
            }
        }
    }
    fprintf(tab, pos ? "%c" : "", pos ? byteOut << (8 - pos) : '1');
    return 0;
}

Node *read_table(FILE *tab, Text table[]) {
    char tableDecod[1000], byteIn = 0;
    unsigned char pos = 0, curSymb = 0, curCount;
    size_t tableIndex = 0, countDecod = 0, i, j;

    fread(tableDecod, sizeof(char), 1000, tab);
    countDecod = !tableDecod[tableIndex++] ? 256 : (unsigned) tableDecod[tableIndex - 1];
    byteIn = tableDecod[tableIndex++];

    for (i = 0; i < countDecod; i++) {
        curSymb = readByte(&byteIn, &pos, tableDecod, &tableIndex);
        table[curSymb].size = curCount = readByte(&byteIn, &pos, tableDecod, &tableIndex);

        for (j = 0; j < curCount; table[curSymb].text[j] <<= 1, j++) {
            table[curSymb].text[j] = (byteIn & 0x80) != 0;
            byteIn = pos++ == 7 ? tableDecod[tableIndex++] : byteIn << 1;
            pos = pos == 8 ? (char) 0 : pos;
        }
    }

    return createTree(table);
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

Node *writeNode(FILE *out, Node *root, Node *node, unsigned char byteIn) {
    node = ((byteIn & 0x80) && node->right) ? node->right :
           !(byteIn & 0x80) && node->left ? node->left : node;
    if (!(node->left || node->right)) {
        fprintf(out, "%c", node->ch), node = root;
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

unsigned char readByte(char *byte, unsigned char *pos, char *table, size_t *index) {
    unsigned char res = 0, i;
    for (i = 0; i < 8; i++) {
        res <<= 1, res |= ((*byte) & 0x80) != 0;
        (*byte) = (*pos)++ == 7 ? table[(*index)++] : (*byte) << 1;
        (*pos) = (unsigned char) ((*pos) == 8 ? (char) 0 : (*pos));
    }
    return res;
}

Node *createTree(Text table[]) {
    Node *root = calloc(1, sizeof(Node)), *nodes = calloc(512, sizeof(Node)), *node;
    unsigned i, j;
    for (i = 0; i < COUNT_CHAR; i++) {
        if (table[i].size) {
            node = root;
            for (j = 0; j < table[i].size; j++) {
                node = n_direction(node, table[i].text[j], nodes);
            }
            node->ch = (char) i;
        }
    }
    return root;
}
