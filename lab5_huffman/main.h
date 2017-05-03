#ifndef LAB5_MAIN_H
#define LAB5_MAIN_H

/**
 * Count of char in alphabet
 */
#define COUNT_CHAR 256

/**
 * Buffer size
 */
#define BUFF 512

#define INPUT "in.txt"
#define OUTPUT "out.txt"
#define TABLE "table"

/**
 * Block of text
 */
typedef struct Text {
    char text[512];
    size_t size;
} Text;

int encoding(FILE *in, FILE *out, FILE *tab);

int decoding(FILE *in, FILE *out, FILE *tab);

int write_table(FILE *tab, size_t quantityChar[], Text table[]);

Node *read_table(FILE *tab, Text table[]);

void build_table(Node *root, Text *table, Text *buffer);

Node *n_direction(Node *root, int direction, Node *nodes);

Node *writeNode(FILE *out, Node *root, Node *node, unsigned char byteIn);

void writeLetter(FILE *file, unsigned char letter, int *pos, int *byte);

unsigned char readByte(char *byte, unsigned char *pos, char *table, size_t *index);

Node *createHuffmanTree(Text *table);

#endif /*LAB5_MAIN_H*/
