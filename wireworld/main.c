#include <stdio.h>
#include <termios.h>

#define SYMBOL 40
#define WIDTH 14
#define HEIGHT 5

typedef enum CellStates {
    EMPTY = 0, HEAD, TAIL, WIRE
} State;

void set_keypress(void) {
    struct termios new_settings;
    tcgetattr(0, &new_settings);
    /* Disable canonical mode, and set buffer size to 1 byte */
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);
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
    set_keypress();
    State field[HEIGHT][WIDTH] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0},
            {2, 1, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3},
            {0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    }, tmpfield[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++)
            printCell(field[i][j]);
        printf("\n");
    }

    while (getchar()) {
        nextWorld((State *) field, (State *) tmpfield);
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                field[i][j] = tmpfield[i][j];
                printCell(field[i][j]);
            }
            printf("\n");
        }
    }
    return 0;
}
