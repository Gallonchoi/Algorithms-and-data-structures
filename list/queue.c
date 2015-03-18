#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define QUEUE_MAX_SIZE 100

typedef int ElemType;

typedef struct {
    ElemType * base;
    int front;
    int rear;
    int max_size;
    int length;
} queue;

bool init(queue *q) {
    q->base = (ElemType *) malloc(QUEUE_MAX_SIZE * sizeof(ElemType));
    if( ! q->base) return false;
    q->front = 0;
    q->rear = 0;
    q->max_size = QUEUE_MAX_SIZE;
    q->length = 0;
    return true;
}

bool in(queue *q, ElemType e) {
    if(q->length < q->max_size) {
        if(q->rear == q->max_size) {
            q->rear = 0;
        }
        *(q->base + q->rear++) = e;
        q->length ++;
        return true;
    } else {
        return false;
    }
}

bool out(queue *q, ElemType *e) {
    if(q->length == 0) {
        return false;
    } else {
        if(q->front == q->max_size) {
            q->front = 0;
        }
        *e = *(q->base + q->front++);
        q->length --;
        return true;
    }
}

int main(void) {
    queue q;
    init(&q);
    printf("In: ");
    for(int i = 0; i < 100; i++) {
        if(in(&q, i) == true) {
            printf(" %d", i);
        }
    }
    printf("\nOut: ");
    for(int i = 0; i < 100; i++) {
        ElemType e;
        out(&q, &e);
        printf(" %d", e);
    }
    printf("\n");
    return 0;
}
