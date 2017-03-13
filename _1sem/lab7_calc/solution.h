#ifndef LAB7_SOLUTION_H
#define LAB7_SOLUTION_H

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
enum {
    ERR_OPEN = -7, ERR_CREATE,
    ERR_SCAN, ERR_PRINT, ERR_CLOSE,
    ERR_NULL, ERR_ALLOC, NO_ERR
};

enum {
    RIGHT_BRACKET = -2, LEFT_BRACKET,
    NUMBER, SUM, RAZN, MULT, DIV
};

typedef struct stackNode {
    int data;
    struct stackNode *prev;
} StackNode;

typedef struct stack {
    StackNode *head;
} Stack;

Stack *stackInit();

int stackIsEmpty(Stack *stack);

int stackPop(Stack *stack);

void *stackPush(Stack *stack, int value);

int stackTop(Stack *stack);

void setOutput(Stack *ops, Stack *nums, int *divZero, int syntax_err);

void ToOpsStack(Stack *ops, int ch, int *last, int *syntax_err);

int isDigit(int ch);

int getNumber(FILE *f, int *base);

void getSum(Stack *ops, Stack *nums);

void getMult(Stack *ops, Stack *nums, int *divZero);

void RightBracketHandler(Stack *ops, Stack *nums, int *last, int *syntax_err);

void NumberOpsHandler(Stack *ops, Stack *nums, int tmp, int *last, int *divZero, int *err);

void addCorrect(Stack *stack, int ch, int type, int *last, int *err);

#endif /*LAB7_SOLUTION_H*/
