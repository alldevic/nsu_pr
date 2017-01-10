#include <stdio.h>
#include <stdlib.h>

#define COLOR 1
#define SYMBOL 40


typedef enum CellStates {
    EMPTY = ' ', HEAD = 'g', TAIL = 'o', WIRE = 'O'
} State;

char *rle_decode(char *str)
{
    return str;
}
void printCell(State st) {
    switch (st) {
        case EMPTY:
            printf("\x1b[30m\x1b[40m%c\x1b[0m", SYMBOL);
            break;
        case HEAD:
            printf("\x1b[34m\x1b[44m%c\x1b[0m", SYMBOL);
            break;
        case TAIL:
            printf("\x1b[31m\x1b[41m%c\x1b[0m", SYMBOL);
            break;
        case WIRE:
            printf("\x1b[33m\x1b[43m%c\x1b[0m", SYMBOL);
            break;
    };
}

State nextWorld(char *old, char *new, int height, int width) {
    int i;
    for (i = 0; i < width * height; i++) {
        switch (old[i]) {
            case EMPTY:
                new[i] = EMPTY;
                break;
            case TAIL:
                new[i] = WIRE;
                break;
            case HEAD:
                new[i] = TAIL;
                break;
            case WIRE: {
                int hc = (old[i - width - 1] == HEAD) + (old[i - width] == HEAD) + (old[i - width + 1] == HEAD) +
                         (old[i - 1] == HEAD) + (old[i + 1] == HEAD) +
                         (old[i + width - 1] == HEAD) + (old[i + width] == HEAD) + (old[i + width + 1] == HEAD);
                new[i] = (hc == 1 || hc == 2) ? HEAD : WIRE;
                break;
            }
            default:break;
        }
    }
    new[i] = old[i];
}

int main(int argc, char **argv) {
    FILE *input = fopen(argv[1],"r");
    if (!input) return -1;
    int height, width;
    if(fscanf(input, "%d", &height) < 0)
        return -2;
    if(fscanf(input, "%d", &width) < 0)
        return -2;
    
    char *str = malloc(height*width*sizeof(char));
    char *tmpstr = malloc(height*width*sizeof(char));
    if (!str || !tmpstr)
        return -3;
    
    if(fscanf(input, "%s", str) < 0)
        return -4;

    str = rle_decode(str);


    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            COLOR ? printCell((State) str[i * height + j]) : printf("%c", str[i * height + j]);
        printf("\n");
    }

    while (getchar()) {
        nextWorld(str, tmpstr, height, width);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                str[i*height+j] = tmpstr[i*height+j];
                COLOR ? printCell((State) str[i * height + j]) : printf("%c", str[i * height + j]);
            }
            printf("\n");
        }
    }
    return 0;
}
