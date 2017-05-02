#ifndef LAB5_MAIN_H
#define LAB5_MAIN_H
#define COUNT_CHAR 256u
#define MY_BUFFER 512

#define IN "in.txt"
#define OUT "out.txt"

typedef struct Text {
    char text[512];
    size_t size;
} Text;

int encoding(FILE *fin, FILE *fout, FILE *tabl,
             unsigned char text[], size_t quantityChar[], Text huffmanTable[]);

int decoding(FILE *fin, FILE *fout, FILE *tabl,
             unsigned char text[], Text huffmanTable[]);

int write_table(FILE *tabl, size_t quantityChar[], Text huffmanTable[]);

Node *read_table(FILE *tabl, Text huffmanTable[]);

void build_table(Node *root, Text *huffmanTable, Text *buffer);

Node *n_direction(Node *root, int direction, Node *nodes);

#endif /*LAB5_MAIN_H*/
