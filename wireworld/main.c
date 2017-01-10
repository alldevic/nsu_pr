#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define COLOR 1
#define OFFLINE 0
#define SYMBOL 40


typedef enum CellStates {
    EMPTY = 'q', HEAD = 'g', TAIL = 'o', WIRE = 'e'
} State;

char *rle_decode(char *str) {
    return str;
}

char *rle_encode(char *str) {
    int cnt;
    char smb, *encode = calloc(strlen(str),sizeof(char)), *newstr = calloc(strlen(str),sizeof(char));
    smb = str[0];
    cnt = 0;

    for (int i = 0; i <= strlen(str); i++) {
        if (str[i]==smb) {
            cnt++;
        }
        else {
            sprintf(newstr, "%d", cnt);
            strcat(encode, newstr);
            sprintf(newstr, "%c", smb);
            strcat(encode, newstr);
            smb = str[i];
            cnt = 1;
        }
    }
    return encode;
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
            default:
                break;
        }
    }
    new[i] = old[i];
}

int main(int argc, char **argv) {
    FILE *file = fopen(argv[1], "r");

    if (!file) return -1;
    int height, width;
    if (fscanf(file, "%d", &height) < 0)
        return -2;
    if (fscanf(file, "%d", &width) < 0)
        return -2;
    char *str = malloc(height * width * sizeof(char));
    char *tmpstr = malloc(height * width * sizeof(char));
    if (!str || !tmpstr)
        return -3;
    while (fscanf(file, "%s", tmpstr) > 0)
        strcat(str, tmpstr);
    fclose(file);

    int count = atoi(argv[3]);

    str = rle_decode(str);
    if (!OFFLINE) {
        for (int i = 0; i < height * width; i++) {
            if ((i % width == 0) && (i != 0))
                printf("\n");
            COLOR ? printCell((State) str[i]) : printf("%c", str[i]);
        }
        printf("\n");
    }

    while (count > 0) {
        if (!OFFLINE)
            printf("\n");
        nextWorld(str, tmpstr, height, width);

        for (int i = 0; i < height * width; i++) {
            str[i] = tmpstr[i];
            if ((i % width == 0) && (i != 0) && !OFFLINE)
                printf("\n");
            (!OFFLINE) ? (COLOR ? printCell((State) str[i]) : printf("%c", str[i])) : NULL;
        }
        if (!OFFLINE)
            printf("\n");

        count--;
    }

    file = fopen(argv[2], "w");
    if (!file)
        return -4;
    fprintf(file, "%d %d\n", height, width);
    fprintf(file, "%s", rle_encode(str));
    fclose(file);
    return 0;
}
