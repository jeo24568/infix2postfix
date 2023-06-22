#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


Queue* GetNewQue(long x) {
    Queue* tmp = (Queue*)malloc(sizeof(Queue));
    tmp->data = x;
    tmp->next = NULL;
    return tmp;
}

Queue* rear = NULL;
Queue* front = NULL;

int IsQueueEmpty(Queue* q) {
    return q == NULL;
}
void Enque(Queue** front, long input) {
    Queue* tmp = GetNewQue(input);
    if (IsQueueEmpty(*front)) {
        *front = tmp;
        rear = tmp;
        return;
    }
    rear->next = tmp;
    rear = tmp;
}
void Deque(Queue** front){
    Queue* q = *front;
    if (IsQueueEmpty(*front)) {return ;}
    if (*front == rear) {
        *front = NULL;
        rear = NULL;
    } else {
        *front = (*front)->next;
    }
    free(q);
}
void Print(Queue* queue) {
    Queue* tmp = queue;
    if (IsQueueEmpty(queue)) {return;}
    printf("print queue: ");
    do {
        printf("%ld ", tmp->data);
        tmp = tmp->next;
    }
    while (tmp != NULL);
    printf("\n");
}
void MakeQueEmpty(Queue** front) {
    while (!IsQueueEmpty(*front)) {
        Deque(front);
    }
}
int QueLength(Queue** front) {
    int cnt = 0;
    Queue* tmp = *front;
    while (tmp != rear) {
        cnt++;
        tmp = tmp->next;
    }
    return cnt+1;
}
