#ifndef B_TREE_H_
#define B_TREE_H_

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

typedef struct search_result {
    bool is_found;
    b_tree * target_node, * parent_node;
    int idx, p_idx;
} result;

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
    before->child_num = after->child_num = (*t_node)->child_num/2;
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

void combile(b_tree * parent, int idx) {
    b_tree l_node, r_node;
    int key, new_key_num, new_child_num;
    key = (*parent)->key[idx];
    l_node = (*parent)->child[idx];
    r_node = (*parent)->child[idx + 1];

    new_key_num = l_node->key_num + r_node->key_num + 1;
    new_child_num = l_node->child_num + r_node->child_num;
    if(new_key_num >= degree * 2) {
        exit(1);
    }

    for(int i = idx+1; i < (*parent)->key_num; i++) {
        (*parent)->key[i - 1] = (*parent)->key[i];
        (*parent)->child[i] = (*parent)->child[i + 1];
    }
    (*parent)->key_num --;
    (*parent)->child_num --;

    l_node->key[l_node->key_num ++] = key;

    for(int i = l_node->key_num, j = 0; i < new_key_num; i++, j++) {
        l_node->key[i] = r_node->key[j];
        l_node->child[i] = r_node->child[j];
    }
    l_node->child[new_key_num] = r_node->child[r_node->child_num - 1];
    l_node->key_num = new_key_num;
    l_node->child_num = new_child_num;

    if((*parent)->key_num == 0) {
        // parent is root
        (*parent)->key_num = l_node->key_num;
        (*parent)->child_num = l_node->child_num;
        for(int i = 0; i < l_node->key_num; i++) {
            (*parent)->key[i] = l_node->key[i];
            (*parent)->child[i] = l_node->child[i];
        }
        (*parent)->child[l_node->child_num-1] = l_node->child[l_node->child_num-1];
    }
}

void print_root(b_tree * root);

void insert(b_tree * root, int key) {
    if( ! *root) {
        exit(1);
    } else {
        b_tree * t_node, * parent;
        int idx;
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
                    t_node = parent;
                }
            }
            // find the position to insert
            int i = 0;
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
                    for(int j = (*t_node)->key_num; j >= i; j --) {
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

result search_process(b_tree t_node, b_tree p_node, int p_idx, int key) {
    int i = 0;
    while(i < t_node->key_num && key > t_node->key[i]) {
        i ++;
    }
    if(i < t_node->key_num && key == t_node->key[i]) {
        result r;
        r.is_found = true;
        r.target_node = &t_node;
        r.idx = i;
        if(p_node) {
            r.parent_node = &p_node;
            r.p_idx = p_idx;
        } else {
            r.parent_node = NULL;
        }
        return r;
    } else if(t_node->child_num == 0 || t_node->child[i] == NULL) {
        result r;
        r.is_found = false;
        return r;
    } else {
        return search_process(t_node->child[i], t_node, i, key);
    }
}

result search(b_tree t_node, int key) {
    return search_process(t_node, NULL, 0, key);
}

void delete(b_tree * t_node, int key);

void delete_process(b_tree * t_node, int idx, b_tree * parent, int p_idx) {
    if((*t_node)->child_num == 0) {
        /* target node is a leaf */
        if((*t_node)->key_num < degree && parent) {
            // parent has at least degree children and degree-1 keys
            b_tree bro;
            int b_idx;
            bro = NULL;
            if(p_idx > 0) {
                bro = (*parent)->child[p_idx - 1];
                b_idx = bro->key_num - 1;
            }
            if(p_idx < (*parent)->child_num-1) {
                bro = (*parent)->child[p_idx + 1];
                b_idx = 0;
            }

            if(bro) {
                if(bro->key_num < degree) {
                    // combine
                } else {
                    // move bro's key
                    int key;
                    key = bro->key[b_idx];
                    if(b_idx == 0) {
                        (*t_node)->key[(*t_node)->key_num++] = (*parent)->key[p_idx];
                        (*parent)->key[p_idx] = key;

                        for(int i = 0; i < bro->key_num - 1; i++) {
                            bro->key[i] = bro->key[i + 1];
                        }
                    } else {
                        for(int i = (*t_node)->key_num; i > 0; i--) {
                            (*t_node)->key[i] = (*t_node)->key[i - 1];
                        }
                        (*t_node)->key[0] = (*parent)->key[p_idx - 1];
                        (*parent)->key[p_idx - 1] = key;
                    }
                    bro->key_num --;
                }
            }
        }

        for(int i = idx+1; i < (*t_node)->key_num; i++) {
            (*t_node)->key[i-1] = (*t_node)->key[i];
        }
        (*t_node)->key_num --;
    } else {
        /* target node is a internal node */
        b_tree left_t, right_t;
        left_t = (*t_node)->child[idx];
        right_t = (*t_node)->child[idx+1];
        if(left_t->key_num >= degree || right_t->key_num >= degree) {
            b_tree * r_node;
            int r_idx, t_idx;
            if(left_t->key_num >= degree) {
                r_node = &left_t;
                r_idx = left_t->key_num - 1;  // The last one key
                t_idx = idx;
            } else {
                r_node = &right_t;
                r_idx = 0;  // The first one key
                t_idx = idx+1;
            }

            (*t_node)->key[idx] = (*r_node)->key[r_idx];
            delete_process(r_node, r_idx, t_node, t_idx);
        } else {
            // combine two childs
            int key = (*t_node)->key[idx];
            combile(t_node, idx);
            delete(t_node, key);
        }
    }
}

void delete(b_tree * t_node, int key) {
    b_tree target;
    result r;
    int idx;
    r = search(*t_node, key);
    if(r.is_found == true) {
        target = *(r.target_node);
        idx = r.idx;
        delete_process(&target, idx, r.parent_node, r.p_idx);
    }
}
#endif
