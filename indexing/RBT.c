/* Red-Black tree */

#include <stdio.h>
#include <stdlib.h>

typedef enum {BLACK, RED} Color;

typedef struct RBT {
    int data;
    Color color;
    struct RBT *left, *right, *parent;
} *tNode, tree;

void display(tNode t);

tNode find_max(tNode t) {
    if( ! t) {
        return NULL;
    } else if(t->right) {
        return find_max(t->right);
    } else {
        return t;
    }
}

tNode find_min(tNode t) {
    if( ! t) {
        return NULL;
    } else if(t->left) {
        return find_min(t->left);
    } else {
        return t;
    }
}

tNode find(tNode t, int e) {
    if( ! t) {
        return NULL;
    } else if(e < t->data) {
        return find(t->left, e);
    } else if(e > t->data) {
        return find(t->right, e);
    } else {
        return t;
    }
}

tNode find_root(tNode t) {
    while(t->parent) {
        t = t->parent;
    }
    return t;
}

tNode find_grandparent(tNode n) {
    if (n && n->parent) {
        return n->parent->parent;
    } else {
        return NULL;
    }
}

tNode find_uncle(tNode n) {
    tNode g = find_grandparent(n);
    if ( ! g) {
        return NULL; // No grandparent means no uncle
    }
    if (n->parent == g->left) {
        return g->right;
    } else {
        return g->left;
    }
}

tNode find_brother(tNode n) {
    tNode p;
    p = n->parent;
    if( ! p) {
        return NULL;
    } else {
        if(p->left == n) {
            return p->right;
        } else {
            return p->left;
        }
    }
}

void rotate_left(tNode *p) {
    tNode g = (*p)->parent;
    tNode n = (*p)->right;

    if(g) {
        if(g->left == *p) {
            g->left = n;
        } else {
            g->right = n;
        }
    }
    n->parent = g;

    (*p)->right = n->left;
    if((*p)->right) {
        (*p)->right->parent = (*p);
    }

    n->left = (*p);
    (*p)->parent = n;
}

void rotate_right(tNode *p) {
    tNode g = (*p)->parent;
    tNode n = (*p)->left;

    if(g) {
        if(g->left == *p) {
            g->left = n;
        } else {
            g->right = n;
        }
    }
    n->parent = g;

    (*p)->left = n->right;
    if((*p)->left) {
        (*p)->right->parent = (*p);
    }

    n->right = (*p);
    (*p)->parent = n;
}

void insert_fixup(tNode *n) {
    if( ! (*n)->parent) {
        // N is the root node, i.e., first node of red–black tree
        (*n)->color = BLACK;
        return;
    } else if ((*n)->parent->color == BLACK){
        // N's parent (P) is black
        return;
    } else {
        // N's parent (P) is RED
        tNode u = find_uncle(*n);
        tNode g = find_grandparent(*n);
        if(u && u->color == RED) {
            // N's parent (P) and uncle (U) are red
            (*n)->parent->color = BLACK;
            u->color = BLACK;
            g->color = RED;
            return insert_fixup(&g);
        } else {
            // N's parent (P) is RED and uncle (U) are BLACK
            if((*n == (*n)->parent->right) && ((*n)->parent == g->left)) {
                // N is added to right of left child of grandparent
                tNode p = (*n)->parent;
                rotate_left(&p);
                *n = (*n)->left;
            } else if((*n == (*n)->parent->left) && ((*n)->parent == g->right)) {
                // N is added to left of right child of grandparent
                tNode p = (*n)->parent;
                rotate_right(&p);
                *n = (*n)->right;
            }
            // N is added to left of left child of grandparent, or N is added to right of right child of grandparent
            g = find_grandparent(*n);
            (*n)->parent->color = BLACK;
            g->color = RED;
            if(*n == (*n)->parent->left) {
                rotate_right(&g);
            } else {
                rotate_left(&g);
            }
        }
    }
}

tNode insert(tNode *root, int e) {
    tNode par = NULL, t;
    t = *root;
    while(t) {
        par = t;
        if(e < t->data) {
            t = t->left;
        } else if(e > t->data) {
            t = t->right;
        }
    }
    t = (tNode) malloc (sizeof(tree));
    t->data = e;
    t->left = t->right = NULL;
    t->parent = par;
    t->color = RED;
    if(par) {
        if(e < par->data) {
            par->left = t;
        } else {
            par->right = t;
        }
    } else {
        *root = t;
    }

    insert_fixup(&t);
    return find_root(t);
}

void delete_fixup(tNode *n) {
    tNode s, p, c;
    s = find_brother(*n);  // n's brother
    p = (*n)->parent;  // n's parent
    c = (*n)->left ? (*n)->left : (*n)->right;  // n's child

    // replace n with its child
    if(p) {
        if(p->left == *n) {
            p->left = c;
        } else {
            p->right = c;
        }
    }
    if(c) {
        c->parent = p;
    }

    if((*n)->color == BLACK) {
        if(c && c->color == RED) {
            c->color = BLACK;
        } else if(p) {
            if(s->color == RED) {
                // s is red
                p->color = RED;
                s->color = BLACK;
                if(c == p->left) {
                    rotate_left(&p);
                } else {
                    rotate_right(&p);
                }
            } else {
                // s is black
                if(( ! s->left || s->left->color == BLACK) &&
                   ( ! s->right || s->right->color == BLACK)) {
                    // s's two children are black
                    if(p->color == BLACK) {
                        s->color = RED;
                        delete_fixup(&p);
                    } else {
                        s->color = RED;
                        p->color = BLACK;
                    }
                } else {
                    if(c == p->left &&
                       s->left && s->left->color == RED &&
                       ( ! s->right || s->right->color == BLACK)) {
                        s->color = RED;
                        s->left->color = BLACK;
                        rotate_right(&s);
                    } else if(c == p->right &&
                              s->right && s->right->color == RED &&
                              ( ! s->left || s->left->color == BLACK)) {
                        s->color = RED;
                        s->right->color = BLACK;
                        rotate_left(&s);
                    }
                    if(c) {
                        s = find_brother(c);
                    } else {
                        s = p->left ? p->left : p->right;
                    }
                    s->color = p->color;
                    p->color = BLACK;

                    if (c == p->left) {
                        s->right->color = BLACK;
                        rotate_left(&p);
                    } else {
                        s->left->color = BLACK;
                        rotate_right(&p);
                    }
                }
            }
        }
    }
}

void replace(tNode *x, tNode *y) {
    // FIXME
    int t;
    t = (*x)->data;
    (*x)->data = (*y)->data;
    (*y)->data = t;

    Color c;
    c = (*x)->color;
    (*x)->color = (*y)->color;
    (*y)->color = c;
}

tNode delete(tNode *t, int e) {
    tNode m, n;
    m = find(*t, e);
    if( ! m) {
        return find_root(*t);
    } else {
        if(m->left && m->right) {
            n = find_max(m->left);
            replace(&n, &m);
        } else {
            n = m;
        }
        delete_fixup(&n);
        tNode root;
        root = find_root(*t);
        if(root == n) {
            free(n);
            root = NULL;
        }
        return root;
    }
}

void display(tNode t) {
    if(t) {
        if(t->color == RED) {
            printf("[%d[RED]]\n", t->data);
        } else {
            printf("[%d[BLACK]]\n", t->data);
        }
    } else {
        printf("[nil]\n");
    }
}

int main(void) {
    tNode t = NULL;

    t = insert(&t, 3);
    t = insert(&t, 1);
    t = insert(&t, 2);
    t = insert(&t, 4);
    printf("-----------\n");
    display(t);
    display(t->left);
    display(t->right);
    display(t->right->right);
    printf("-----------\n");
    t = delete(&t, 2);
    display(t);
    display(t->left);
    display(t->right);
    printf("-----------\n");
    t = delete(&t, 3);
    display(t);
    display(t->left);
    display(t->right);

    /* printf("-----------\n"); */
    /* display(t); */
    /* display(t->left); */
    /* display(t->right); */
    /* printf("-----------\n"); */
    /* display(t); */
    /* display(t->left); */
    /* display(t->right); */
    /* display(t->right->right); */
    /* t = insert(&t, 5); */
    /* printf("-----------\n"); */
    /* display(t); */
    /* display(t->left); */
    /* display(t->right); */
    /* display(t->right->left); */
    /* display(t->right->right); */
    /* t = insert(&t, 0); */
    /* t = insert(&t, -1); */
    /* printf("-----------\n"); */
    /* display(t); */
    /* display(t->left); */
    /* display(t->left->left); */
    /* display(t->left->right); */
    return 0;
}
