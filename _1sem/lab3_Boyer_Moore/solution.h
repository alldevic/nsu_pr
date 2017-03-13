#ifndef LAB3_SOLUTION_H
#define LAB3_SOLUTION_H

/**
 * #def File with source values
 * Default for problem: <b>"in.txt"</b>
 */
#define INPUT "in.txt"

/**
 * @def File with results<br>
 * Default for problem: <b>"out.txt"</b>
 */
#define OUTPUT "out.txt"

/**
 * @def Length of pattern<br>
 * Default for problem: <b>16</b>, but also include '\0' and EOF
 */
#define PAT_LEN 16

/**
 * @enum List of errors:<ul>
 * <li>NO_ERR - succesfull
 * <li>ERR_OPEN - problem with <b>INPUT</b>
 * <li>ERR_CREATE - problem with <b>OUTPUT</b>
 * <li>ERR_SCAN - can't read from <b>INPUT</b>
 * <li>ERR_PRINT - can't wtite to <b>OUTPUT</b>
 * <li>ERR_CLOSE - can't correct close file
 * <li>ERR_ALLOC - cab't allocation memory
 */
enum ERRORS {NO_ERR, ERR_OPEN, ERR_CREATE,
    ERR_SCAN, ERR_PRINT, ERR_CLOSE, ERR_ALLOC};

int str_len(char *str);
int get_input(char *pattern, char *text, char **s);
void init_occ(char *template, int *occ);
#endif /*LAB3_SOLUTION_H*/
