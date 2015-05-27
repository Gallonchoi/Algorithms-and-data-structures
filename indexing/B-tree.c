#include <stdio.h>
#include <stdlib.h>

#define degree 2
#define middle (degree - 1)

typedef enum {true, false} bool;

typedef struct b_tree {
    int key_num;
    int key[degree * 2 - 1];

    int child_num;
    struct b_tree * child[degree * 2];
} b_tree_t, *b_tree;

bool is_full(b_tree t_node) {
    return (t_node->key_num == degree * 2 - 1) ? true : false;
}

void create(b_tree * t_node) {
    *t_node = (b_tree) malloc (sizeof(b_tree_t));
    (*t_node)->child_num = 0;
    (*t_node)->key_num = 0;
}

void split(b_tree * t_node, b_tree * parent, int idx) {
    // get the middle key
    int middle_key = (*t_node)->key[middle];
    // split current node to two nodes
    b_tree before, after;
    // create null node
    create(&before);
    create(&after);
    before->key_num = middle;
    after->key_num = middle;
    int i;
    for(i = 0; i <= middle; i++) {
        // transfer childs
        before->child[i] = (*t_node)->child[i];
        after->child[i] = (*t_node)->child[middle+i+1];
        // transfer keys
        if(i < middle) {
            before->key[i] = (*t_node)->key[i];
            after->key[i] = (*t_node)->key[middle+i+1];
        }
    }

    // link the new two nodes
    if(parent) {
        printf("idx = %d\n", idx);
        // move old keys and childs
        if(idx < (*parent)->key_num) {
            for(i = (*parent)->key_num - 1; i >= idx; i--) {
                (*parent)->key[i+1] = (*parent)->key[i];
                (*parent)->child[i+2] = (*parent)->child[i+1];
            }
        }
        // insert new key
        (*parent)->key[idx] = middle_key;
        (*parent)->child[idx] = before;
        (*parent)->child[idx + 1] = after;
        (*parent)->child_num ++;
        (*parent)->key_num ++;
    } else {
        /* t_node is root */
        /* create new root */
        b_tree new_root;
        create(&new_root);
        new_root->key_num = 1;
        new_root->key[0] = middle_key;
        new_root->child_num = 2;
        new_root->child[0] = before;
        new_root->child[1] = after;
        /* destroy old root */
        *t_node = new_root;
    }
}

void insert(b_tree * root, int key) {
    if( ! *root) {
        exit(1);
    } else {
        b_tree * t_node, * parent;
        int i, j, idx;
        t_node = root;
        parent = NULL;
        while(1) {
            // split if the current node is full
            if(is_full(*t_node) == true) {
                if( ! parent) {
                    // root
                    split(t_node, NULL, 0);
                } else {
                    split(t_node, parent, idx);
                }
            }
            // find the position to insert
            i = 0;
            while(i < (*t_node)->key_num && key > (*t_node)->key[i]) {
                i ++;
            }
            // insert to its child if it exists
            if(i < (*t_node)->child_num) {
                idx = i;
                parent = t_node;
                t_node = &(*t_node)->child[i];
            } else {
                // insert to current position

                // move keys which after i
                if(i < (*t_node)->key_num) {
                    for(j = (*t_node)->key_num; j >= i; j --) {
                        (*t_node)->key[j+1] = (*t_node)->key[j];
                    }
                }

                (*t_node)->key[i] = key;
                (*t_node)->key_num ++;
                break;
            }
        }
    }
}

bool search(b_tree t_node, int key) {
    int i = 0;
    while(i < t_node->key_num && key > t_node->key[i]) {
        i ++;
    }
    if(i < t_node->key_num && key == t_node->key[i]) {
        return true;
    } else if(t_node->child_num == 0 || t_node->child[i] == NULL) {
        return false;
    } else {
        return search(t_node->child[i], key);
    }
}

void delete(b_tree * root, int key) {
}

int main(void) {
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
        if(search(root, i) == true) {
            printf("FOUND %d\n", i);
        } else {
            printf("NOT FOUND %d\n", i);
        }
    }
    return 0;
}
