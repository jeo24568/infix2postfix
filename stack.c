#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Stack* st = NULL;

Stack* GetNewNode(double x) {
    Stack* tmp = (Stack*)malloc(sizeof(Stack));
    tmp->data = x;
    tmp->next = NULL;
    
    return tmp;
}
void Push(Stack** top, double input) {
    Stack* tmp = GetNewNode(input);
    if (*top != NULL) {
        tmp->next = *top;
        *top = tmp;
        return;
    }
    *top = tmp;
    
}
void Pop(Stack** st) {
    if (*st != NULL) {
        Stack* tmp = *st;
        *st = (*st)->next;
        free(tmp);
    } else {return;}
}
double Top(Stack** st) {
    return (*st)->data;
}
int IsStackEmpty(Stack** st) {
    return *st == NULL;
}
void MakeStackEmpty(Stack** st) {
    while (!IsStackEmpty(st)) {
        Pop(st);
    }
}
