#include <stdio.h>

#define WIDTH 10
#define HEIGHT 3

#define COLOR 1
#define SYMBOL 40


typedef enum CellStates {
    EMPTY = ' ', HEAD = 'g', TAIL = 'o', WIRE = 'O'
} State;


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

State nextWorld(State *old, State *new) {
    int i;
    for (i = 0; i < WIDTH * HEIGHT; i++) {
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
                int hc = (old[i - WIDTH - 1] == HEAD) + (old[i - WIDTH] == HEAD) + (old[i - WIDTH + 1] == HEAD) +
                         (old[i - 1] == HEAD) + (old[i + 1] == HEAD) +
                         (old[i + WIDTH - 1] == HEAD) + (old[i + WIDTH] == HEAD) + (old[i + WIDTH + 1] == HEAD);
                new[i] = (hc == 1 || hc == 2) ? HEAD : WIRE;
                break;
            }
        }
    }
    new[i] = old[i];
}

int main(void) {
    State field[HEIGHT][WIDTH] = {
            {EMPTY, EMPTY, EMPTY, EMPTY, WIRE, WIRE, EMPTY, EMPTY, EMPTY, EMPTY},
            {TAIL, HEAD, WIRE, WIRE, WIRE, EMPTY, WIRE, WIRE, WIRE, WIRE},
            {EMPTY, EMPTY, EMPTY, EMPTY, WIRE, WIRE, EMPTY, EMPTY, EMPTY, EMPTY},
    }, tmpfield[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++)
            COLOR ? printCell(field[i][j]):printf("%c", field[i][j]);
        printf("\n");
    }

    while (getchar()) {
        nextWorld((State *) field, (State *) tmpfield);
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                field[i][j] = tmpfield[i][j];
                COLOR ? printCell(field[i][j]):printf("%c", field[i][j]);
            }
            printf("\n");
        }
    }
    return 0;
}
