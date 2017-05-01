#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "main.h"

Text huffmanTable[COUNT_CHAR];

size_t quantityChar[COUNT_CHAR] = {0};

unsigned char text[MY_BUFFER] = {0};


int main(void) {
    FILE *fin = NULL, *fout = NULL, *tabl = NULL;
    unsigned char huffmanFlag[3];
    int i = 0;


    fin = fopen(IN, "rb");
    fout = fopen(OUT, "wb");
    if ((fin == NULL) || (fout == NULL)) {
        return 1;
    }


    fread(huffmanFlag, sizeof(unsigned char), 3, fin);

    for (i = 0; i < COUNT_CHAR; ++i) {
        huffmanTable[i].size = 0;
    }

    switch (huffmanFlag[0]) {
        case 'c':
            encoding(fin, fout, tabl);
            break;
        case 'd':
            decoding(fin, fout, tabl);
            break;
        default:
            break;
    }

    fclose(fin);
    fclose(fout);

    return 0;
}

void encoding(FILE *fin, FILE *fout, FILE *tabl) {
    size_t size = fread(text, sizeof(char), MY_BUFFER, fin), flag = 1, i, j, k;
    unsigned char byteOut = 0, position = 0;

    if (size == 0) {
        fclose(fin);
        fclose(fout);
        exit(0);
    }

    while (flag) {
        for (i = 0; i < size; ++i) {
            ++quantityChar[text[i]];
        }
        if (size == MY_BUFFER) {
            size = fread(text, sizeof(char), MY_BUFFER, fin);
        } else {
            flag = 0;
        }
    }

    fclose(fin);

    tabl = fopen("tabl.txt", "wb");
    if (tabl == NULL) {
        fclose(fin);
        fclose(fout);
        exit(1);
    }

    write_table(tabl);

    fin = fopen(IN, "rb");

    flag = 1;
    fread(text, sizeof(char), 3, fin);
    size = fread(text, sizeof(char), MY_BUFFER, fin);

    while (flag) {
        for (k = 0; k < size; ++k) {
            unsigned char rem = text[k];

            for (j = 0; j < huffmanTable[rem].size; ++j) {
                byteOut <<= 1;

                if (huffmanTable[rem].text[j]) {
                    byteOut = (unsigned char) (byteOut | 0x01);
                }

                ++position;

                if (position == 8) {
                    fprintf(fout, "%c", byteOut);
                    position = 0;
                    byteOut = 0;
                }
            }
        }

        if (size == MY_BUFFER) {
            size = fread(text, sizeof(char), MY_BUFFER, fin);
        } else {
            flag = 0;
        }
    }

    if (position != 0) {
        byteOut <<= 8 - position;
        fprintf(fout, "%c", byteOut);
    }

    position = (!position) ? (unsigned char) 8 : position;
    fprintf(fout, "%c", position);
}

void decoding(FILE *fin, FILE *fout, FILE *tabl) {
    int size = (int) fread(text, sizeof(char), MY_BUFFER, fin);
    int index = 0, i, runner = 0, offset;
    Node *root, *node;
    unsigned char byteIn = text[index++], position = 0;

    if (size == 0) {
        fclose(fin);
        fclose(fout);
        exit(0);
    }

    tabl = fopen("tabl.txt", "rb");
    if (tabl == NULL) {
        fclose(fin);
        fclose(fout);
        exit(1);
    }

    node = root = read_table(tabl);


    while (size == MY_BUFFER) {
        for (i = 0; i < 8; ++i) {
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
            ++position;

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
        for (i = 0; i < 8; ++i) {
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
            ++position;

            if (position == 8) {
                position = 0;
                byteIn = text[index++];
            }
        }
    }

    for (i = position; (i < 8) && runner; ++i) {
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
        ++position;
    }

    byteIn = text[size - 2];
    offset = text[size - 1];

    if (offset == 0) {
        offset = 8;
    }

    for (i = 0; i < offset; ++i) {
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
}

void write_table(FILE *tabl) {
    size_t i, j;
    char position = 0;
    unsigned char byteOut = 0, countChar = 0;
    Text buffer;
    Node *root;
    List *tree = NULL;

    for (i = 0; i < COUNT_CHAR; ++i) {
        if (quantityChar[i] != 0) {
            Node *l = calloc(1, sizeof(Node));
            if (l == NULL) {
                printf("Error memory.");
                exit(1);
            }

            l->ch = (unsigned char) i;
            l->size = quantityChar[i];

            l_push_front(&tree, l);

            ++countChar;
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

    for (i = 0; i < COUNT_CHAR; ++i) {
        if (quantityChar[i]) {
            unsigned char letter = (unsigned char) i;

            for (j = 0; j < 8; ++j) {
                byteOut <<= 1;

                byteOut |= (letter & 0x80) != 0;
                letter <<= 1;
                ++position;

                if (position == 8) {
                    fprintf(tabl, "%c", byteOut);
                    byteOut = 0;
                    position = 0;
                }
            }

            letter = (unsigned char) huffmanTable[i].size;

            for (j = 0; j < 8; ++j) {
                byteOut <<= 1;

                byteOut |= (letter & 0x80) != 0;
                letter <<= 1;
                ++position;

                if (position == 8) {
                    fprintf(tabl, "%c", byteOut);
                    byteOut = 0;
                    position = 0;
                }
            }

            for (j = 0; j < huffmanTable[i].size; ++j) {
                byteOut <<= 1;
                byteOut |= huffmanTable[i].text[j];

                ++position;

                if (position == 8) {
                    fprintf(tabl, "%c", byteOut);
                    byteOut = 0;
                    position = 0;
                }
            }

        }
    }

    if (position != 0) {
        byteOut <<= 8 - position;
        fprintf(tabl, "%c", byteOut);
    }
}

Node *read_table(FILE *tabl) {
    char tableDecod[1000];
    unsigned char byteIn = 0, position = 0;
    size_t tableIndex = 0, countDecod = 0, i, j, currentCount;
    Node *root, *nodes;
    fread(tableDecod, sizeof(unsigned char), 1000, tabl);
    countDecod = (size_t) (!tableDecod[tableIndex++] ? 256 : tableDecod[tableIndex - 1]);

    byteIn = (unsigned char) tableDecod[tableIndex++];

    for (i = 0; i < countDecod; ++i) {
        unsigned char currentSymbol = 0;

        for (j = 0; j < 8; ++j) {
            currentSymbol <<= 1;
            currentSymbol |= (byteIn & 0x80) != 0;
            byteIn <<= 1;
            ++position;

            if (position == 8) {
                byteIn = (unsigned char) tableDecod[tableIndex++];

                position = 0;
            }
        }

        currentCount = 0;

        for (j = 0; j < 8; ++j) {
            currentCount <<= 1;
            currentCount |= (byteIn & 0x80) != 0;
            byteIn <<= 1;
            ++position;

            if (position == 8) {
                byteIn = (unsigned char) tableDecod[tableIndex++];

                position = 0;
            }
        }

        huffmanTable[currentSymbol].size = (size_t) currentCount;

        for (j = 0; j < currentCount; ++j) {
            huffmanTable[currentSymbol].text[j] = (byteIn & 0x80) != 0;

            byteIn <<= 1;
            ++position;

            if (position == 8) {
                byteIn = (unsigned char) tableDecod[tableIndex++];

                position = 0;
            }
        }
    }


    root = calloc(1, sizeof(Node));
    nodes = calloc(512, sizeof(Node));

    for (i = 0; i < COUNT_CHAR; ++i) {
        if (huffmanTable[i].size) {
            Node *node = root;

            for (j = 0; j < huffmanTable[i].size; ++j) {
                node = n_direction(node, huffmanTable[i].text[j], nodes);
            }

            node->ch = (unsigned char) i;
        }
    }

    return root;
}

void build_table(Node *root, Text *huffmanTable, Text *buffer) {
    unsigned char i;
    if (root->left != NULL) {
        buffer->text[buffer->size] = 0;
        buffer->size++;

        build_table(root->left, huffmanTable, buffer);
    }

    if (root->right != NULL) {
        buffer->text[buffer->size] = 1;
        buffer->size++;

        build_table(root->right, huffmanTable, buffer);
    }

    if (root->left == NULL && root->right == NULL) {
        for (i = 0; i < buffer->size; ++i) {
            huffmanTable[root->ch].text[i] = buffer->text[i];
            huffmanTable[root->ch].size = buffer->size;
        }
    }

    if (buffer->size != 0) {
        buffer->size--;
    }
}

Node *n_direction(Node *root, int direction, Node *nodes) {
    static int index = 0;
    if (direction) {
        if (!root->right) {
            Node *right = &nodes[index++];
            if (!right) {
                printf("Error memory");
                exit(1);
            }

            root->right = right;
        }

        return root->right;
    } else {
        if (!root->left) {
            Node *left = &nodes[index++];
            if (!left) {
                printf("Error memory");
                exit(1);
            }

            root->left = left;
        }

        return root->left;
    }
}
