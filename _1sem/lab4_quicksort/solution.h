#ifndef LAB4_SOLUTION_H
#define LAB4_SOLUTION_H

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
 * @enum List of errors:<ul>
 * <li>NO_ERR - succesfull
 * <li>ERR_OPEN - problem with <b>INPUT</b>
 * <li>ERR_CREATE - problem with <b>OUTPUT</b>
 * <li>ERR_SCAN - can't read from <b>INPUT</b>
 * <li>ERR_PRINT - can't wtite to <b>OUTPUT</b>
 * <li>ERR_CLOSE - can't correct close file
 * <li>ERR_ALLOC - cab't allocation memory
 */
enum ERRORS
{
    NO_ERR, ERR_OPEN, ERR_CREATE,
    ERR_SCAN, ERR_PRINT, ERR_CLOSE, ERR_ALLOC
};

int get_input(int *n, int **array);

void quick_sort(int n, int *array);

int set_output(int *n, int *array);

#endif /*LAB4_SOLUTION_H*/
