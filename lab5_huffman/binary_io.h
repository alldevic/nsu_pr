#ifndef LAB5_BINARY_IO_H
#define LAB5_BINARY_IO_H

void setCount(int);

int getCount();

int readBit(FILE *);

int readByte(FILE *);

void writeBit(FILE *, int);

void writeCode(FILE *, long, int);

char findMax(int *, int, int *);

char getBit(unsigned char, int);

int readInt(FILE *);

void writeInt(FILE *, int);

long convertCode(char *);

#endif /*LAB5_BINARY_IO_H*/
