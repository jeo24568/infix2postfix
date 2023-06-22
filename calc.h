#ifndef calc_h
#define calc_h

#include <stdio.h>
#include "stack.h"
#include "queue.h"

extern int isOperator[100];
extern int cntQ;

typedef enum {
    NO_ERROR,
    MODULO_BY_ZERO_ERROR,
    NOT_BALANCE_ERROR,
    WRONG_INPUT_ERROR,
    OUT_OF_RANGE,
    DIVISION_BY_ZERO_ERROR
} ErrorCode;

ErrorCode CheckBalance(char* input);
ErrorCode CheckExpression(char* input);
ErrorCode In2Post(char* input, int* cntOperand, int* cntOperator);
int opPrecedence(char op);
ErrorCode Calculator(double a, double b, char op, double* result);
ErrorCode cal(Queue** qf, Stack** st, double* resultPtr, char input[]);


#endif /* calc_h */
