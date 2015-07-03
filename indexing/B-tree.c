#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "b-tree.h"

void print(b_tree * t_node) {
    int key_num, child_num;
    key_num = (*t_node)->key_num;
    child_num = (*t_node)->child_num;
    for(int i = 0; i < key_num; i++) {
        if(i < child_num) {
            print(&(*t_node)->child[i]);
        }
        printf("%02d ", (*t_node)->key[i]);
    }
    if(key_num < child_num) {
        print(&(*t_node)->child[child_num - 1]);
    }
}

void test_delete() {
    b_tree root;
    create(&root);
    int range = 50;
    for(int i = range; i >= 1; i--) {
        insert(&root, i);
    }
    for(int i = range; i >= 1; i--) {
        delete(&root, i);
        printf("[%02d]: ", i);
        print(&root);
        printf("\n");
    }
    int begin, end;
    begin = 1;
    end = 50;
    for(int i = begin; i <= end; i++) {
        insert(&root, i);
        printf("[%02d]: ", i);
        print(&root);
        printf("\n");
    }
}

void test() {
    b_tree root;
    create(&root);

    int ran[100], range;
    range = 100;

    srand((int)time(NULL));
    for(int i = 0; i < range; ++i) {
        ran[i] = rand() % 100 + 1;
        insert(&root, ran[i]);
        printf("[%d]%d ", i, ran[i]);
    }

    for(int i = 0; i < range; ++i) {
        result r = search(root, ran[i]);
        if(r.is_found == false) {
            printf("\nError!\n");
            printf("[%d]%d\n", i, ran[i]);
            return;
        }
    }
}

void test2() {
    b_tree root;
    create(&root);

    for(int i = 0; i <= 50; i ++) {
        insert(&root, i);
    }

    create(&root);
    for(int j = 50; j >= 0; j --) {
        insert(&root, j);
    }

    for(int i = 0; i <= 50; ++i) {
        if(i == 24) {
            printf("test\n");
        }
        result r = search(root, i);
        if(r.is_found == false) {
            printf("\nError!\n");
            printf("[%d]\n", i);
            return;
        } else {
            printf("GOOD: %d\n", i);
        }
    }
}

int main(void) {
    test_delete();
    /* test(); */
    /* test2(); */
    return 0;
}
