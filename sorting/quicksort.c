#include<stdio.h>

typedef int ValType;

void swap(ValType *i, ValType *j) {
    ValType t = *i;
    *i = *j;
    *j = t;
}

void quickSort(ValType *a, int n) {
    int left, right;
    ValType pivot;
    left = 0;
    right = n - 1;
    if(n < 2) return;
    pivot = a[n/2];
    while(left < right) {
        while(a[left] < pivot) left++;
        while(pivot < a[right]) right--;
        if(left >= right) break;
        swap(&a[left++], &a[right--]);
    }
    quickSort(a, left);
    quickSort(a + left, n - left);
}

int main(void) {
    ValType lst[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int n = sizeof(lst) / sizeof(lst[0]);
    int i;
    for(i = 0; i < n; i++)
        printf("%d ", lst[i]);
    printf("\n");

    quickSort(lst, n);

    for(i = 0; i < n; i++)
        printf("%d ", lst[i]);
    printf("\n");

    return 0;
}
