#ifndef LAB5_HUFFMAN_H
#define LAB5_HUFFMAN_H

struct QueueNode *buildHuffmanTree(char data[], unsigned int freq[], int size);

void printCodes(FILE *file,struct QueueNode *root);

#endif /*LAB5_HUFFMAN_H*/
