#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "main.h"
#include "errors.h"

Text huffmanTable[COUNT_CHAR];
size_t quantityChar[COUNT_CHAR] = {0};
unsigned char text[MY_BUFFER] = {0};

int main(void) {
    FILE *fin = NULL, *fout = NULL, *tabl = NULL;
    unsigned char huffmanFlag[3];
    int i = 0;

    ERR((fin = fopen(IN, "rb")) == NULL);
    ERR((fout = fopen(OUT, "wb")) == NULL);

    fread(huffmanFlag, sizeof(unsigned char), 3, fin);

    for (i = 0; i < COUNT_CHAR; i++) {
        huffmanTable[i].size = 0;
    }

    if (huffmanFlag[0] == 'c') {
        encoding(fin, fout, tabl);
    } else if (huffmanFlag[0] == 'd') {
        decoding(fin, fout, tabl);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

int encoding(FILE *fin, FILE *fout, FILE *tabl) {
    size_t size = 0, i, j, k;
    int pos = 0, byteOut = 0;

    if (!(size = fread(text, sizeof(char), MY_BUFFER, fin))) {
        return 0;
    }

    while (size) {
        for (i = 0; i < size; i++) {
            quantityChar[text[i]]++;
        }
        size = fread(text, sizeof(char), MY_BUFFER, fin);
    }
    fclose(fin);

    ERR((tabl = fopen("tabl.txt", "wb")) == NULL);
    write_table(tabl);

    ERR((fin = fopen(IN, "rb")) == NULL);
    fread(text, sizeof(char), 3, fin);

    size = fread(text, sizeof(char), MY_BUFFER, fin);
    while (size) {
        for (k = 0; k < size; k++) {
            for (j = 0; j < huffmanTable[text[k]].size; j++) {
                byteOut <<= 1, pos++;
                byteOut = huffmanTable[text[k]].text[j] ? (byteOut | 0x01) : byteOut;
                byteOut = pos == 8 ? pos = fprintf(fout, "%c", byteOut) - 1 : byteOut;
            }
        }

        size = fread(text, sizeof(char), MY_BUFFER, fin);
    }

    fprintf(fout, pos ? "%c" : "", pos ? byteOut << (8 - pos) : '1');
    fprintf(fout, "%c", !pos ? 8 : pos);
    return 0;
}

int decoding(FILE *fin, FILE *fout, FILE *tabl) {
    int size = (int) fread(text, sizeof(char), MY_BUFFER, fin);
    int index = 0, i, runner = 0, offset;
    Node *root, *node;
    unsigned char byteIn = text[index++], position = 0;

    if (size == 0) {
        fclose(fin), fclose(fout);
        return 0;
    }

    ERR((tabl = fopen("tabl.txt", "rb")) == NULL);
    node = root = read_table(tabl);

    while (size == MY_BUFFER) {
        for (i = 0; i < 8; i++) {
            if ((byteIn & 0x80) != 0) {
                if (node->right) {
                    node = node->right;
                } else {
                    break;
                }
            } else {
                if (node->left) {
                    node = node->left;
                } else {
                    break;
                }
            }

            if (!node->left && !node->right) {
                fputc(node->ch, fout);
                node = root;
            }

            byteIn <<= 1;
            position++;

            if (position == 8) {
                position = 0;
                byteIn = text[index++];

                if (index == size) {
                    size = (int) fread(text, sizeof(char), MY_BUFFER, fin);
                    index = 0;

                    if (size != MY_BUFFER) {
                        break;
                    }
                }
            }
        }
    }

    while (index < size - 2) {
        runner = 1;
        for (i = 0; i < 8; i++) {
            if ((byteIn & 0x80) != 0) {
                if (node->right) {
                    node = node->right;
                } else {
                    index = size;
                    break;
                }
            } else {
                if (node->left) {
                    node = node->left;
                } else {
                    index = size;
                    break;
                }
            }

            if (!node->left && !node->right) {
                fputc(node->ch, fout);
                node = root;
            }

            byteIn <<= 1;
            position++;

            if (position == 8) {
                position = 0;
                byteIn = text[index++];
            }
        }
    }

    for (i = position; (i < 8) && runner; i++) {
        if ((byteIn & 0x80) != 0) {
            if (node->right) {
                node = node->right;
            } else {
                break;
            }
        } else {
            if (node->left) {
                node = node->left;
            } else {
                break;
            }
        }

        if (!node->left && !node->right) {
            fputc(node->ch, fout);
            node = root;
        }

        byteIn <<= 1;
        position++;
    }

    byteIn = text[size - 2];
    offset = text[size - 1];

    if (offset == 0) {
        offset = 8;
    }

    for (i = 0; i < offset; i++) {
        if ((byteIn & 0x80) != 0) {
            if (node->right) {
                node = node->right;
            } else {
                break;
            }
        } else {
            if (node->left) {
                node = node->left;
            } else {
                break;
            }
        }

        if (!node->left && !node->right) {
            fputc(node->ch, fout);
            node = root;
        }

        byteIn <<= 1;
    }
    return 0;
}

int write_table(FILE *tabl) {
    size_t i, j;
    int pos = 0, byteOut = 0, countChar = 0;
    Text buffer;
    Node *root, *l;
    List *tree = NULL;

    for (i = 0; i < COUNT_CHAR; i++) {
        if (quantityChar[i]) {
            ERR((l = calloc(1, sizeof(Node))) == NULL);
            l->ch = (unsigned char) i, l->size = quantityChar[i];
            l_push_front(&tree, l);
            countChar++;
        }
    }

    while (l_size(tree) > 1) {
        Node *left = l_pop_front(&tree);
        Node *right = l_pop_front(&tree);

        l_push_front(&tree, combine(left, right));
        l_qsort(&tree);
    }


    buffer.size = 0;
    root = tree->node;

    if (root->left && root->right) {
        build_table(root, huffmanTable, &buffer);
    } else {
        huffmanTable[root->ch].size = 1;
        huffmanTable[root->ch].text[0] = 0;
    }


    fprintf(tabl, "%c", countChar);

    for (i = 0; i < COUNT_CHAR; i++) {
        if (quantityChar[i]) {
            unsigned char letter = (unsigned char) i;

            for (j = 0; j < 8; j++) {
                byteOut <<= 1;

                byteOut |= (letter & 0x80) != 0;
                letter <<= 1, pos++;
                byteOut = pos == 8 ? pos = fprintf(tabl, "%c", byteOut) - 1 : byteOut;
            }

            letter = (unsigned char) huffmanTable[i].size;

            for (j = 0; j < 8; j++) {
                byteOut <<= 1;
                byteOut |= (letter & 0x80) != 0;
                letter <<= 1, pos++;
                byteOut = pos == 8 ? pos = fprintf(tabl, "%c", byteOut) - 1 : byteOut;
            }

            for (j = 0; j < huffmanTable[i].size; j++) {
                byteOut <<= 1, pos++;
                byteOut |= huffmanTable[i].text[j];
                byteOut = pos == 8 ? pos = fprintf(tabl, "%c", byteOut) - 1 : byteOut;
            }

        }
    }
    fprintf(tabl, pos ? "%c" : "", pos ? byteOut << (8 - pos) : '1');
    return 0;
}

Node *read_table(FILE *tabl) {
    char tableDecod[1000], byteIn = 0;
    unsigned char position = 0, currentSymbol = 0, currentCount;
    size_t tableIndex = 0, countDecod = 0, i, j;
    Node *root, *nodes;

    fread(tableDecod, sizeof(char), 1000, tabl);
    countDecod = (size_t) (!tableDecod[tableIndex++] ? 256 : tableDecod[tableIndex - 1]);
    byteIn = tableDecod[tableIndex++];

    for (i = 0; i < countDecod; i++) {

        currentSymbol = 0;
        for (j = 0; j < 8; j++) {
            currentSymbol <<= 1, currentSymbol |= (byteIn & 0x80) != 0;
            byteIn <<= 1, position++;

            if (position == 8) {
                byteIn = tableDecod[tableIndex++], position = 0;
            }
        }

        currentCount = 0;
        for (j = 0; j < 8; j++) {
            currentCount <<= 1, currentCount |= (byteIn & 0x80) != 0;
            byteIn <<= 1, position++;

            if (position == 8) {
                byteIn = tableDecod[tableIndex++], position = 0;
            }
        }

        huffmanTable[currentSymbol].size = currentCount;

        for (j = 0; j < currentCount; j++) {
            huffmanTable[currentSymbol].text[j] <<= 1;
            huffmanTable[currentSymbol].text[j] = (byteIn & 0x80) != 0;

            byteIn <<= 1, position++;
            if (position == 8) {
                byteIn = tableDecod[tableIndex++], position = 0;
            }
        }
    }


    root = calloc(1, sizeof(Node));
    nodes = calloc(512, sizeof(Node));

    for (i = 0; i < COUNT_CHAR; i++) {
        if (huffmanTable[i].size) {
            Node *node = root;

            for (j = 0; j < huffmanTable[i].size; j++) {
                node = n_direction(node, huffmanTable[i].text[j], nodes);
            }

            node->ch = (unsigned char) i;
        }
    }

    return root;
}

void build_table(Node *root, Text *huffmanTable, Text *buffer) {
    size_t i;
    if (root->left) {
        buffer->text[buffer->size] = 0, buffer->size++;
        build_table(root->left, huffmanTable, buffer);
    }

    if (root->right) {
        buffer->text[buffer->size] = 1, buffer->size++;
        build_table(root->right, huffmanTable, buffer);
    }

    if (!(root->left || root->right)) {
        for (i = 0; i < buffer->size; i++) {
            huffmanTable[root->ch].text[i] = buffer->text[i];
            huffmanTable[root->ch].size = buffer->size;
        }
    }

    buffer->size = buffer->size ? buffer->size - 1 : buffer->size;
}

Node *n_direction(Node *root, int direction, Node *nodes) {
    static int index = 0;
    if (direction) {
        if (!root->right) {
            root->right = &nodes[index++];
        }

        return root->right;
    } else {
        if (!root->left) {
            root->left = &nodes[index++];
        }

        return root->left;
    }
}
