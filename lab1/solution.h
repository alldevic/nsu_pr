#ifndef LAB1_SOLUTION_H
#define LAB1_SOLUTION_H

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
 * <li>NO_ERR - successful
 * <li>ERR_OPEN - problem with <b>INPUT</b>
 * <li>ERR_CREATE - problem with <b>OUTPUT</b>
 * <li>ERR_SCAN - can't read from <b>INPUT</b>
 * <li>ERR_PRINT - can't write to <b>OUTPUT</b>
 * <li>ERR_CLOSE - can't correct close file
 * <li>ERR_ALLOC - can't allocation memory
 * <li>ERR_NULL - nullable argument
 */
enum ERRORS {
    NO_ERR, ERR_OPEN, ERR_CREATE, ERR_SCAN,
    ERR_PRINT, ERR_CLOSE, ERR_ALLOC, ERR_NULL
};

/**
 * @def Macros for trapping errors from <bb>ERRORS</b> enum
 */
#define ERR(x,y) if (!x) return y;

#endif /*LAB1_SOLUTION_H*/
