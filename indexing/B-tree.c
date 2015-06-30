#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "b-tree.h"

void print_root(b_tree * root) {
    printf("root's key nums = %d\n", (*root)->key_num);
    printf("root's child nums = %d\n", (*root)->child_num);
    printf("root's keys: ");
    for(int i = 0; i < (*root)->key_num; i++) {
        printf("%d ", (*root)->key[i]);
    }
    b_tree child;
    printf("\n");
    for(int j = 0; j < (*root)->child_num; j++) {
        child = (*root)->child[j];
        printf("root's %dth child's key nums = %d\n", j, child->key_num);
        printf("keys: ");
        for(int i = 0; i < child->key_num; i++) {
            printf("%d  ", child->key[i]);
        }
        printf("\n");
    }
    for(int j = 0; j < (*root)->child_num; j++) {
        printf("----------\n");
        if((*root)->child[j]->child_num > 0) {
            print_root(&(*root)->child[j]);
        }
    }
}

void print(b_tree * t_node) {
    int key_num, child_num;
    key_num = (*t_node)->key_num;
    child_num = (*t_node)->child_num;
    for(int i = 0; i < key_num; i++) {
        if(i < child_num) {
            print(&(*t_node)->child[i]);
        }
        printf("%d ", (*t_node)->key[i]);
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
        /* print_root(&root); */
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
            print_root(&root);
            return;
        }
    }
}

int main(void) {
    test_delete();
    /* test(); */
    return 0;
}
