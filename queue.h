#ifndef queue_h
#define queue_h

struct queue {
    long data;
    struct queue* next;
}; typedef struct queue Queue;
extern Queue* rear;
extern Queue* front;



Queue* GetNewQue(long x);
int IsQueueEmpty(Queue* q);
void Enque(Queue** front, long input);
void Deque(Queue** front);
void Print(Queue* queue);
void MakeQueEmpty(Queue** front);
int QueLength(Queue** front);

#endif /* queue_h */
