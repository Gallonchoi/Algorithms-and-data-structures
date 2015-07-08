/**
 * B 树
 *
 * @author 蔡嘉伦
 */

#include <stdio.h>
#include "b-tree.h"

/**
 * 输出菜单
 *
 * @return void
 */
void print_menu() {
    int idx = 1;
    printf(" [%d] 构造新树 \n", idx ++);
    printf(" [%d] 添加元素 \n", idx ++);
    printf(" [%d] 删除元素 \n", idx ++);
    printf(" [%d] 搜索元素 \n", idx ++);
    printf(" [%d] 销毁树 \n", idx ++);
    printf(" [%d] 退出 \n", idx ++);
}

/**
 * 输出个人信息
 *
 * @return void
 */
void print_info() {
    printf("数据结构--B树\n");
    printf("计算机学院 - 13级软件工程4班 蔡嘉伦 3113006352\n");
}

/**
 * 先序输出B树
 *
 * @param t_node B树结点
 * @return void
 */
void print_tree(b_tree * t_node) {
    int key_num, child_num;
    key_num = (*t_node)->key_num;
    child_num = (*t_node)->child_num;
    for(int i = 0; i < key_num; i++) {
        if(i < child_num) {
            print_tree(&(*t_node)->child[i]);
        }
        printf("%d ", (*t_node)->key[i]);
    }
    if(key_num < child_num) {
        print_tree(&(*t_node)->child[child_num - 1]);
    }
}

int main(void) {
    bool is_exit = false;
    b_tree root = NULL;

    print_info();
    while(is_exit == false) {
        print_menu();
        int command, key;
        scanf("%d", &command);
        switch(command) {
        case 1:
            if(root == NULL) {
                create(&root);
                printf("构造B树成功\n");
            } else {
                printf("树已构造\n");
            }
            break;
        case 2:
            if(root == NULL) {
                printf("树未构造\n");
                break;
            }
            printf("新元素: ");
            scanf("%d", &key);
            insert(&root, key);
            break;
        case 3:
            if(root == NULL) {
                printf("树未构造\n");
                break;
            }
            printf("待删除元素: ");
            scanf("%d", &key);
            delete(&root, key);
            break;
        case 4:
            if(root == NULL) {
                printf("树未构造\n");
                break;
            }
            printf("待搜索元素: ");
            scanf("%d", &key);
            result r = search(root, key);
            if(r.is_found == true) {
                printf("找到元素\n");
            } else {
                printf("找不到元素\n");
            }
            break;
        case 5:
            if(root == NULL) {
                printf("树未构造\n");
                break;
            }
            destroy(&root);
            break;
        case 6:
            printf("退出中\n");
            is_exit = true;
            break;
        default:
            break;
        }
        if(is_exit == false) {
            printf("B树先序遍历: ");
            if(root == NULL) {
                printf("未构造");
            } else {
                print_tree(&root);
            }
            printf("\n");
        }
    }

    if(root != NULL) {
        free(root);
    }
    return 0;
}
