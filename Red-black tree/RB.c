#include "RB.h"
#include "stdlib.h"
#include "stdio.h"
void fix_grandpa(node_rb** root, node_rb* new_sub_root, node_rb* sub_root){
    if (new_sub_root->parent == NULL){
        *root = new_sub_root;
    }
    else if (new_sub_root->parent->left == sub_root){
        new_sub_root->parent->left = new_sub_root;
    }
    else{
        new_sub_root->parent->right = new_sub_root;
    }
}

void right_rotate (node_rb** root, node_rb* sub_root){
    node_rb* new_sub_root = sub_root->left;

    sub_root->left = new_sub_root->right;
    new_sub_root->right = sub_root;

    if (sub_root->left) sub_root->left->parent = sub_root;
    new_sub_root->parent = sub_root->parent;
    sub_root->parent = new_sub_root;

    fix_grandpa(root, new_sub_root, sub_root);
}
void left_rotate (node_rb** root, node_rb* sub_root){
    node_rb* new_sub_root = sub_root->right;

    sub_root->right = new_sub_root->left;
    new_sub_root->left = sub_root;

    if (sub_root->right) sub_root->right->parent = sub_root;
    new_sub_root->parent = sub_root->parent;
    sub_root->parent = new_sub_root;

    fix_grandpa(root, new_sub_root, sub_root);
}
node_rb* create_node (Data* data, node_rb* parent){
    node_rb* new_node = (node_rb*) malloc(sizeof(node_rb));
    if (!new_node){
        printf ("error with memmory allocation\n");
        exit(1);
    }
    new_node->parent = parent;
    new_node->color = RED;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->data = data;

    return new_node;
}
node_rb* get_min_node (node_rb* sub_root){
    while (sub_root && sub_root->left){
        sub_root = sub_root->left;
    }
    return sub_root;
}
color get_color (node_rb* node){
    return node ? node->color : BLACK;
}

char data_compare (Data* in_the_tree, Data* target){
    if (in_the_tree->key < target->key){
        return 1;
    }
    else if (in_the_tree->key > target->key){
        return -1;
    }
    else{
        return 0;
    }
}

void free_data(Data* data){
    //to realise if it need
}

void print (node_rb* root){
    if (!root){
        return;
    }
    print(root->left);
    printf("%d ", root->data);
    print(root->right);
}
void free_tree (node_rb** root){
    if (!(*root)){
        return;
    }
    free_tree(&(*root)->left);
    free_tree(&(*root)->right);

    free_data((*root)->data);
    free((*root)->data);
    free (*root);
    *root = NULL;
}

void insert (node_rb** root, Data* data){
    node_rb* new_node = NULL;
    recursive_insert(root, data, NULL, &new_node);
    if (new_node){
        fix_insert(root, new_node);
    }
}

void fix_insert (node_rb** root, node_rb* x){
    while (x->parent && get_color(x->parent) == RED){
        node_rb* p = x->parent;
        node_rb* g = p->parent;

        if (p == g->left){
            node_rb* u = g->right;
            if (get_color(u) == RED){
                u->color = BLACK;
                p->color = BLACK;
                g->color = RED;
                x = g;
            }
            else{
                if (x == p->right){
                    x = p;
                    left_rotate(root, x);
                    p = x->parent;
                }
                right_rotate(root, g);
                p->color = BLACK;
                g->color = RED;
            }
        }
        else{
            node_rb* u = g->left;
            if (get_color(u) == RED){
                u->color = BLACK;
                p->color = BLACK;
                g->color = RED;
                x = g;
            }
            else{
                if (x == p->left){
                    x = p;
                    right_rotate(root, x);
                    p = x->parent;
                }
                left_rotate(root, g);
                p->color = BLACK;
                g->color = RED;
            }
        }
    }

    (*root)->color = BLACK;
}

void recursive_insert (node_rb** root, Data* data, node_rb* parent, node_rb** new_node){
    if (!(*root)){
        *new_node = create_node(data, parent);
        *root = *new_node;
        return;
    }
    char cmp = data_compare((*root)->data, data);
    if (cmp > 0){
        recursive_insert(&(*root)->right, data, *root, new_node);
    }
    else if (cmp < 0){
        recursive_insert(&(*root)->left, data, *root, new_node);
    }
    else{
        return;
    }
}
void del (node_rb** root, Data* data){
    node_rb* node_to_del = NULL;
    recursive_del(root, data, &node_to_del);
    if (node_to_del){
        fix_del(root, node_to_del);

        node_rb* p = node_to_del->parent;
        if (p){
            if (p->left == node_to_del) p->left = NULL;
            else p->right = NULL;
        }
        else{
            *root = NULL;
        }

        free(node_to_del);
    }

}
void fix_del (node_rb** root, node_rb* x){
    while (x != *root && get_color(x) == BLACK){
        node_rb* p = x->parent;
        node_rb* s = NULL;
        if (p->left == x){
            s = p->right;
            if (get_color(s) == RED){
                s->color = BLACK;
                p->color = RED;
                left_rotate(root, p);
                s = p->right;
            }
            if (get_color(s->left) == BLACK && get_color(s->right) == BLACK){
                s->color = RED;
                x = p;
            }
            else{
                if (get_color(s->right) == BLACK){
                    s->left->color = BLACK;
                    s->color = RED;
                    right_rotate(root, s);
                    s = p->right;
                }
                s->color = p->color;
                p->color = BLACK;
                s->right->color = BLACK;
                left_rotate(root, p);
                x = *root;
            }
        }
        else{
            s = p->left;
            if (get_color(s) == RED){
                p->color = RED;
                s->color = BLACK;
                right_rotate(root, p);
                s = p->left;
            }

            if (get_color(s->left) == BLACK && get_color(s->right) == BLACK){
                s->color = RED;
                x = p;
            }
            else{
                if (get_color(s->left) == BLACK){
                    s->color = RED;
                    s->right->color = BLACK;
                    left_rotate(root,s);
                    s = p->left;
                }

                s->color = p->color;
                p->color = BLACK;
                s->left->color = BLACK;
                right_rotate(root, p);
                x = *root;
            }
        }

    }

    if (x != NULL) {
        x->color = BLACK;
    }
}

void del_without_clean_data(node_rb** root, Data* data, node_rb** node_to_del){
    if (!(*root)){
        return;
    }
    char cmp = data_compare((*root)->data, data);
    if (cmp > 0){
        del_without_clean_data(&(*root)->right, data, node_to_del);
    }
    else if (cmp < 0){
        del_without_clean_data(&(*root)->left, data, node_to_del);
    }
    else{
        node_rb* temp =(*root)->right;
        if (temp){
            temp->parent = (*root)->parent;
            temp->color = BLACK;
            free(*root);
            *root = temp;
            return;
        }
        else{
            if (get_color(*root) == RED){
                free(*root);
                *root = NULL;
                return;
            }
            else{
                *node_to_del = *root;
            }
        }
    }
    
    return;
}
void recursive_del (node_rb** root, Data* data, node_rb** node_to_del){
    if (!(*root)){
        return;
    }
    char cmp = data_compare((*root)->data, data);
    if (cmp > 0){
        recursive_del(&(*root)->right, data, node_to_del);
    }
    else if (cmp < 0){
        recursive_del(&(*root)->left, data, node_to_del);
    }
    else{
        if ((*root)->left && (*root)->right){
            node_rb* min_node  = get_min_node((*root)->right);
            free_data((*root)->data);
            free((*root)->data);
            (*root)->data = min_node->data;
            del_without_clean_data(&(*root)->right, min_node->data, node_to_del);
        }
        else{
            node_rb* temp = (*root)->left ? (*root)->left : (*root)->right;
            if (temp){
                temp->parent = (*root)->parent;
                temp->color = BLACK;
                free_data((*root)->data);
                free((*root)->data);
                free(*root);
                *root = temp;
                return;
            }
            else{
                if (get_color(*root) == RED){
                    free_data((*root)->data);
                    free((*root)->data);
                    free(*root);
                    *root = NULL;
                    return;
                }
                else{
                    free_data((*root)->data);
                    free((*root)->data);
                    *node_to_del = *root;
                }
            }
        }
    }
    return;
}
node_rb* search (node_rb* root, Data* data){
    node_rb* cur = root;
    char cmp;
    while (cur){
        cmp = data_compare(root->data, data);
        if (cmp > 0){
            cur = cur->right;
        }
        else if (cmp < 0){
            cur = cur->left;
        }
        else{
            return cur;
        }
    }
    return NULL;
}