/**
 * @mainpage Laboratory work №3.
 * Matching algorithm Boyer-Moore.
 */

#include <stdio.h>
#include <malloc.h>
#include "solution.h"


/**
 * @function Entry point  to program. On first step the variables get values from
 INPUT,
 * calculate of the occurance of symbols for pattern, open OUTPUT for write. Next step, in
 * while loop all needs symbols writing to OUTPUT.
 * @return err - error code
 */
int main(void)
{
    FILE *f_out;
    char pattern[PAT_LEN] = "";
    char *text = (char *)realloc(NULL, PAT_LEN);
    int err = get_input(pattern, text, &text), occ[255], fl = 0;
    int b_len = str_len(text), p_len = str_len(pattern), i = p_len, k = p_len;
    init_occ(pattern, occ);
    err = (((f_out = fopen(OUTPUT, "w")) == NULL) || err) ? ERR_CREATE : err;
    fl = (text[0] != EOF) && (text[0] != '\0') && (pattern[0] != '\0');
    while ((i <= b_len) && fl && !err)
    {
        while ((text[i - 1] == pattern[--k]) && (k >= 0) && !err)
        {
            err = (fprintf(f_out, (i == p_len) ? "%d" : " %d", i) < 0) ? ERR_PRINT : err;
            i--;
        }
        if ((k > -1) && !err)
            err = (fprintf(f_out, (i == p_len) ? "%d" : " %d", i) < 0) ? ERR_PRINT : err;
        /**When k=-1 (because --k) the <b>i</b> position is  position of <b>i</b>
         * before loop iteration substarct length of pattern.*/
        i += (k > -1) ? occ[(int) text[i - 1]] : 2 * p_len;
        k = p_len;
    }
    free(text);
    return ((fclose(f_out) == EOF) && !err) ? ERR_CLOSE : err;
}


/**
 * Get and correct <b>pattern</b> and <b>text</b> from INPUT.
 * @param pattern - the pattern of search
 * @param text - text for search
 * @return err - error code
 */
int get_input(char *pattern, char *text, char **s)
{
    FILE *f_in;
    char *a;
    int err = NO_ERR, k = 0, i = 1;
    if ((f_in = fopen(INPUT, "r")) != NULL)
    {
        err = (fscanf(f_in, "%[^\r\n]\r\n", pattern) < 0) ? ERR_SCAN : err;

        while (((text[k++] = (char)fgetc(f_in)) != EOF) && !err)
            if ((k % PAT_LEN) == 0)
            {
                a = realloc(text, (size_t) (PAT_LEN * ++i));
                if (a != NULL)
                    text = a;
                else
                    err = ERR_ALLOC;
            }

        *s = text;

        /**
         * Remove the EOF symbol from <b>text</b>
         */
        text[k - 1] = '\0';

        /**
         * Remove the OS-specific line separator from end of our file. If our file ends
         * on kind of "\\r\\n\0" our text will be mismatch with text from tests. Because
         * we remove line separator from end of file: '\\n' - for Unic and OSX,
         * "\\r\\n" - for WIndows and '\\r' for classic Mac.
         */
        text[k - 2] = (char) ((text[k - 2] == '\n') ? '\0' : text[k - 2]);
        text[k - 2] = (char) ((text[k - 2] == '\r') ? '\0' : text[k - 2]);
        text[k - 3] = (char) ((text[k - 3] == '\r') ? '\0' : text[k - 3]);

        err = ((fclose(f_in) == EOF) && !err) ? ERR_CLOSE : err;
    } else
        err = ERR_OPEN;
    return err;
}


/**
 * @function It's a preprocessiong function for Boyer-Moore-Horspool algorithm. For
 * every symbol in <b>pattern</b> this function return the value of stop-symbol
 * heuristic.
 * @param pattern - the pattern of search
 * @param occ - table of values of heuristic
 */
void init_occ(char *pattern, int *occ)
{
    int i;

    for (i = 0; i < 255; i++)
        occ[i] = str_len(pattern);

    for (i = 0; i < str_len(pattern) - 1; i++)
        occ[(int) pattern[i]] = str_len(pattern) - i - 1;
}


/**
 * Calculate length of string
 * @param str - input string
 * @return len - length of <b>str</b>
 */
int str_len(char *str)
{
    int len = 0;
    while (str[++len] != '\0');
    return len;
}
