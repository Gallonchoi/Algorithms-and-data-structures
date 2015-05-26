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

    bool is_leaf;
} b_tree_t, *b_tree;

bool is_full(b_tree t_node) {
    return (t_node->key_num == degree * 2 - 2) ? true : false;
}

void create(b_tree * t_node) {
    *t_node = (b_tree) malloc (sizeof(b_tree_t));
    (*t_node)->is_leaf = true;
    (*t_node)->child_num = 0;
    (*t_node)->key_num = 0;
}

void split(b_tree * t_node, b_tree * parent, int child_idx) {
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

    // TODO link the new two nodes
    if(parent) {
        return;
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
        if(is_full(*root) == true) {
            split(root, NULL, 0);
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
    } else if(t_node->is_leaf == true || t_node->child[i] == NULL) {
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

    return 0;
}
