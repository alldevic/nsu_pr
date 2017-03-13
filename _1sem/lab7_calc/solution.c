/**
 * @mainpage Laboratory work №7.`
 * Parsing equation
 */
#include <stdlib.h>
#include <stdio.h>
#include "solution.h"

int main(void) {
    FILE *file;
    Stack *ops = stackInit(), *nums = stackInit();
    int ch = 1, last = -3, divZero = 0, syntax_err = 0, tmp = 0;
    file = fopen(INPUT, "r");
    while ((ch >= 0) && !divZero && !syntax_err) {
        syntax_err = 1;
        ch = fgetc(file);
        if (isDigit(ch)) {
            tmp = getNumber(file, &ch);
            NumberOpsHandler(ops, nums, tmp, &last, &divZero, &syntax_err);
        }
        ToOpsStack(ops, ch, &last, &syntax_err);
        RightBracketHandler(ops, nums, &last, &syntax_err);
    }
    if ((last != NUMBER) && (last != RIGHT_BRACKET) && !divZero)
        syntax_err = 1;
    fclose(file);
    setOutput(ops, nums, &divZero, syntax_err);
    return 0;
}

void ToOpsStack(Stack *ops, int ch, int *last, int *syntax_err) {
    if ((ch == '*') && (((*last) == NUMBER) || ((*last) == RIGHT_BRACKET)))
        addCorrect(ops, ch, MULT, last, syntax_err);
    else if ((ch == '/') && (((*last) == NUMBER) || ((*last) == RIGHT_BRACKET)))
        addCorrect(ops, ch, DIV, last, syntax_err);
    else if ((ch == '+') && (((*last) == NUMBER) || ((*last) == RIGHT_BRACKET)))
        addCorrect(ops, ch, SUM, last, syntax_err);
    else if ((ch == '-') && (((*last) == NUMBER) || ((*last) == RIGHT_BRACKET)))
        addCorrect(ops, ch, RAZN, last, syntax_err);
    else if ((ch == '(') && ((*last) != NUMBER))
        addCorrect(ops, ch, LEFT_BRACKET, last, syntax_err);
    else if ((stackTop(ops) == '(') && ((*last) = NUMBER) && (ch == ')'))
        stackPop(ops);
    else if (ch == ')')
        addCorrect(ops, ch, RIGHT_BRACKET, last, syntax_err);
    else if ((ch < 0) || (ch == '\r') || (ch == '\n'))
        (*syntax_err) = 0;
    else
        (*syntax_err) = 1;
}

void setOutput(Stack *ops, Stack *nums, int *divZero, int syntax_err) {
    FILE *file;
    getSum(ops, nums);
    getMult(ops, nums, divZero);
    file = fopen(OUTPUT, "w");
    if (syntax_err)
        fprintf(file, "syntax error");
    else if ((*divZero))
        fprintf(file, "division by zero");
    else if (stackIsEmpty(nums))
        fprintf(file, "syntax error");
    else
        fprintf(file, "%d", stackPop(nums));
    fclose(file);
}

void addCorrect(Stack *stack, int ch, int type, int *last, int *err) {
    stackPush(stack, ch);
    *last = type;
    *err = 0;
}

void NumberOpsHandler(Stack *ops, Stack *nums, int tmp, int *last, int *divZero, int *err) {
    if ((*last) == MULT) {
        addCorrect(nums, tmp * stackPop(nums), NUMBER, last, err);
        stackPop(ops);
    } else if ((*last) == DIV) {
        if (tmp == 0) {
            (*divZero) = 1;
            (*err) = 0;
        } else {
            addCorrect(nums, stackPop(nums) / tmp, NUMBER, last, err);
            stackPop(ops);
        }
    } else
        addCorrect(nums, tmp, NUMBER, last, err);
}

void RightBracketHandler(Stack *ops, Stack *nums, int *last, int *syntax_err) {
    if (((*last) == RIGHT_BRACKET) && !(*syntax_err)) {
        stackPop(ops);
        if (stackTop(ops) == '(')
            stackPop(ops);
        else {
            getSum(ops, nums);
            stackPop(ops);
            (*syntax_err) = 0;
            (*last) = NUMBER;
        }
    }
}

void getMult(Stack *ops, Stack *nums, int *divZero) {
    while (((stackTop(ops) == '*') || (stackTop(ops) == '/')) && !*divZero)
        if (stackPop(ops) == '*')
            stackPush(nums, stackPop(nums) * stackPop(nums));
        else if (stackTop(nums) == 0)
            *divZero = 1;
        else
            stackPush(nums, 1 / stackPop(nums) * stackPop(nums));
}

void getSum(Stack *ops, Stack *nums) {
    while ((stackTop(ops) == '+') || (stackTop(ops) == '-'))
        if (stackPop(ops) == '+')
            stackPush(nums, stackPop(nums) + stackPop(nums));
        else
            stackPush(nums, -stackPop(nums) - stackPop(nums));
}

int getNumber(FILE *f, int *base) {
    int res = 0;
    while ((isDigit(*base)) && (f)) {
        res *= 10;
        res += *base - '0';
        *base = fgetc(f);
    }
    return res;
}

int isDigit(int ch) {
    return ((ch >= '0') && (ch <= '9'));
}

Stack *stackInit() {
    Stack *stack = (Stack *) malloc(sizeof(Stack));
    stack->head = NULL;
    return stack;
}

int stackIsEmpty(Stack *stack) {
    return stack->head == NULL ? 1 : 0;
}

void *stackPush(Stack *stack, int value) {
    StackNode *node = malloc(sizeof(StackNode));
    node->prev = stack->head;
    node->data = value;
    stack->head = node;
    return node;
}

int stackTop(Stack *stack) {
    return stackIsEmpty(stack) ? 0 : stack->head->data;
}

int stackPop(Stack *stack) {
    StackNode *temp;
    int tt;
    if (stackIsEmpty(stack))
        return 0;
    temp = stack->head;
    tt = stackTop(stack);
    stack->head = temp->prev;
    free(temp);
    return tt;
}
 