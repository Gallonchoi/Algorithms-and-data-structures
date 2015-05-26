// Binary search tree
#include<stdio.h>
#include<stdlib.h>

typedef struct treeNode {
    int data;
    struct treeNode *left;
    struct treeNode *right;

} *treeNode, tree;

treeNode find_min(treeNode node) {
    if(node == NULL) {
        return NULL;
    } else if(node->left) {
        return find_min(node->left);
    } else {
        return node;
    }
}
treeNode find_max(treeNode node) {
    if(node == NULL) {
        return NULL;
    } else if(node->right) {
        return find_max(node->right);
    } else {
        return node;
    }
}

treeNode insert(treeNode node,int data) {
    if(node == NULL) {
        treeNode temp;
        temp = (treeNode)malloc(sizeof(tree));
        temp->data = data;
        temp->left = temp->right = NULL;
        return temp;
    } else if(data > node->data) {
        node->right = insert(node->right, data);
    } else if(data < node->data) {
        node->left = insert(node->left, data);
    }
    return node;
}

treeNode delete(treeNode node, int data) {
    if(node == NULL) {
        printf("Element Not Found\n");
    } else if(data < node->data) {
        node->left = delete(node->left, data);
    } else if(data > node->data) {
        node->right = delete(node->right, data);
    } else if(node->right && node->left) {
        treeNode temp;
        temp = find_min(node->right);
        node->data = temp->data;
        node->right = delete(node->right, temp->data);
    } else if(node->left == NULL) {
        node = node->right;
    } else if(node->right == NULL) {
        node = node->left;
    }
    return node;
}

treeNode find(treeNode node, int data) {
    if(node == NULL) {
        return NULL;
    } else if(data > node->data) {
        return find(node->right,data);
    } else if(data < node->data) {
        return find(node->left,data);
    } else {
        return node;
    }
}

void print_inorder(treeNode node) {
    if(node) {
        print_inorder(node->left);
        printf("%d ",node->data);
        print_inorder(node->right);
    }
}

void print_preorder(treeNode node) {
    if(node) {
        printf("%d ",node->data);
        print_preorder(node->left);
        print_preorder(node->right);
    }
}

void print_postorder(treeNode node) {
    if(node) {
        print_postorder(node->left);
        print_postorder(node->right);
        printf("%d ",node->data);
    }
}

int main(void) {
    treeNode root = NULL;
    root = insert(root, 5);
    root = insert(root, -1);
    root = insert(root, 6);
    root = insert(root, 4);
    print_inorder(root);
    printf("\n");
    root = delete(root, -1);
    print_inorder(root);
    printf("\n");
    treeNode temp = NULL;
    temp = find(root, 6);
    if(temp) {
        printf("Find 6\n");
    } else {
        printf("Counldn't find 6\n");
    }
    temp = find(root, 3);
    if(temp) {
        printf("Find 3\n");
    } else {
        printf("Counldn't find 3\n");
    }
    return 0;
}
