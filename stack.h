#ifndef stack_h
#define stack_h

struct stack {
    double data;
    struct stack* next;
    struct stack* prev;
};typedef struct stack Stack;

extern Stack* st;

Stack* GetNewNode(double x);
void Push(Stack** top, double input);
void Pop(Stack** st);
double Top(Stack** st);
int IsStackEmpty(Stack** st);
void MakeStackEmpty(Stack** st);

#endif /* stack_h */
