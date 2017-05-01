#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "Huffman.h"

Text huffmanTable[COUNT_CHAR];

int quantityChar[COUNT_CHAR] = {0};

unsigned char text[MY_BUFFER] = {0};


int main(void) {
    FILE *fin = NULL;
    FILE *fout = NULL;
    FILE *tabl = NULL;

    fin = fopen(IN, "rb");
    fout = fopen(OUT, "wb");
    if ((fin == NULL) || (fout == NULL)) {
        return 1;
    }

    unsigned char huffmanFlag[3];

    fread(huffmanFlag, sizeof(unsigned char), 3, fin);

    for (int i = 0; i < COUNT_CHAR; ++i) {
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
    int size = (int) fread(text, sizeof(unsigned char), MY_BUFFER, fin);

    if (size == 0) {
        fclose(fin);
        fclose(fout);
        exit(0);
    }

    //fprintf(fout, "d\r\n");

    int flag = 1;
    while (flag) {
        for (int i = 0; i < size; ++i) {
            ++quantityChar[text[i]];
        }
        if (size == MY_BUFFER) {
            size = (int) fread(text, sizeof(unsigned char), MY_BUFFER, fin);
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

    unsigned char byteOut = 0;
    unsigned char position = 0;

    fin = fopen(IN, "rb");

    flag = 1;
    fread(text, sizeof(unsigned char), 3, fin);
    size = (int) fread(text, sizeof(unsigned char), MY_BUFFER, fin);

    while (flag) {
        for (int k = 0; k < size; ++k) {
            unsigned char rem = text[k];

            for (int j = 0; j < huffmanTable[rem].size; ++j) {
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
            size = (int) fread(text, sizeof(unsigned char), MY_BUFFER, fin);
        } else {
            flag = 0;
        }
    }

    if (position != 0) {
        byteOut <<= 8 - position;
        fprintf(fout, "%c", byteOut);
    }

    if (position == 0) {
        position = 8;
    }

    fprintf(fout, "%c", position);
}

void decoding(FILE *fin, FILE *fout, FILE *tabl) {
    int size = (int) fread(text, sizeof(unsigned char), MY_BUFFER, fin);

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

    Node *root = read_table(tabl);

    Node *node = root;

    int index = 0;
    unsigned char byteIn = text[index++];
    unsigned char position = 0;

    while (size == MY_BUFFER) {
        for (int i = 0; i < 8; ++i) {
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
                    size = (int) fread(text, sizeof(unsigned char), MY_BUFFER, fin);
                    index = 0;

                    if (size != MY_BUFFER) {
                        break;
                    }
                }
            }
        }
    }

    int runner = 0;

    while (index < size - 2) {
        runner = 1;
        for (int i = 0; i < 8; ++i) {
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

    for (int i = position; (i < 8) && runner; ++i) {
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
    int offset = text[size - 1];

    if (offset == 0) {
        offset = 8;
    }

    for (int i = 0; i < offset; ++i) {
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
    unsigned char countChar = 0;

    List *tree = NULL;

    for (int i = 0; i < COUNT_CHAR; ++i) {
        if (quantityChar[i] != 0) {
            Node *l = calloc(1, sizeof(Node));
            if (l == NULL) {
                printf("Error memory.");
                exit(1);
            }

            l->ch = (unsigned char) i;
            l->size = (unsigned long long int) quantityChar[i];

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

    Text buffer;
    buffer.size = 0;

    Node *root = tree->node;

    if (root->left && root->right) {
        build_table(root, huffmanTable, &buffer);
    } else {
        huffmanTable[root->ch].size = 1;
        huffmanTable[root->ch].text[0] = 0;
    }

    unsigned char byteOut = 0;
    char position = 0;

    fprintf(tabl, "%c", countChar);

    for (int i = 0; i < COUNT_CHAR; ++i) {
        if (quantityChar[i]) {
            unsigned char letter = (unsigned char) i;

            for (int j = 0; j < 8; ++j) {
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

            for (int j = 0; j < 8; ++j) {
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

            for (int j = 0; j < huffmanTable[i].size; ++j) {
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
    fread(tableDecod, sizeof(unsigned char), 1000, tabl);

    unsigned char byteIn = 0;
    unsigned char position = 0;
    int tableIndex = 0;
    int countDecod = tableDecod[tableIndex++];

    if (countDecod == 0) {
        countDecod = 256;
    }

    byteIn = (unsigned char) tableDecod[tableIndex++];

    for (int i = 0; i < countDecod; ++i) {
        unsigned char currentSymbol = 0;

        for (int j = 0; j < 8; ++j) {
            currentSymbol <<= 1;
            currentSymbol |= (byteIn & 0x80) != 0;
            byteIn <<= 1;
            ++position;

            if (position == 8) {
                byteIn = (unsigned char) tableDecod[tableIndex++];

                position = 0;
            }
        }

        int currentCount = 0;

        for (int j = 0; j < 8; ++j) {
            currentCount <<= 1;
            currentCount |= (byteIn & 0x80) != 0;
            byteIn <<= 1;
            ++position;

            if (position == 8) {
                byteIn = (unsigned char) tableDecod[tableIndex++];

                position = 0;
            }
        }

        huffmanTable[currentSymbol].size = currentCount;

        for (int j = 0; j < currentCount; ++j) {
            huffmanTable[currentSymbol].text[j] = (byteIn & 0x80) != 0;

            byteIn <<= 1;
            ++position;

            if (position == 8) {
                byteIn = (unsigned char) tableDecod[tableIndex++];

                position = 0;
            }
        }
    }


    Node *root = calloc(1, sizeof(Node));
    Node *nodes;
    nodes = calloc(512, sizeof(Node));

    for (int i = 0; i < COUNT_CHAR; ++i) {
        if (huffmanTable[i].size) {
            Node *node = root;

            for (int j = 0; j < huffmanTable[i].size; ++j) {
                node = n_direction(node, huffmanTable[i].text[j], nodes);
            }

            node->ch = (unsigned char) i;
        }
    }

    return root;
}

void build_table(Node *root, Text *huffmanTable, Text *buffer) {
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
        for (unsigned char i = 0; i < buffer->size; ++i) {
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
            //Node *right = calloc(1, sizeof(Node));
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
            //Node *left = calloc(1, sizeof(Node));
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
