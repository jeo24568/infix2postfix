#include <stdio.h>
#include <string.h>

#include "calc.h"

void init(void) {
    int i;
    MakeQueEmpty(&front);
    MakeStackEmpty(&st);
    // 연산자 판별 배열 초기화
    for (i=0; i<QueLength(&front); i++) {
        isOperator[i] = 0;
    }
    cntQ = 0;
}

// MARK: main
int main(void) {
    ErrorCode tmp = NO_ERROR;
    char input[110] = "";
    char end[10] = "[enter]";
    
    printf("./calculator\n");
    while(1) {
        double result = 0;
        int inputLen = 0;
        double* resultPtr = &result;
        
        printf(">> ");
        fgets(input, sizeof(input), stdin);
        
        input[strcspn(input, "\n")] = '\0';
        
        inputLen = (int)strlen(input);

        if ((int)strlen(input) > 100) {
            printf("Too long expression\n");
            fflush(stdin);
            continue;
        }
        // 종료
        if (strcmp(end, input) == 0) {
            printf("종료됨\n");
            return 0;
        }
        tmp = cal(&front, &st, resultPtr, input);
        switch (tmp) {
            case NO_ERROR:
                printf("result: %.2f\n", result);
                break;
            case MODULO_BY_ZERO_ERROR:
                printf("Modulo by zero\n");
                break;
            case NOT_BALANCE_ERROR:
                printf("Bracket imbalance\n");
                break;
            case WRONG_INPUT_ERROR:
                printf("Wrong input\n");
                break;
            case OUT_OF_RANGE:
                printf("Out of range\n");
                break;
            case DIVISION_BY_ZERO_ERROR:
                printf("Division by zero\n");
                break;
        }
        init();

    }
    return 0;
}
