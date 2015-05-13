#include <stdio.h>
#include <stdlib.h>

typedef struct treeNode {
    int data;
    int height;
    struct treeNode *left, *right;
} *tNode, tree;

void display_avl(tNode t);

int max(int x, int y) {
    return x > y ? x : y;
}

int get_height(tNode t) {
    return t ? t->height : -1;
}

int get_child_max_height(tNode t) {
    return t ? max(get_height(t->left), get_height(t->right)) : -1;
}

int abs(int x) {
    return x > 0 ? x : -x;
}

int child_differ_height(tNode t) {
    tNode l, r;
    l = t->left;
    r = r->right;
    return abs(get_height(l) - get_height(r));
}

tNode find_min(tNode t) {
    if( ! t) {
        return NULL;
    } else if( ! t->left) {
        return t;
    } else {
        return find_min(t->left);
    }
}

tNode find_max(tNode t) {
    if( ! t) {
        return NULL;
    } else if( ! t->right) {
        return t;
    } else {
        return find_max(t->right);
    }
}

tNode find(tNode t, int e) {
    if( ! t) {
        return NULL;
    } else if(t->data == e) {
        return t;
    } else if(e < t->data) {
        return find(t->left, e);
    } else {
        return find(t->right, e);
    }
}

tNode find_parent(tNode t, int e) {
    if( ! t) return NULL;
    if(e < t->data) {
        if(t->left->data == e) {
            return t;
        } else {
            return find_parent(t->left, e);
        }
    } else if(e > t->data) {
        if(t->right->data == e) {
            return t;
        } else {
            return find_parent(t->right, e);
        }
    } else {
        return NULL;
    }
}

tNode LL(tNode t) {
    if( ! t) exit(1);

    tNode root;

    root = t->left;
    t->left = root->right;
    root->right = t;

    t->height = get_child_max_height(t) + 1;
    root->height = get_child_max_height(root) + 1;
    return root;
}

tNode RR(tNode t) {
    if( ! t) exit(1);

    tNode root;

    root = t->right;
    t->right = root->left;
    root->left = t;

    t->height = get_child_max_height(t) + 1;
    root->height = get_child_max_height(root) + 1;
    return root;
}

tNode LR(tNode t) {
    if( ! t) exit(1);

    t->left = RR(t->left);
    return LL(t);
}

tNode RL(tNode t) {
    if( ! t) exit(1);

    t->right = LL(t->right);
    return RR(t);
}

tNode insert(tNode t, int e) {
    if( ! t) {
        tNode leaf;
        leaf = (tNode) malloc (sizeof(tree));
        leaf->data = e;
        leaf->left = leaf->right = NULL;
        leaf->height = 0;
        return leaf;
    } else if(e < t->data) {
        t->left = insert(t->left, e);
        if(get_height(t->left) - get_height(t->right) == 2) {
            if(e < t->left->data) {
                t = LL(t);
            } else {
                t = LR(t);
            }
        }
    } else if(e > t->data) {
        t->right = insert(t->right, e);
        if(get_height(t->right) - get_height(t->left) == 2) {
            if(e > t->right->data) {
                t = RR(t);
            } else {
                t = RL(t);
            }
        }
    }
    t->height = get_child_max_height(t) + 1;
    return t;
}

tNode balance(tNode root, tNode anchor) {
    while(anchor) {
        tNode head = find_parent(root, anchor->data);
        int anchor_in_left = head ? anchor == head->left : -1;
        if(get_height(anchor->right) - get_height(anchor->left) == 2) {
            anchor = RR(anchor);
        } else if(get_height(anchor->left) - get_height(anchor->right) == 2) {
            anchor = LL(anchor);
        }
        if(head) {
            if(anchor_in_left) {
                head->left = anchor;
            } else {
                head->right = anchor;
            }
        } else {
            root = anchor;
        }
        anchor->height = get_child_max_height(anchor) + 1;
        anchor = find_parent(root, anchor->data);
    }
    return root;
}

tNode delete(tNode root, int e) {
    if( ! root) return NULL;

    tNode target, head;
    int target_in_left;

    target = find(root, e);

    if(target) {
        head = find_parent(root, e);
        target_in_left = head ? target == head->left : 0;
        if(target->left || target->right) {
            /* target has childs  */
            if(target->left) {
                /* target has left child */
                tNode r_node, r_head;
                r_node = find_max(target->left);  // left child's max node, for replacing the target
                r_head = find_parent(root, r_node->data);

                r_head->right = r_node->left;

                // replace target with r_node
                r_node->left = target->left;
                r_node->right = target->right;

                if( ! head) {
                    root = r_node;
                    // target is root
                } else {
                    if(target_in_left) {
                        head->left = r_node;
                    } else {
                        head->right = r_node;
                    }
                }

                // balance from r_head to root
                root = balance(root, r_head);
                free(target);
            } else {
                /* target has only right child */
                if( ! head) {
                    // target is root
                    root = target->right;
                } else {
                    if(target_in_left) {
                        head->left = target->right;
                    } else {
                        head->right = target->right;
                    }
                }

                root = balance(root, head);
                free(target);
            }
        } else {
            /* target is a leaf(has no child) */
            if( ! head) {
                // target is root
                root = NULL;
            } else {
                if(target_in_left) {
                    head->left = NULL;
                } else {
                    head->right = NULL;
                }
            }
            root = balance(root, head);
            free(target);
        }
    }
    return root;
}

void destroy(tNode t){
    if(t) {
        destroy(t->left);
        destroy(t->right);
        free(t);
    }
}

void display_avl(tNode t) {
    tNode avl[1000];
    avl[0] = t;
    int i = 0, sum = 1, max_elem = 1, cur_elem = 1;
    int height = t->height;
    while(height >= 0 && i < sum) {
        if( ! avl[i]) {
            avl[sum++] = NULL;
            avl[sum++] = NULL;
            if((i+1) % 2 == 0) {
                printf("[NULL ");
            } else {
                printf("NULL] ");
            }
        } else {
            avl[sum++] = avl[i]->left;
            avl[sum++] = avl[i]->right;
            if((i+1) % 2 == 0) {
                printf("[%d(%d) ", avl[i]->data, avl[i]->height);
            } else {
                if(i == 0) {
                    printf("[%d(%d)] ", avl[i]->data, avl[i]->height);
                } else {
                    printf("%d(%d)] ", avl[i]->data, avl[i]->height);
                }
            }
        }
        i++;

        if(cur_elem == max_elem) {
            printf("\n");
            max_elem *= 2;
            cur_elem = 1;
            height--;
        } else {
            cur_elem++;
        }
    }
}

int main(void) {
    tNode t = NULL;
    int i, j;
    const int max = 10;

    for(i = 0, j = 0; i < max; i++, j = (j + 7) % max) {
        t = insert(t, j);
        printf("%d ",j);
    }
    printf("\n");

    printf("--------------\n");
    printf("Init avl: \n");
    display_avl(t);
    t = delete(t, 4);
    printf("--------------\n");
    printf("Delete 4: \n");
    display_avl(t);
    t = delete(t, 2);
    printf("--------------\n");
    printf("Delete 2: \n");
    display_avl(t);
    t = delete(t, 0);
    printf("--------------\n");
    printf("Delete 0: \n");
    display_avl(t);
    t = delete(t, 0);

    printf("--------------\n");
    printf("Delete 0: \n");
    display_avl(t);

    destroy(t);
    return 0;
}
