#include <stdio.h>

typedef int valType;

int left(int idx) {
    return 2 * idx + 1;
}

int right(int idx) {
    return 2 * idx + 2;
}

void swap(valType *x, valType *y) {
    valType t;
    t = *x;
    *x = *y;
    *y = t;
}

void heapify(valType lst[], int idx, int max) {
    int l, r;
    l = left(idx);
    r = right(idx);

    if(r < max && lst[r] > lst[l] && lst[r] > lst[idx]) {
        swap(&lst[idx], &lst[r]);
        heapify(lst, r, max);
    } else if(l < max && lst[l] > lst[idx]) {
        swap(&lst[idx], &lst[l]);
        heapify(lst, l, max);
    }
}

void buildHeap(valType lst[], int n) {
    for(int i = n/2 - 1; i >= 0; i--) {
        heapify(lst, i, n);
    }
}

void heapsort(valType lst[], int n) {
    buildHeap(lst, n);
    for(int i = n -1; i > 0; i--) {
        swap(&lst[0], &lst[i]);
        heapify(lst, 0, i);
    }
}

int main(void) {
    valType lst[] = {-58, 8, 29, -87, 6, 65, 25, -58, 58, 1, -55, 53, -5, 8, -5, -56, 45, -13, -72, 88};
    /* valType lst[] = {56, -22, 98, -88, -69, -29, 94, -85, -12, -10, 42, -16, -92, -61, 66, 56, 58, -94, -70, 60}; */
    /* valType lst[] = {9, 8, 7, 6, 5, 4, 3, 2, 1}; */
    int i, n;
    n = sizeof(lst)/sizeof(lst[0]);

    for (i = 0; i < n; i++)
        printf("%d ", lst[i]);
    printf("\n");

    heapsort(lst, n);

    for (i = 0; i < n; i++)
        printf("%d ", lst[i]);
    printf("\n");

    return 0;
}
