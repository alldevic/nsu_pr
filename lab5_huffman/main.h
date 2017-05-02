#ifndef LAB5_MAIN_H
#define LAB5_MAIN_H
#define COUNT_CHAR 256
#define BUFFER 512

#define INPUT "in.txt"
#define OUTPUT "out.txt"
#define TABLE "table"

typedef struct Text {
    char text[512];
    size_t size;
} Text;

int encoding(FILE *fin, FILE *fout, FILE *tabl);

int decoding(FILE *fin, FILE *fout, FILE *tabl);

int write_table(FILE *tabl, size_t quantityChar[], Text huffmanTable[]);

Node *read_table(FILE *tabl, Text huffmanTable[]);

void build_table(Node *root, Text *huffmanTable, Text *buffer);

Node *n_direction(Node *root, int direction, Node *nodes);

Node *writeNode(FILE *fout, Node *root, Node *node, unsigned char byteIn);

#endif /*LAB5_MAIN_H*/
