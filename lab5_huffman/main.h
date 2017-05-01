#ifndef LAB5_MAIN_H
#define LAB5_MAIN_H
#define COUNT_CHAR 256
#define MY_BUFFER 512

#define IN "in.txt"
#define OUT "out.txt"


typedef struct Text {
    char text[512];
    size_t size;
} Text;


void encoding(FILE *fin, FILE *fout, FILE *tabl);

void decoding(FILE *fin, FILE *fout, FILE *tabl);

void write_table(FILE *tabl);

Node *read_table(FILE *tabl);

void build_table(Node *root, Text *huffmanTable, Text *buffer);

Node *n_direction(Node *root, int direction, Node *nodes);

#endif /*LAB5_MAIN_H*/
