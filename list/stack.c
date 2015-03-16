#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define STACK_INIT_SIZE 100
#define STACK_INCREMENT_SIZE 100
typedef int ElemType;

typedef struct {
    ElemType * base;
    ElemType * top;
    int max_size;
    int increment_size;
} stack;

bool init(stack *s) {
    s->base = (ElemType *) malloc(STACK_INIT_SIZE * sizeof(ElemType));
    if( ! s->base) return false;
    s->top = s->base;
    s->max_size = STACK_INIT_SIZE;
    s->increment_size = STACK_INCREMENT_SIZE;
    return true;
}

bool get_top(stack *s, ElemType *e) {
    if(s->top == s->base) return false;
    *e = *(s->top - 1);
    return true;
}

bool pop(stack *s, ElemType *e) {
    if(s->top == s->base) return false;
    *e = *(--s->top);
    return true;
}

bool push(stack *s, ElemType e) {
    if(s->top - s->base >= s->max_size) {
        s->base = (ElemType *) realloc(s->base, (s->max_size + s->increment_size) * sizeof(ElemType));
        if( ! s->base) return false;
        s->top = s->base + s->max_size;
        s->max_size += s->increment_size;
        printf("stack size = %d\n", s->max_size);
    }
    *(s->top++) = e;
    return true;
}

void clear(stack *s) {
    s->base = s->top;
}

void destroy(stack *s) {
    free(s->base);
    s->base = NULL;
}

int size(stack *s) {
    return s->top - s->base;
}

int main(void) {
    stack st;
    ElemType e;

    if(init(&st) == true) {
        // init stack
        printf("Init correct!\n");
        printf("s.max_size = %d\n", st.max_size);
    } else {
        printf("Init incorrect!\n");
        return 0;
    }

    if(push(&st, 1) == true) {
        // push an element in stack
        printf("Push correct!\n");
    } else {
        printf("Push incorrect!\n");
        return 0;
    }

    if(get_top(&st, &e) == true) {
        // get the top element from stack
        printf("Get top correct!\n");
        printf("Top = %d\n", e);
    } else {
        printf("Get top incorrect!\n");
        return 0;
    }

    // push and pop
    for(int i = 1; i <= 56; i++) {
        if(push(&st, i) == false) {
            printf("Push incorrect! i = %d\n", i);
            return 0;
        }
        if(i < 40) {
            clear(&st);
        }
    }
    printf("size = %d\n", size(&st));
    for(int i = 1; i <= 20; i++) {
        if(pop(&st, &e) == false) {
            printf("\nPop incorrect! i = %d\n", i);
            return 0;
        } else {
            printf("%d ", e);
        }
    }
    printf("\n");
    destroy(&st);

    return 0;
}
