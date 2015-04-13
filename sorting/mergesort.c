#include <stdio.h>

void merge(int lst[], int n, int m) {
    int i, j, k;
    int temp[n];

    for(i = 0, j = m, k = 0; k < n; k++) {
        if(j != n && (i == m || lst[j] < lst[i])) {
            temp[k] = lst[j++];
        } else {
            temp[k] = lst[i++];
        }
    }

    for(i = 0; i < n; i++) {
        lst[i] = temp[i];
    }
}

void merge_sort(int lst[], int n) {
    if(n < 2) return;
    int m = n / 2;
    merge_sort(lst, m);
    merge_sort(lst + m, n - m);
    merge(lst, n, m);
}

int main(void) {
    int lst[] = {5, 4, 3, -4, 10, 2, 5, -32, 5, 9, 8, 7, 123};
    int n = sizeof(lst) / sizeof(lst[0]);
    int i;

    for (i = 0; i < n; i++)
        printf("%d ", lst[i]);
    printf("\n");

    merge_sort(lst, n);

    for (i = 0; i < n; i++)
        printf("%d ", lst[i]);
    printf("\n");

    return 0;
}
