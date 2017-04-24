#ifndef LAB5_SOLUTION_H
#define LAB5_SOLUTION_H

#define INPUT "in.txt"
#define OUTPUT "out.txt"
#define ENCODE 'c'
#define DECODE 'd'
#define ALPH_SIZE 256

int fencode(FILE *in, FILE *out);

int fdecode(FILE *in, FILE *out);

#endif /*LAB5_SOLUTION_H*/
