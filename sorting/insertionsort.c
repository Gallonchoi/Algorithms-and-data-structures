#include <stdio.h>

void insertion_sort(int lst[], int n) {
    int i, j, temp;
    for(i = 1; i < n; i++) {
        t = lst[i];
        for(j = i - 1; j >= 0 && t < lst[j]; j--) {
            lst[j + 1] = lst[j];
        }
        lst[j + 1] = t;
    }
}

int main(void) {
    int lst[] = {9, -7, 8, 7, 6, -2, 10, 0, 0, -431};
    int n = sizeof(lst) / sizeof(lst[0]);
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", lst[i]);
    printf("\n");

    insertion_sort(lst, n);

    for (i = 0; i < n; i++)
        printf("%d ", lst[i]);
    printf("\n");

    return 0;
}
