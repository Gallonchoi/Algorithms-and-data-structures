#ifndef B_TREE_H_
#define B_TREE_H_

#include <stdlib.h>

#define degree 2  // 结点二分之一度数
#define middle (degree - 1)  // 结点(除根)最少关键字数目

typedef enum {true, false} bool;

typedef struct b_tree {
    int key_num;  // 关键字数目
    int key[degree * 2 - 1];  // 关键字
    int child_num;  // 子结点数目
    struct b_tree * child[degree * 2];  // 子结点
} b_tree_t, *b_tree;

typedef struct search_result {
    // 搜索结果
    bool is_found;  // 搜索是否成功
    b_tree * target_node, * parent_node;  // 目标结点及该结点的父结点
    int idx, t_idx;  // 目标关键字在结点的索引, 及目标结点在父结点的索引
} result;

bool is_full(b_tree t_node);  // 判断结点内关键字是否满
void create(b_tree * t_node);  // 建立新结点
void insert(b_tree * root, int key);  // 插入关键字
result search(b_tree t_node, int key);  // 搜索关键字
void delete(b_tree * t_node, int key);  // 删除关键字
void split(b_tree * t_node, b_tree * parent, int idx);  // 分离结点
void combine(b_tree * parent, int idx);  // 合并结点
result search_process(b_tree t_node, b_tree p_node, int p_idx, int key);  // 搜索关键字-子函数
void delete_process(b_tree * t_node, int idx, b_tree * parent, int p_idx);  // 删除关键字-子函数
void balance(b_tree * parent, int t_idx);

bool is_full(b_tree t_node) {
    return (t_node->key_num == degree * 2 - 1) ? true : false;
}

void create(b_tree * t_node) {
    *t_node = (b_tree) malloc (sizeof(b_tree_t));
    (*t_node)->child_num = 0;
    (*t_node)->key_num = 0;
}

void split(b_tree * t_node, b_tree * parent, int idx) {
    // 获取待提升关键字
    int middle_key = (*t_node)->key[middle];
    b_tree before, after;
    create(&before);
    create(&after);
    before->key_num = middle;
    after->key_num = middle;
    before->child_num = after->child_num = (*t_node)->child_num/2;
    for(int i = 0; i <= middle; i++) {
        // 分离子结点
        before->child[i] = (*t_node)->child[i];
        after->child[i] = (*t_node)->child[middle+i+1];
        // 分离关键字
        if(i < middle) {
            before->key[i] = (*t_node)->key[i];
            after->key[i] = (*t_node)->key[middle+i+1];
        }
    }
    // 连接新结点
    if( ! parent) {
        // 当前结点为根结点
        // 创建新根
        b_tree new_root;
        create(&new_root);
        new_root->key_num = 1;
        new_root->key[0] = middle_key;
        new_root->child_num = 2;
        new_root->child[0] = before;
        new_root->child[1] = after;
        // 设新根为根结点
        *t_node = new_root;
    } else {
        // 提升中间关键字到父结点
        if(idx < (*parent)->key_num) {
            // 在父结点分空间给新关键字及新子结点
            for(int i = (*parent)->key_num - 1; i >= idx; i--) {
                (*parent)->key[i+1] = (*parent)->key[i];
                (*parent)->child[i+2] = (*parent)->child[i+1];
            }
        }
        (*parent)->key[idx] = middle_key;
        (*parent)->child[idx] = before;
        (*parent)->child[idx + 1] = after;
        (*parent)->child_num ++;
        (*parent)->key_num ++;
    }
}

void combine(b_tree * parent, int idx) {
    b_tree l_node, r_node;
    int key, new_key_num, new_child_num;
    key = (*parent)->key[idx];  // 目标关键字
    l_node = (*parent)->child[idx];  // 左子结点(作为合并后的新结点)
    r_node = (*parent)->child[idx + 1];  // 右子结点

    new_key_num = l_node->key_num + r_node->key_num + 1;  // 重新计算关键字数目
    new_child_num = l_node->child_num + r_node->child_num;  // 重新计算子结点数目

    /* 在父结点剔除目标关键字 */
    for(int i = idx+1; i < (*parent)->key_num; i++) {
        (*parent)->key[i - 1] = (*parent)->key[i];
        (*parent)->child[i] = (*parent)->child[i + 1];
    }
    (*parent)->key_num --;
    (*parent)->child_num --;

    // 目标关键字加入到左子结点末尾
    l_node->key[l_node->key_num ++] = key;

    // 将右子结点关键字及子结点加入到左子结点中
    for(int i = l_node->key_num, j = 0; i < new_key_num; i++, j++) {
        l_node->key[i] = r_node->key[j];
        l_node->child[i] = r_node->child[j];
    }
    l_node->child[new_key_num] = r_node->child[r_node->child_num - 1];
    l_node->key_num = new_key_num;
    l_node->child_num = new_child_num;

    if((*parent)->key_num == 0) {
        // 父结点为根结点, 并且其为空结点
        // 建立新父结点
        (*parent)->key_num = l_node->key_num;
        (*parent)->child_num = l_node->child_num;
        for(int i = 0; i < l_node->key_num; i++) {
            (*parent)->key[i] = l_node->key[i];
            (*parent)->child[i] = l_node->child[i];
        }
        (*parent)->child[l_node->child_num-1] = l_node->child[l_node->child_num-1];
    }
}


void insert(b_tree * root, int key) {
    if( ! *root) {
        exit(1);
    } else {
        b_tree * t_node, * parent;
        int idx;
        t_node = root;
        parent = NULL;
        while(1) {
            // 迭代到合适的插入位置
            // 如果结点关键字已满, 分离该结点
            if(is_full(*t_node) == true) {
                if( ! parent) {
                    // 该结点为根结点
                    split(t_node, NULL, 0);
                } else {
                    split(t_node, parent, idx);
                    t_node = parent;
                }
            }
            // 找到合适的关键字位置
            int i = 0;
            while(i < (*t_node)->key_num && key > (*t_node)->key[i]) {
                i ++;
            }
            if(i < (*t_node)->child_num) {
                // 当前结点是内结点, 则继续迭代到子结点中
                idx = i;
                parent = t_node;
                t_node = &(*t_node)->child[i];
            } else {
                // 当前结点是叶子结点, 直接插入
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

result search_process(b_tree t_node, b_tree parent, int t_idx, int key) {
    int i = 0;
    while(i < t_node->key_num && key > t_node->key[i]) {
        i ++;
    }
    if(i < t_node->key_num && key == t_node->key[i]) {
        if(parent && t_node->key_num < degree) {
            balance(&parent, t_idx);
            return search(parent, key);
        }
        result r;
        r.is_found = true;
        r.target_node = &t_node;
        r.idx = i;
        if(parent) {
            r.parent_node = &parent;
            r.t_idx = t_idx;
        } else {
            r.parent_node = NULL;
        }
        return r;
    } else if(t_node->child_num == 0 || t_node->child[i] == NULL) {
        result r;
        r.is_found = false;
        return r;
    } else {
        if(parent && t_node->key_num < degree) {
            balance(&parent, t_idx);
            return search(parent, key);
        }
        return search_process(t_node->child[i], t_node, i, key);
    }
}

result search(b_tree t_node, int key) {
    return search_process(t_node, NULL, 0, key);
}

void balance(b_tree * parent, int t_idx) {
    b_tree * t_node;
    t_node = &(*parent)->child[t_idx];
    if((*t_node)->key_num < degree) {
        b_tree bro;  // 兄弟结点
        bool bro_is_left;
        int b_key;
        bro = NULL;
        // 查找合适的兄弟结点
        if(t_idx > 0) {
            // 兄弟结点为目标结点左兄弟
            bro = (*parent)->child[t_idx - 1];
            bro_is_left = true;
        }
        if(t_idx < (*parent)->child_num-1) {
            // 兄弟结点为目标结点右兄弟
            bro = (*parent)->child[t_idx + 1];
            bro_is_left = false;
        }

        if(bro) {
            if(bro->key_num < degree) {
                // 兄弟结点关键字数目为"最少关键字数目"
                // 合并目标结点及其兄弟结点
                if(bro_is_left == false) {
                    // 兄弟结点为目标结点的右兄弟
                    combine(parent, t_idx);
                } else {
                    // 兄弟结点为目标结点的左兄弟
                    combine(parent, t_idx - 1);
                }
            } else {
                // 兄弟结点关键字大于"最少关键字数目"
                // 拉取父结点关键字到目标结点替换待删除关键字
                // 从兄弟结点提取关键字到父结点
                if(bro_is_left == false) {
                    // 兄弟结点为目标结点右兄弟
                    // 父结点关键字加入到目标结点中
                    (*t_node)->key[(*t_node)->key_num++] = (*parent)->key[t_idx];
                    if((*t_node)->child_num > 0) {
                        (*t_node)->child[(*t_node)->child_num++] = bro->child[0];
                    }
                    // 兄弟关键字替换父结点关键字
                    (*parent)->key[t_idx] = bro->key[0];

                    // 删除兄弟结点关键字及子结点
                    for(int i = 0; i < bro->key_num - 1; i++) {
                        bro->key[i] = bro->key[i + 1];
                        bro->child[i] = bro->child[i + 1];
                    }
                    if(bro->child_num > 0) {
                        bro->child[bro->child_num-2] = bro->child[bro->child_num-1];
                    }
                    // 无需更新关键字索引
                } else {
                    // 兄弟结点为目标结点左兄弟
                    // 父结点关键字加入到目标结点中
                    if((*t_node)->child_num > 0) {
                        (*t_node)->child[(*t_node)->child_num++] = (*t_node)->child[(*t_node)->child_num - 1];
                    }
                    for(int i = (*t_node)->key_num; i > 0; i--) {
                        (*t_node)->key[i] = (*t_node)->key[i - 1];
                        (*t_node)->child[i] = (*t_node)->child[i - 1];
                    }
                    (*t_node)->key[0] = (*parent)->key[t_idx - 1];
                    // 兄弟关键字替换父结点关键字
                    (*parent)->key[t_idx - 1] = bro->key[bro->key_num - 1];
                    if((*t_node)->child_num > 0) {
                        (*t_node)->child[0] = bro->child[bro->child_num - 1];
                    }
                    // 更新关键字数目
                    (*t_node)->key_num++;
                }
                bro->key_num --;
                if(bro->child_num > 0) {
                    bro->child_num --;
                }
            }
        }
    }
}


/**
 * @param t_node 目标结点
 * @param idx 目标关键字在目标结点中的索引
 * @param parant 目标结点的父结点
 * @param p_idx 目标结点在父结点中的索引
 */
void delete_process(b_tree * t_node, int idx, b_tree * parent, int p_idx) {
    int key = (*t_node)->key[idx];

    if(parent && (*t_node)->key_num < degree) {
        balance(parent, p_idx);
        delete(parent, key);
        return;
    }

    if((*t_node)->child_num == 0) {
        // 目标结点是叶子结点
        // 直接删除叶子结点中的关键字
        for(int i = idx+1; i < (*t_node)->key_num; i++) {
            (*t_node)->key[i-1] = (*t_node)->key[i];
        }
        (*t_node)->key_num --;
    } else {
        // 目标结点为内结点
        b_tree left_t, right_t;
        left_t = (*t_node)->child[idx];  // 左子结点
        right_t = (*t_node)->child[idx+1];  // 右子结点
        if(left_t->key_num >= degree || right_t->key_num >= degree) {
            // 目标关键字的左右子结点的关键字数目有不为最少关键字数目
            // 提升子结点关键字替换掉待删除结点, 并递归删除该子结点关键字
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
            // 目标关键字的左右子结点的关键字数目都为最少关键字数目
            // 合并待删除关键字及左右子结点, 再递归删除待删关键字
            // 目标结点不为根且关键字为1
            int key = (*t_node)->key[idx];
            combine(t_node, idx);
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
        delete_process(&target, idx, r.parent_node, r.t_idx);
    }
}
#endif
