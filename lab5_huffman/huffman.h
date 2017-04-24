#ifndef LAB5_HUFFMAN_H
#define LAB5_HUFFMAN_H

struct QueueNode *buildHuffmanTree(char data[], unsigned int freq[], int size);

void printCodes(struct QueueNode *root, int arr[], int top);

void HuffmanCodes();

#endif /*LAB5_HUFFMAN_H*/
