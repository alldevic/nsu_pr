/**
 * @mainpage Laboratory work #9
 * Dictionary
 */
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "solution.h"

/**
 * @function Calculating keys for new dictionary items
 * @param str - new dictionary item
 * @return key for item
 */
unsigned char hash(char *str) {
    int res = 0, i = 0;
    while (str[i++] != '\0')
        res += str[i - 1];
    return (unsigned char) (res / (i - 1));
}

/**
 * @function Adding node with <b>value</b> data to <b>list</b>
 * @param list - list for adding
 * @param value - new data for item
 * @return error code
 */
int lstAddTo(LST list, char *value) {
    if (!list) {
        list = malloc(sizeof(LSTNode));
        ERR(list, ERR_ALLOC);
        list->next = NULL;
        strcpy(list->data, value);
    } else if (!list->data)
        strcpy(list->data, value);
    else if (!list->next) {
        LST newNode = malloc(sizeof(LSTNode));
        ERR(newNode, ERR_ALLOC);
        strcpy(newNode->data, value);
        newNode->next = NULL;
        list->next = newNode;
    } else
        return lstAddTo(list->next, value);
    return NO_ERR;
}

/**
 * @function Write char string to <b>OUTPUT</b>
 * @param str - string for output
 * @return error code
 */
int setOutput(char *str) {
    FILE *file = fopen(OUTPUT, "w");
    ERR(file, ERR_CREATE);
    ERR(fprintf(file, "%s", str) < 0, ERR_PRINT);
    ERR(fclose(file) == EOF, ERR_CLOSE);
    return NO_ERR;
}

/**
 * @function Search item with <b>word</b> data in <b>list</b>
 * @param list - list for searching
 * @param word - value of item
 * @return 1 - if exists, 0 - else
 */
int isInclude(LST list, char *word) {
    ERR(list, 0);
    return !strcmp(word, list->data) ? 1 : isInclude(list->next, word);
}

/**
 * @function Entry point of program. Read lines from <b>INPUT</b>,
 * adding them to dictionary and find needed element.
 * @return error code
 */
int main(void) {
    FILE *file;
    LST dict[MAX_LEN] = {0};
    char keyWord[MAX_LEN] = {0}, word[MAX_LEN] = {0};
    int i = 0;
    ERR((file = fopen(INPUT, "r")) != NULL, ERR_OPEN);

    while (i < MAX_LEN)
        ERR((dict[i++] = malloc(sizeof(LSTNode))) != NULL, ERR_ALLOC);

    ERR(fscanf(file, "%s", keyWord) < 0, ERR_SCAN);
    while ((fscanf(file, "%s", word) > 0) && (lstAddTo(dict[hash(word)], word) == NO_ERR));
    ERR(fclose(file) == EOF, ERR_CLOSE);
    return setOutput(isInclude(dict[hash(keyWord)], keyWord) ? "YES" : "NO");
}
