//
//  calc.c
//  DataStructure_Project02
//
//  Created by 김재환 on 2023/06/04.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>

#include "calc.h"
#include "stack.h"
#include "queue.h"

int opFlag[100] = {0};
int cntQ = 0;

// MARK: 괄호쌍 검사
ErrorCode CheckBalance(char* input) {
    Stack* checkSt = NULL;
    int i;
    int LENGTH = (int)strlen(input);
    
    for (i=0; i<LENGTH; i++) {
        if (input[i] == '(') {
            Push(&checkSt, input[i]);
        } else if (input[i] == ')') {
            if (IsStackEmpty(&checkSt)) {
                return NOT_BALANCE_ERROR;
            }
            else {Pop(&checkSt);}
        }
    }
    if (!IsStackEmpty(&checkSt)) {
        return NOT_BALANCE_ERROR;
    }
    return NO_ERROR;
}

// MARK: 잘못된 입력 검사
ErrorCode CheckExpression(char* input) {
    int i, tmp;
    int LENGTH = (int)strlen(input);
    
    for (i=0; i<LENGTH; i++) {
        tmp = input[i];
        // 공백은 허용
        if (tmp == ' ')
            continue;

        if (!(tmp >= '0' && tmp <= '9') && !(tmp == '+' || tmp == '-' || tmp == '*' ||
                                              tmp == '/' || tmp == '%' || tmp == '^' ||
                                              tmp == '(' || tmp == ')' || tmp == 'l')) {
            return WRONG_INPUT_ERROR;
        } else if (tmp == 'l') {
            if (input[i+1] == 'o' && input[i+2] == 'g') {
                if (input[i+3] == '2') {
                    i += 2;
                } else if (input[i+3] == '1' && input[i+4] == '0') {
                    i += 3;
                } else {
                    return WRONG_INPUT_ERROR;
                }
            } else {
                return WRONG_INPUT_ERROR;
            }
        }
    }
    return NO_ERROR;
}

// MARK: Infix to Postfix
// 입력받은 문자열을 한글자씩 분해해서 0~9 사이면 tmpstr에 붙여서 atol로 한번에 enqueue
// 0~9 사이가 아닌 43과 같은 operator면 stack에 push하고 top의 연산자의 우선순위가 새로 push해야하는 연산자의 우선순위보다 낮다면 enqueuq, 높다면 그대로 push
// 예외: operand의 수와 operation의 수를 비교해서 단항연산자 판별 (cntOpnd, cntOptor)
// 예외: operator이면 isOperator에 1 삽입. 아니면 1이 아닌 값을 삽입해서 계산할 때 정수와 연산자를 구분한다 (43과 '+' 같은 경우)
ErrorCode In2Post(char* input, int* cntOperand, int* cntOperator) {
    int i, cntOpnd = 0, cntOptor = 0;
    int LENGTH = (int)strlen(input);
    
    for (i=0; i<LENGTH; i++) {
        char tmpStr[100] = "";
        
        // input[i]가 0~9일 경우 strcat. 연산자 만나면 while 종료 후 atol[str] 을 enque
        while (input[i] >= '0' && input[i] <= '9') {
            int tmpStrLength = (int)strlen(tmpStr);
            tmpStr[tmpStrLength] = input[i];
            tmpStr[tmpStrLength + 1] = '\0';
            i++;
        }
        // tmpstr이 비어있지 않은 경우 enque
        if ((int)strlen(tmpStr) != 0) {
            // 입력 정수의 범위가 long의 범위를 벗어났을 경우의 예외처리
            char* endPtr;
            long num = strtol(tmpStr, &endPtr, 10);
            if (*endPtr != '\0' || num == LONG_MAX || num == LONG_MIN) {
                return OUT_OF_RANGE; // 변환 중 오류가 발생하거나 범위를 벗어난 경우
            }
            Enque(&front, num);
            opFlag[cntQ] = 0;
            cntQ++;
            cntOpnd++;
        }
        switch (input[i]) {
            case '(':
                Push(&st, input[i]);
                break;
            case ')':
                while (Top(&st) != '(') {
                    Enque(&front, Top(&st));
                    opFlag[cntQ] = 1;
                    cntQ++;
                    Pop(&st);
                }
                Pop(&st);
                break;
            case '+':
            case '-':
                // 단항 연산자로 처리되는 경우 바로앞에 연산자가 오는경우의 예외처리
                if (input[i - 1] == '+'|| input[i - 1] == '-' || input[i - 1] == '*' || input[i - 1] == '/' || input[i - 1] == '^') {
                    return WRONG_INPUT_ERROR;
                }
                // 단항 연산자로 처리되는 경우
                if ((i == 0 || (input[i - 1] < '0' || input[i - 1] > '9')) && input[i + 1] >= '0' && input[i + 1] <= '9') {
                    
                    int j = i + 1;
                    while (j < LENGTH && input[j] >= '0' && input[j] <= '9') {
                        j++;
                    }
                    char tmpStr[100];
                    strncpy(tmpStr, input + i, j - i);
                    tmpStr[j - i] = '\0';

                    // 입력 정수의 범위가 long의 범위를 벗어났을 경우의 예외처리
                    char* endPtr;
                    long num = strtol(tmpStr, &endPtr, 10);
                    if (*endPtr != '\0' || num == LONG_MAX || num == LONG_MIN) {
                        return OUT_OF_RANGE; // 변환 중 오류가 발생하거나 범위를 벗어난 경우
                    }

                    Enque(&front, num); // long 값을 enqueue
                    opFlag[cntQ] = 0;
                    cntQ++;
                    cntOpnd++;

                    i = j - 1; // 다음 처리할 인덱스로 이동
                }
                // 이항 연산자로 처리되는 경우
                else {
                    // empty
                    if (IsStackEmpty(&st)) {
                        Push(&st, input[i]);
                        cntOptor++;
                    }
                    // not empty
                    else {
                        if (opPrecedence(Top(&st)) >= opPrecedence(input[i])) {
                            Enque(&front, Top(&st));
                            Pop(&st);
                            opFlag[cntQ] = 1;
                            cntQ++;
                            i--;
                        } else {
                            Push(&st, input[i]);
                            cntOptor++;
                        }
                    }
                }
                break;
            case '*':
            case '/':
            case '%':
            case '^':
                // empty
                if (IsStackEmpty(&st)) {
                    Push(&st, input[i]);
                    cntOptor++;
                }
                // not empty
                else {
                    if (opPrecedence(Top(&st)) >= opPrecedence(input[i])) {
                        Enque(&front, Top(&st));
                        Pop(&st);
                        opFlag[cntQ] = 1;
                        cntQ++;
                        i--;
                    } else {
                        Push(&st, input[i]);
                        cntOptor++;
                    }
                }
                break;
            default:
                // Check for log operators
                if (i < LENGTH - 2 && input[i] == 'l' && input[i + 1] == 'o' && input[i + 2] == 'g') {
                    i += 2;
                    if (i < LENGTH - 1 && input[i + 1] == '2') {
                        if (IsStackEmpty(&st)) {
                            Push(&st, '@');
                            cntOptor++;
                            i++;
                        } else {
                            if (opPrecedence(Top(&st)) >= opPrecedence('@')) {
                                Enque(&front, '@');
                                Pop(&st);
                                opFlag[cntQ] = 1;
                                cntQ++;
                            } else {
                                Push(&st, '@');
                                cntOptor++;
                                i++;
                            }
                        }
                        break;
                    } else if (i < LENGTH - 2 && input[i + 1] == '1' && input[i + 2] == '0') {
                        if (IsStackEmpty(&st)) {
                            Push(&st, '#');
                            cntOptor++;
                            i += 2;
                        }
                        else {
                            if (opPrecedence(Top(&st)) >= opPrecedence('#')) {
                                Enque(&front, '#');
                                Pop(&st);
                                opFlag[cntQ] = 1;
                                cntQ++;
                                i += 2;
                            } else {
                                Push(&st, '#');
                                cntOptor++;
                                i += 2;
                            }
                        }
                        break;
                    } else {
                        return WRONG_INPUT_ERROR;
                    }
                }
        }
    }
    // stack에 남은 연산자들 모두 enqueue
    while (!IsStackEmpty(&st)) {
        Enque(&front, Top(&st));
        opFlag[cntQ] = 1;
        cntQ++;
        Pop(&st);
    }
    *cntOperand = cntOpnd;
    *cntOperator = cntOptor;
    return NO_ERROR;
}

// MARK: 연산자 우선순위 설정
int opPrecedence(char op) {
    switch(op) {
        case '(':
        case ')':
            return 0;
        case '^':
            return 3;
        case '*':
        case '/':
        case '%':
            return 2;
        case '+':
        case '-':
            return 1;
        case '@': // log2
            return 2;
        case '#': // log10
            return 2;
    }
    return -1;
}

// MARK: 계산기
ErrorCode Calculator(double a, double b, char op, double* result) {
    int i;
    int sign_flag = 0;
    switch(op) {
        case '+':
            *result = a + b;
            break;
        case '-':
            *result = a - b;
            break;
        case '*':
            if ((a > 0 && b > 0 && a > LONG_MAX / b) ||
                (a < 0 && b < 0 && a < LONG_MAX / b) ||
                (a < 0 && b > 0 && a < LONG_MIN / b) ||
                (a > 0 && b < 0 && b < LONG_MIN / a)) {
                return OUT_OF_RANGE;
            }
            *result = a * b;
            break;
        case '/':
            if (b == 0) {
                return DIVISION_BY_ZERO_ERROR;
            }
            *result = a / b;
            break;
        case '%':
            if (b == 0) {
                return MODULO_BY_ZERO_ERROR;
            }
            *result = (long)a % (long)b;
            break;
        case '^':
            sign_flag = 0;
            if (b != (long)b) { return WRONG_INPUT_ERROR; } // b가 정수가 아닌 경우 오류 반환
            if (a < 0) { sign_flag = 1; a *= -1; }
            if (b < 0) {
                if (a == 0) { return DIVISION_BY_ZERO_ERROR; } // 0을 0으로 나누려고 할 때 오류 반환
                *result = 1;
                for (i = 0; i > b; i--) {
                    // 값 표현범위 넘어서는지 검사
                    if (*result < LONG_MIN / a) { return OUT_OF_RANGE; }
                    *result /= a;
                }
            } else {
                *result = 1;
                for (i = 0; i < b; i++) {
                    // 값 표현범위 넘어서는지 검사
                    if (*result > LONG_MAX / a) { return OUT_OF_RANGE; }
                    *result *= a;
                }
            }
            if (sign_flag == 1) { *result *= -1; }
            break;
        case '@': // log2
            if (a <= 0) {
                return WRONG_INPUT_ERROR;
            }
            *result = log2(a);
            break;
        case '#': // log10
            if (a <= 0) {
                return WRONG_INPUT_ERROR;
            }
            *result = log10(a);
            break;
    }
    return NO_ERROR;
}


// MARK: 계산
// 1. q deque해서 stack으로 push
// 2. 연산자 만나면 stack에서 pop 2번해서 계산기에 연산자와 함께 입력
// 3. 계산 끝나면 결과 stack에 push 후 다시 deque
ErrorCode cal(Queue** qf, Stack** st, double* resultPtr, char input[]) {
    ErrorCode tmp = NO_ERROR;
    int cntOperator = 0, cntOperand = 0;
    int* cntOperatorPtr = &cntOperator;
    int* cntOperandPtr = &cntOperand;
    
    long i, op;
    double a, b;
    int len;
        
    tmp = CheckBalance(input);
    if (tmp != NO_ERROR) {return tmp;} // 괄호쌍 예외처리
    tmp = CheckExpression(input);
    if (tmp != NO_ERROR) {return tmp;} // 잘못된 입력 예외처리
    
    tmp = In2Post(input, cntOperandPtr, cntOperatorPtr);
    if (tmp != NO_ERROR) {return tmp;}
    if (cntOperator == 0) {return WRONG_INPUT_ERROR;}
    
    len = QueLength(&front);
    
    for (i=0; i<len; i++) {
        op = (*qf)->data;
        // 연산자 -> stack pop 1번 -> calculator -> result push
        if (opFlag[i] == 1 && (op == '@' || op == '#')) {
            if (IsStackEmpty(st)) { return WRONG_INPUT_ERROR;}
            a = Top(st);
            Pop(st);
            tmp = Calculator(a, 0, op, resultPtr);
            if (tmp != NO_ERROR) {return tmp;}
            Deque(qf);
            Push(st, *resultPtr);
        }
        // 연산자 -> stack pop 2번 -> calculator -> result push
        else if (opFlag[i] == 1 && (op == '+' || op == '-' || op == '*' || op == '/' ||
            op == '%' || op == '^')) {
            if (IsStackEmpty(st)) { return WRONG_INPUT_ERROR;}
            b = Top(st);
            Pop(st);
            if (IsStackEmpty(st)) { return WRONG_INPUT_ERROR;}
            a = Top(st);
            Pop(st);
            tmp = Calculator(a, b, op, resultPtr); // 계산 완료
            if (tmp != NO_ERROR) {return tmp;} // 계산 예외차리
            // 계산완료한 연산자 deque하고 계산결과 push
            Deque(qf);
            Push(st, *resultPtr);
        } else { // 피연산자 push
            Push(st, (*qf)->data);
            Deque(qf);
        }
    }
    // stack에 result 외의 값이 남아있으면
    if((*st)->next != NULL) { return WRONG_INPUT_ERROR;}
    return NO_ERROR;
}
