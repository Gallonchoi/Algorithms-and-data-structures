#include<stdio.h>
#include<stdbool.h>

bool binary_search(int lst[], int left, int right, int elem) {
    if(left > right) {
        return false;
    }
    int mid = (left + right) / 2;
    if(lst[mid] == elem) {
        return true;
    } else if(lst[mid] < elem) {
        return binary_search(lst, mid+1, right, elem);
    } else {
        return binary_search(lst, left, mid-1, elem);
    }
}

bool test() {
    int lst[100];
    for(int i = 0; i < 100; i++) {
        lst[i] = i;
    }
    for(int i = 0; i < 100; i++) {
        if(binary_search(lst, 0, 100, i) == false) {
            return false;
        }
    }
    for(int i = -10; i < 0; i++) {
        if(binary_search(lst, 0, 100, i) == true) {
            return false;
        }
    }
    for(int i = 100; i < 110; i++) {
        if(binary_search(lst, 0, 100, i) == true) {
            return false;
        }
    }
    return true;
}

int main(void) {
    if(test() == true) {
        printf("Correct!\n");
    } else {
        printf("Incorrect!\n");
    }
    return 0;
}
