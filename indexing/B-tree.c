#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "b-tree.h"

void test_insert() {
    b_tree root;
    create(&root);
    insert(&root, 1);
    /* insert(&root, 2); */
    insert(&root, 3);
    /* insert(&root, 4); */
    insert(&root, 5);
    insert(&root, 7);
    insert(&root, 6);
    insert(&root, 4);
    printf("key num = %d\n", root->key_num);
    for(int i = 0; i < root->key_num; i++) {
        printf("key[%d] = %d\n", i, root->key[i]);
    }
    printf("child num = %d\n", root->child_num);
    printf("child[0]->key_num = %d\n", root->child[0]->key_num);
    printf("child[0]->key[0] = %d\n", root->child[0]->key[0]);
    printf("child[1]->key_num = %d\n", root->child[1]->key_num);
    printf("child[1]->key[0] = %d\n", root->child[1]->key[0]);
    printf("child[1]->key[1] = %d\n", root->child[1]->key[1]);
    printf("child[2]->key_num = %d\n", root->child[2]->key_num);
    printf("child[2]->key[0] = %d\n", root->child[2]->key[0]);
    for(int i = 0; i < 10; i++) {
        result r = search(root, i);
        if(r.is_found == true) {
            printf("FOUND %d == %d\n", i, (*r.target_node)->key[r.idx]);
        } else {
            printf("NOT FOUND %d\n", i);
        }
    }
}

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
        printf("[%d]: ", i);
        print(&root);
        printf("\n");
    }
    create(&root);
    for(int i = range; i >= 1; i--) {
        insert(&root, i);
    }
    for(int i = 1; i <= range; i++) {
        delete(&root, i);
        printf("[%d]: ", i);
        print(&root);
        printf("\n");
    }
}

void test() {
    b_tree root;
    create(&root);

    int ran[100], range;
    range = 100;
    /* for(int i = 1; i <= range; i++) { */
    /*     result r = search(root, i); */
    /*     if(r.is_found == false) { */
    /*         printf("\nError!\n%d\n", i); */
    /*         print_root(&root); */
    /*         return; */
    /*     } */
    /* } */



    srand((int)time(NULL));
    for(int i = 0; i < range; ++i) {
        ran[i] = rand() % 100 + 1;
        insert(&root, ran[i]);
        printf("[%d]%d ", i, ran[i]);
        /* result r = search(root, ran[0]); */
        /* if(r.is_found == false) { */
        /*     printf("\nError!\n[%d]%d\n", i, ran[i]); */
        /*     print_root(&root); */
        /*     return; */
        /* } */
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
    return 0;
}
